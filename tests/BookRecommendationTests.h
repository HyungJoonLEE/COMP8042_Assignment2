/*
 * Copyright (c) Sayyedhassan Shavarani 2023
 * All rights reserved. Unauthorized redistribution is prohibited.
 */
#ifndef BOOKRECOMMENDATIONTESTS_H
#define BOOKRECOMMENDATIONTESTS_H
#include <iostream>
#include <cmath>
#include "TestEnvironment.h"
#include "../include/BookRecommendation.h"

BookRecommendation setUp(){
    TestEnvironment te;
    BookRecommendation bookRecommendation;
    bookRecommendation.addUserBorrowedBook(te.user1, te.book1);
    bookRecommendation.addUserBorrowedBook(te.user1, te.book2);
    bookRecommendation.addUserBorrowedBook(te.user1, te.book3);
    bookRecommendation.addUserBorrowedBook(te.user2, te.book1);
    bookRecommendation.addUserBorrowedBook(te.user2, te.book2);
    bookRecommendation.addUserBorrowedBook(te.user2, te.book4);
    bookRecommendation.addUserBorrowedBook(te.user3, te.book1);
    bookRecommendation.addUserBorrowedBook(te.user3, te.book3);
    bookRecommendation.addUserBorrowedBook(te.user3, te.book4);
    bookRecommendation.addUserBorrowedBook(te.user4, te.book2);
    bookRecommendation.addUserBorrowedBook(te.user4, te.book3);
    bookRecommendation.addUserBorrowedBook(te.user6, te.book6);
    bookRecommendation.addUserBorrowedBook(te.user6, te.book7);
    bookRecommendation.addUserBorrowedBook(te.user6, te.book8);
    return bookRecommendation;
}

std::pair<int, int> bookRecommendationTestCalculateSimilarity() {
    int passedTests = 0;
    BookRecommendation bookRecommendation = setUp();
    passedTests += assert(bookRecommendation.calculateSimilarity("user1", "user1") == 1.0);
    passedTests += assert(bookRecommendation.calculateSimilarity("user1", "user2") == 0.5);
    passedTests += assert(bookRecommendation.calculateSimilarity("user1", "user6") == 0.0);
    double d1 = bookRecommendation.calculateSimilarity("user4", "user1");
    double d2 = bookRecommendation.calculateSimilarity("user1", "user4");
    passedTests += assert(d1 == d2);
    return std::make_pair(passedTests, 4);
}

std::pair<int, int> bookRecommendationTestGetNeighborhood() {
    int passedTests = 0;
    BookRecommendation bookRecommendation = setUp();
    UnorderedSet<std::string> neighborhood = bookRecommendation.getNeighborhood("user1", 10);
    passedTests += assert(neighborhood.size() == 3);
    passedTests += assert(neighborhood.search("user2"));
    UnorderedSet<std::string> neighborhood2 = bookRecommendation.getNeighborhood("user1", 2);
    passedTests += assert(neighborhood2.size() == 2);
    passedTests += assert(neighborhood2.search("user2"));
    UnorderedSet<std::string> emptyNeighborhood = bookRecommendation.getNeighborhood("user6", 10);
    passedTests += assert(emptyNeighborhood.size() == 0);
    return std::make_pair(passedTests, 5);
}

std::pair<int, int> bookRecommendationTestGetRecommendedBooks1() {
    int passedTests = 0;
    TestEnvironment te;
    BookRecommendation bookRecommendation = setUp();
    UnorderedSet<Book> recommendedBooks = bookRecommendation.getRecommendedBooks(UnorderedSet<std::string>(), "user3");
    passedTests += assert(recommendedBooks.size() == 0);
    return std::make_pair(passedTests, 1);
}

std::pair<int, int> bookRecommendationTestGetRecommendedBooks2() {
    int passedTests = 0;
    TestEnvironment te;
    BookRecommendation bookRecommendation = setUp();
    UnorderedSet<std::string> neighborhood = bookRecommendation.getNeighborhood("user1", 2);
    UnorderedSet<Book> recommendedBooks = bookRecommendation.getRecommendedBooks(neighborhood, "user1");
    passedTests += assert(recommendedBooks.size() == 1);
    passedTests += assert(recommendedBooks.search(te.book4));
    return std::make_pair(passedTests, 2);
}

std::pair<int, int> bookRecommendationTestGetBookRecommendations1() {
    int passedTests = 0;
    BookRecommendation bookRecommendation = setUp();
    std::vector<Book> recommendations2 = bookRecommendation.getBookRecommendations("user1", 2, 2);
    passedTests += assert(recommendations2.size() == 1);
    passedTests += assert(recommendations2[0].ISBN == "0446973483");
    return std::make_pair(passedTests, 2);
}

std::pair<int, int> bookRecommendationTestGetBookRecommendations2() {
    int passedTests = 0;
    BookRecommendation bookRecommendation = setUp();
    std::vector<Book> recommendations = bookRecommendation.getBookRecommendations("user4", 2, 10);
    passedTests += assert(recommendations.size() == 2);
    Book& b1 = recommendations.at(0);
    Book& b2 = recommendations.at(1);
    passedTests += assert(b1.ISBN == "0486411044");
    passedTests += assert(b1.title == "First Fig and Other Poems (Dover Thrift Editions)");
    passedTests += assert(b2.ISBN == "0446973483");
    passedTests += assert(b2.title == "Until the Sun Dies");
    return std::make_pair(passedTests, 5);
}

std::pair<int, int> bookRecommendationTestGetBookRecommendations3() {
    int passedTests = 0;
    BookRecommendation bookRecommendation = setUp();
    std::vector<Book> emptyRecommendations = bookRecommendation.getBookRecommendations("user6", 2, 2);
    passedTests += assert(emptyRecommendations.empty());
    return std::make_pair(passedTests, 1);
}

int bookRecommendationTests() {
    int passedTests = 0;
    int totalTests = 0;
    std::pair<int, int> r1 = bookRecommendationTestCalculateSimilarity();
    passedTests += r1.first;
    totalTests += r1.second;
    std::pair<int, int> r2 = bookRecommendationTestGetNeighborhood();
    passedTests += r2.first;
    totalTests += r2.second;
    std::pair<int, int> r3 = bookRecommendationTestGetRecommendedBooks1();
    passedTests += r3.first;
    totalTests += r3.second;
    std::pair<int, int> r4 = bookRecommendationTestGetRecommendedBooks2();
    passedTests += r4.first;
    totalTests += r4.second;
    std::pair<int, int> r5 = bookRecommendationTestGetBookRecommendations1();
    passedTests += r5.first;
    totalTests += r5.second;
    std::pair<int, int> r6 = bookRecommendationTestGetBookRecommendations2();
    passedTests += r6.first;
    totalTests += r6.second;
    std::pair<int, int> r7 = bookRecommendationTestGetBookRecommendations3();
    passedTests += r7.first;
    totalTests += r7.second;
    double grade = static_cast<double>(passedTests * 100) / totalTests;
    grade = std::round(grade * 10) / 10;
    std::cout << "Total tests passed: " << passedTests << " out of " << totalTests << " (" << grade << "%)"  << std::endl;
    return 0;
}

#endif //BOOKRECOMMENDATIONTESTS_H
