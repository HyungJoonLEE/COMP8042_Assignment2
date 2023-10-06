/*
 * Copyright (c) Sayyedhassan Shavarani 2023
 * All rights reserved. Unauthorized redistribution is prohibited.
 */
#ifndef HASHTABLETESTS_H
#define HASHTABLETESTS_H
#include <iostream>
#include <cmath>
#include "../include/Utils.h"
#include "TestEnvironment.h"
#include "../include/HashTable.h"

std::pair<int, int> hashTableTestForBookDataStructure() {
    int passedTests = 0;
    TestEnvironment env;
    HashTable<std::string, Book> bookTable(8);
    bookTable.insert(env.book1.ISBN, env.book1);
    bookTable.insert(env.book2.ISBN, env.book2);
    passedTests += assert(bookTable.size() == 2);
    Book* foundBook = bookTable.search(env.book1.ISBN);
    passedTests += assert(foundBook != nullptr);
    passedTests += assert(foundBook->ISBN == env.book1.ISBN);
    Book updatedBook = env.book1;
    updatedBook.title = "Updated Title";
    bookTable.updateValueForKey(env.book1.ISBN, updatedBook);
    foundBook = bookTable.search(env.book1.ISBN);
    passedTests += assert(foundBook != nullptr);
    passedTests += assert(foundBook->title == "Updated Title");
    passedTests += assert(bookTable.remove(env.book2.ISBN));
    passedTests += assert(!bookTable.remove(env.book2.ISBN));
    passedTests += assert(bookTable.size() == 1);
    foundBook = bookTable.search("NonexistentISBN");
    passedTests += assert(foundBook == nullptr);
    Book newBook = env.book3;
    bookTable[newBook.ISBN] = newBook;
    foundBook = bookTable.search(newBook.ISBN);
    passedTests += assert(foundBook != nullptr);
    passedTests += assert(foundBook->title == env.book3.title);
    passedTests += assert(bookTable.loadFactor() == 0.25); // 2 elements / 8 slots
    bookTable.clear();
    passedTests += assert(bookTable.size() == 0);
    for (int i = 1; i <= 10; ++i) {
        Book book = env.book4;
        book.ISBN += std::to_string(i);
        bookTable.insert(book.ISBN, book);
    }
    passedTests += assert(bookTable.loadFactor() == 0.625); // 10 elements / 16 slots
    int count = 0;
    for (auto it = bookTable.begin(); it != bookTable.end(); ++it) {
        ++count;
    }
    passedTests += assert(count == 10);
    return std::make_pair(passedTests, 15);
}

std::pair<int, int> hashTableTestForUserDataStructure() {
    int passedTests = 0;
    TestEnvironment env;
    HashTable<std::string, Patron> userTable(8);
    userTable.insert(env.user1.ID, env.user1);
    userTable.insert(env.user2.ID, env.user2);
    passedTests += assert(userTable.size() == 2);
    Patron* foundUser = userTable.search(env.user1.ID);
    passedTests += assert(foundUser != nullptr);
    passedTests += assert(foundUser->ID == env.user1.ID);
    Patron updatedUser = env.user1;
    updatedUser.name = "Updated Name";
    userTable.updateValueForKey(env.user1.ID, updatedUser);
    foundUser = userTable.search(env.user1.ID);
    passedTests += assert(foundUser != nullptr);
    passedTests += assert(foundUser->name == "Updated Name");
    passedTests += assert(userTable.remove(env.user2.ID));
    passedTests += assert(!userTable.remove(env.user2.ID));
    passedTests += assert(userTable.size() == 1);
    foundUser = userTable.search("NonexistentISBN");
    passedTests += assert(foundUser == nullptr);
    Patron newUser = env.user3;
    userTable[newUser.ID] = newUser;
    foundUser = userTable.search(newUser.ID);
    passedTests += assert(foundUser != nullptr);
    passedTests += assert(foundUser->ID == env.user3.ID);
    passedTests += assert(userTable.loadFactor() == 0.25); // 2 elements / 8 slots
    userTable.clear();
    passedTests += assert(userTable.size() == 0);
    for (int i = 0; i <= 9; ++i) {
        Patron user = env.user4;
        user.ID += std::to_string(i);
        userTable.insert(user.ID, user);
    }
    passedTests += assert(userTable.loadFactor() == 0.625); // 10 elements / 16 slots
    int count = 0;
    for (auto it = userTable.begin(); it != userTable.end(); ++it) {
        ++count;
    }
    passedTests += assert(count == 10);
    return std::make_pair(passedTests, 15);
}


