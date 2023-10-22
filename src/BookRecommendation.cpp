#include "../include/BookRecommendation.h"


UnorderedSet<Book> BookRecommendation::getRecommendedBooks(const UnorderedSet<std::string> &neighborhood,
                                                           const std::string &targetUserID) {
    return UnorderedSet<Book>();
}

UnorderedSet<std::string> BookRecommendation::getNeighborhood(const std::string &targetUserID, int neighborhoodSize) {
    return UnorderedSet<std::string>();
}

double BookRecommendation::calculateSimilarity(const std::string &userID1, const std::string &userID2) {
    return 0;
}

std::vector<Book> BookRecommendation::getBookRecommendations(const std::string &targetUserID, int numRecommendations,
                                                             int neighborhoodSize) {
    return std::vector<Book>();
}

void BookRecommendation::addUserBorrowedBook(Patron &userID, Book &book) {
    // Check if the user already exists in the hash table
    if (!userBorrowedBooks.search(userID.ID)) {
        userBorrowedBooks[userID.ID] = UnorderedSet<Book>();
    }
    userBorrowedBooks[userID.ID].insert(book);
}
