#include "../include/BookRecommendation.h"
#include "../include/Stack.h"


// Returns the set of recommended books for the target patron id.
//
// For this, using a Stack, search the books that are frequently borrowed by users in the neighborhood and
// filter out the books that the target user has already borrowed.
// Then rank the remaining books based on popularity or ratings within the neighborhood
// and return the set of recommended books.
// To implement this function:
// - Create a HashTable to store the frequency of each book borrowed by users in the neighborhood.
// - Iterate through the frequency map and add books that haven't been borrowed by the target patron id
// to the recommendedBooks UnorderedSet.
// return the recommendedBooks UnorderedSet.
UnorderedSet<Book> BookRecommendation::getRecommendedBooks(const UnorderedSet<std::string> &neighborhood,
                                                           const std::string &targetUserID) {
    Stack<Book> bookStack{};
    UnorderedSet<Book> recommendedBooks{};
    bool flag = true;

    for (const auto& users : neighborhood) {
        if (users == targetUserID) continue;
        else {
            for (const auto &books: *userBorrowedBooks.search(users)) {
                bookStack.push(books);
            }
        }
    }

    for (const auto& users : neighborhood) {
        for (const auto &book: *userBorrowedBooks.search(users)) {
            if (!bookBorrowedByUsers.search(book.title)) {
                bookBorrowedByUsers[book.title] = UnorderedSet<Patron>();
            }
        }
    }

    while (!bookStack.isEmpty()) {
        for (const auto& targetBook : userBorrowedBooks[targetUserID]) {
            if (bookStack.top() == targetBook) {
                bookStack.pop();
                flag = true;
                break;
            }
        }
        if (!flag) {
            recommendedBooks.insert(bookStack.top());
            bookStack.pop();
        }
        flag = false;
    }

    return recommendedBooks;
}


UnorderedSet<std::string> BookRecommendation::getNeighborhood(const std::string &targetUserID, int neighborhoodSize) {
    struct userSimilarity {
        std::string user_id;
        double similarity;
    };

    std::vector<std::string> userVec{};
    std::vector<userSimilarity> similarityVec{};
    Stack<std::string> neighborStack{};
    UnorderedSet<std::string> neighborhoodSet{};
    unsigned long similarityVecSize = 0;
    userSimilarity temp;

    for (const auto& pair : userBorrowedBooks) {
        userVec.push_back(pair->key);
    }

    for (const std::string& users : userVec) {
        userSimilarity data;
        if (users == targetUserID) continue;
        else {
            data.user_id = users;
            data.similarity = calculateSimilarity(targetUserID, users);
            similarityVec.push_back(data);
        }
    }

    similarityVecSize = similarityVec.size();
    for (int i = 0; i < similarityVecSize - 1; i++) {
        for (int j = 0; j < similarityVecSize - i - 1; j++) {
            if (similarityVec[j].similarity > similarityVec[j + 1].similarity) {
                temp = similarityVec[j];
                similarityVec[j] = similarityVec[j + 1];
                similarityVec[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < similarityVecSize; i++) {
        if (similarityVec[i].similarity > 0) {
            neighborStack.push(similarityVec[i].user_id);
        }
    }

    while (!neighborStack.isEmpty()) {
        neighborhoodSet.insert(neighborStack.top());
        neighborStack.pop();
        if (neighborhoodSet.size() == neighborhoodSize) break;
    }

    return neighborhoodSet;
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
    UnorderedSet<std::string> neighborhoodSet{};
    UnorderedSet<Book> neighborhoodBookSet {};
    std::vector<Book> recommendations {};


    for (const auto& target : getNeighborhood(targetUserID, neighborhoodSize)) {
        neighborhoodSet.insert(target);
    }

    for (const auto& book : getRecommendedBooks(neighborhoodSet, targetUserID)) {
        neighborhoodBookSet.insert(book);
    }

    for (const auto& book : neighborhoodBookSet) {
        if (recommendations.size() == numRecommendations) break;
        recommendations.push_back(book);
    }

    return recommendations;
}

void BookRecommendation::addUserBorrowedBook(Patron &userID, Book &book) {
    // Check if the user already exists in the hash table
    if (!userBorrowedBooks.search(userID.ID)) {
        userBorrowedBooks[userID.ID] = UnorderedSet<Book>();
    }
    userBorrowedBooks[userID.ID].insert(book);
}