std::pair<int, int> hashTableTestForGeneralDataStructures() {
    int passedTests = 0;
    HashTable<int, int> intHashTable(20);
    passedTests += assert(intHashTable.size() == 0);
    intHashTable.insert(1, 100);
    intHashTable.insert(2, 200);
    passedTests += assert(intHashTable.size() == 2);
    passedTests += assert(*intHashTable.search(1) == 100);
    intHashTable.updateValueForKey(1, 150);
    passedTests += assert(*intHashTable.search(1) == 150);
    passedTests += assert(intHashTable.remove(2));
    passedTests += assert(!intHashTable.remove(2));
    passedTests += assert(intHashTable.size() == 1);
    passedTests += assert(intHashTable.search(2) == nullptr);
    passedTests += assert(intHashTable.loadFactor() == 0.05); // 1 element / 20 slots
    intHashTable[3] = 300;
    passedTests += assert(*intHashTable.search(3) == 300);
    HashTable<int, int>::Iterator it = intHashTable.begin();
    HashTable<int, int>::Iterator endIt = intHashTable.end();
    passedTests += assert(it != endIt);
    ++it;
    ++it;
    passedTests += assert(it == endIt);

    for (int i = 4; i <= 20; ++i) // this for loop initiates a rehash and doubles the size of the table
        intHashTable.insert(i, i * 100);
    passedTests += assert(intHashTable.loadFactor() ==  0.475); // 19 elements / 40 slots
    passedTests += assert(intHashTable.tableSize == 40);
    intHashTable.clear();
    passedTests += assert(intHashTable.size() == 0);
    HashTable<std::string, std::string> stringHashTable(2);
    stringHashTable.insert("apple", "fruit");
    stringHashTable.insert("banana", "fruit");
    passedTests += assert(stringHashTable.size() == 2);
    passedTests += assert(*stringHashTable.search("apple") == "fruit");
    passedTests += assert(stringHashTable.remove("banana"));
    passedTests += assert(!stringHashTable.remove("banana"));
    HashTable<std::string, std::string>::Iterator strIt = stringHashTable.begin();
    HashTable<std::string, std::string>::Iterator strEndIt = stringHashTable.end();
    passedTests += assert(strIt != strEndIt);
    ++strIt;
    passedTests += assert(strIt == strEndIt);
    passedTests += assert(stringHashTable.loadFactor() == 0.5); // 1 element / 2 slots
    return std::make_pair(passedTests, 22);
}


int hashTableTests() {
    int passedTests = 0;
    int totalTests = 0;
    std::pair<int, int> r1 = hashTableTestForBookDataStructure();
    passedTests += r1.first;
    totalTests += r1.second;
    std::pair<int, int> r2 = hashTableTestForUserDataStructure();
    passedTests += r2.first;
    totalTests += r2.second;
    std::pair<int, int> r3 = hashTableTestForGeneralDataStructures();
    passedTests += r3.first;
    totalTests += r3.second;
    double grade = static_cast<double>(passedTests * 100) / totalTests;
    grade = std::round(grade * 10) / 10;
    std::cout << "Total tests passed: " << passedTests << " out of " << totalTests << " (" << grade << "%)"  << std::endl;
    return 0;
}
#endif //HASHTABLETESTS_H
