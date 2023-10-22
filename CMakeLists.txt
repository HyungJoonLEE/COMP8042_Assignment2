cmake_minimum_required(VERSION 3.25)
project(8042_Assignment_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(8042_Assignment_2
        include/Date.h
        include/Utils.h
        include/UnorderedSet.h
        include/Stack.h/
        include/HashTable.h
        src/BookRecommendation.cpp
        tests/TestEnvironment.h
        tests/UnorderedSetTests.h
        tests/HashTableTests.h
        tests/BookRecommendationTests.h
        main.cpp)
