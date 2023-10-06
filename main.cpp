/*
 * Copyright (c) Sayyedhassan Shavarani 2023
 * All rights reserved. Unauthorized redistribution is prohibited.
 */
#include <iostream>
#include <cstdlib>
#include "tests/UnorderedSetTests.h"
#include "tests/HashTableTests.h"
#include "tests/BookRecommendationTests.h"
#include "include/LExceptions.h"
/*
 * This is the driver file which directs the project on testing different modules.
 * For each new testing function add a new case with the next available "module_choice" to be able to test it out.
 */

int main(int argc, char* argv[]) {
    int module_choice = 0;

    if (argc > 1) {
        module_choice = atoi(argv[1]);
    }
    switch (module_choice) {
        case 0: // Testing Assignment 2:
            std::cout << ">> UnorderedSet:\t\t\t\t";
            unorderedSetTests();
            std::cout << ">> HashTable:\t\t\t\t\t";
            hashTableTests();
            std::cout << ">> Book Recommender System: \t";
            bookRecommendationTests();
            break;
        default:
            throw std::invalid_argument("Invalid module choice");
            break;
    }
    return 0;
}
