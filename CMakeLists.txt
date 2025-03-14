cmake_minimum_required(VERSION 3.14)
project(nmp25)

enable_testing()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -lpthread)
endif()

foreach(IDX RANGE 1 3 1)
	add_executable(Question-${IDX}-out Question-${IDX}/Question-${IDX}.cc)
	target_link_libraries(Question-${IDX}-out pthread)
	add_test(question-${IDX} Question-${IDX}-out)
endforeach()


# Question 4: Extension Task. You need to integrate Google Tests here

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add the test executable for Question 4
add_executable(Question-4-out Question-4/Question-4.cc)

# Link gMock and gTest to your test executable
target_link_libraries(Question-4-out gtest gmock gtest_main)

# Add the test to CTest
add_test(NAME question-4 COMMAND Question-4-out)