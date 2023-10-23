#include "../include/BookRecommendation.h"
#include "../include/Stack.h"


UnorderedSet<Book> BookRecommendation::getRecommendedBooks(const UnorderedSet<std::string> &neighborhood,
                                                           const std::string &targetUserID) {
    return UnorderedSet<Book>();
}

UnorderedSet<std::string> BookRecommendation::getNeighborhood(const std::string &targetUserID, int neighborhoodSize) {
    return UnorderedSet<std::string>();
}

double BookRecommendation::calculateSimilarity(const std::string &userID1, const std::string &userID2) {
    unsigned long unionSize = 0, intersectionSize = 0;
    std::vector<Book> user1Vec = std::vector<Book>();
    std::vector<Book> user2Vec = std::vector<Book>();

    for (const auto& book : userBorrowedBooks[userID1]) user1Vec.push_back(book);
    for (const auto& book : userBorrowedBooks[userID2]) user2Vec.push_back(book);

    auto book1 = user1Vec.begin();
    auto book2 = user2Vec.begin();

    // Calculate intersection size.
    while (book1 != user1Vec.end() && book2 != user2Vec.end()) {
        if (*book1 < *book2) {
            book1++;
        }
        else if (*book1 > *book2) {
            book2++;
        }
        else {
            intersectionSize++;
            book1++;
            book2++;
        }
    }

    unionSize = user1Vec.size() + user2Vec.size() - intersectionSize;

    return (double)intersectionSize / (double)unionSize;
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
