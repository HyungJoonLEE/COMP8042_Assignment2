/*
 * Copyright (c) Sayyedhassan Shavarani 2023
 * All rights reserved. Unauthorized redistribution is prohibited.
 */
#ifndef BOOKRECOMMENDATION_H
#define BOOKRECOMMENDATION_H
/**
 * Implementation of the book recommendation system main class.
 */
#include <iostream>
#include <vector>
#include "Utils.h"
#include "Stack.h"
#include "UnorderedSet.h"
#include "HashTable.h"

class BookRecommendation {
public:
    void addUserBorrowedBook(Patron& userID, Book& book);
    std::vector<Book> getBookRecommendations(const std::string& targetUserID, int numRecommendations, int neighborhoodSize);
    double calculateSimilarity(const std::string& userID1, const std::string& userID2);
    UnorderedSet<std::string> getNeighborhood(const std::string& targetUserID, int neighborhoodSize);
    UnorderedSet<Book> getRecommendedBooks(const UnorderedSet<std::string>& neighborhood, const std::string& targetUserID);

private:
    HashTable<std::string, UnorderedSet<Book>> userBorrowedBooks;
    HashTable<std::string, UnorderedSet<Patron>> bookBorrowedByUsers;
};

#endif //BOOKRECOMMENDATION_H
