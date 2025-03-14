#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <filesystem>

// Question 3: This is an extension task that requires you to decode sensor data from a CAN log file.
// CAN (Controller Area Network) is a communication standard used in automotive applications (including Redback cars)
// to allow communication between sensors and controllers.
//
// Your Task: Using the definition in the Sensors.dbc file, extract the "WheelSpeedRR" values
// from the candump.log file. Parse these values correctly and store them in an output.txt file with the following format:
// (<UNIX_TIME>): <DECODED_VALUE>
// eg:
// (1705638753.913408): 1234.5
// (1705638754.915609): 6789.0
// ...
// The above values are not real numbers; they are only there to show the expected data output format.
// You do not need to use any external libraries. Use the resources below to understand how to extract sensor data.
// Hint: Think about manual bit masking and shifting, data types required,
// what formats are used to represent values, etc.
// Resources:
// https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial
// https://www.csselectronics.com/pages/can-dbc-file-database-intro


/**
NOTES:

BO_ == message definitions

BO_ 1797 ECU_WheelSpeed: 8 Vector__XXX 

message, called ECU_WheelSpeed w/id 1797 and a length of 8 bytes

- multiple signals in the message:

WheelSpeedRR : 32|16@0+ (0.1,0) [0|0] "km/h" Vector__XXX

Signal Name : WheelSpeedRR
32|16: starts at bit 32, 16bits long
@O+ : signal is unsigned, and right aligned
(0.1,0): scale and offset -> signal value is scaled by 0.1 and offset by 0
[0][0]: value range of signal -> unused in this instance
km/h : signal units
Vector_XXX: The ECU or node that sends the signal

PLAN:

We need to find the ECU_WheelSpeed Messages and extract the relevant bits for WheelSpeedRR.
But I don't see a 1797#...

Ah! This is dumb af:
In the dbc file, the can message Id is in decimal (1797#)
but in the log, its in hexidecimal...

No wonder the early porsche ems sucked...

NB: right aligned means the MSB is the second byte, the lsb is the first, just need to swap these two around

STEPS:

1. go through all lines

2. extract unix timecode, values and messageId
3. If messageId matches:
    - extract WheelSpeedRR bits (32->47)
    - account for right-aligned, scale ,etc
    - output with timestamp
*/

// HELPERS:
std::vector<std::string> splitString(std::string string, char delimiter);
std::vector<std::string> extractLog(std::string rawLog);

int main() {

    // Just to check paths - ensure access to candump.log
    std::filesystem::path cwd = std::filesystem::current_path();
    // std::cout << "Current working directory: " << cwd << std::endl;

    // Open log file
    std::ifstream file("../Question-3/candump.log");

    if (!file){
        std::cout << "file not found" << std::endl;
    }

    // Open output file:
    std::ofstream outputFile("../Question-3/output.txt");


    // read logs
    std::string rawLog;
    while (std::getline(file,rawLog)){
   
        std::vector<std::string> log = extractLog(rawLog);

        // remove messages without values
        if (log.size() != 3){
            continue;
        }

        // find message id 1797# (6F5#, ECU_WheelSpeed)
        std::string messageId = log[1];
        if (messageId != "6F5"){
            continue;
        } 

        // Extract WheelSpeedRR (easier as string that bit manipulation)
        // Vals are right-aligned, so MSB is the second set of values 
        std::string lsb = log[2].substr(8,2);
        std::string msb = log[2].substr(10,2);
        std::string wheelSpeedRR = msb + lsb;
        
        // convert to decimal - WheelSpeedRR is unsigned
        double wheelSpeedRR_val  = std::stoi(wheelSpeedRR, nullptr, 16);

        // apply scale + offset
        wheelSpeedRR_val *= 0.1;
        
        // write cleaned log to file:
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << wheelSpeedRR_val;
        std::string decodedLog = log[0] + " " + oss.str();
        outputFile << decodedLog << std::endl;
    }

    // close file
    file.close();

    return 0;
}

// HELPER FUNCTIONS:

// String Splitter Helper
std::vector<std::string> splitString(std::string string, char delimiter){

    std::istringstream stream(string);
    std::string word;
    std::vector<std::string> words;

    while (std::getline(stream, word, delimiter)){
            words.push_back(word);
    }

    return words;
}

// Extract Timestamp, MessageId and Values from Raw Log
std::vector<std::string> extractLog(std::string rawLog){

    // Split out timestamp
    std::vector<std::string> log = splitString(rawLog, ' ');

    // Split out MessageID and vals (if any)
    std::vector<std::string> values = splitString(log[2], '#');

    // Add timestamp to values vector
    values.insert(values.begin(), log[0]);

    return values;
}
