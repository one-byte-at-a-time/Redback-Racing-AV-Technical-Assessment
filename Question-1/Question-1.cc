/// Question 1: This question requests you to fix all compile errors in the main function and any classes. Address
/// the comments given inside the code.
/// Your code should run and compile without any warnings or errors.


#include <iostream>
#include <map>
#include <vector>
#include <initializer_list>
#include <cmath>
#include <memory>

const int values[] = { 1,2,3,4,5 };
const int val_size = sizeof values / sizeof(int);



class Foo {

    std::vector<int> mElements;

    public:
        Foo(std::initializer_list<int> list)
        : mElements(list) {}
};


struct person {
    int age;
    float weight;
};


void show_output(const int** pp)
{
    std::cout << pp << " : " << *pp << " : " << **pp << std::endl;
}



int main() {

    int x = -10;
    int y = 15;

    std::cout << "(" << x << "," << y << ")" << std::endl;

    // print integer ratios of y:x till x < y
    // invalid ratios should print 0

    // NB: not sure if the comments here are correct -> this will output nothing if till x < y as x is already < y
    while (x >= y)
    {   
        if (x == 0){
            std::cout << "ratio: " << 0 << std::endl;
        } else {
            std::cout << "ratio: " << y/x << std::endl;
        }
        x++;
        y--;
    }


    int i = 1, j = 1; // don't change values

    if ((i != 3) && (j == 1))
    {
        std::cout << "j is 1 and i is not 3\n";
    }
    else {
        std::cout << "either j is NOT 1, or i is set to 3\n";

    }


    typedef std::map<int, double> valmap;
    valmap m;

    for (int i = 0; i < val_size; i++)
        m.insert(std::make_pair(values[i], pow(values[i], .5)));

    m.insert(std::make_pair(1, 2.0));

    const int n = 1;
    const int* p = &n;
    const int** pp = &p;
    show_output(pp);

    // Initialise a person on the heap with the use of smart pointers (unique_ptr) instead.
    std::unique_ptr<person> ptr = std::make_unique<person>();
    ptr->age = 10;
    ptr->weight = 55.5;

    // Initialise with 5 integers
    // Foo foo;
    std::initializer_list<int> list = {1,2,3,4,5};
    Foo foo(list);

    return 0;
}
