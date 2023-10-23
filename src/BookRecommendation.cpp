#include "../include/BookRecommendation.h"
#include "../include/Stack.h"


UnorderedSet<Book> BookRecommendation::getRecommendedBooks(const UnorderedSet<std::string> &neighborhood,
                                                           const std::string &targetUserID) {
    return UnorderedSet<Book>();
}

UnorderedSet<std::string> BookRecommendation::getNeighborhood(const std::string &targetUserID, int neighborhoodSize) {
    struct user_sim {
        std::string user_id;
        double similarity;
    };

    std::vector<std::string> userVec = std::vector<std::string>();
    std::vector<user_sim> similarityVec = std::vector<user_sim>();
    Stack<std::string> neighborStack = Stack<std::string>();
    UnorderedSet<std::string> neighborhoodSet = UnorderedSet<std::string>();
    unsigned long similarityVecSize = 0;
    user_sim temp;

    for (const auto& pair : userBorrowedBooks) {
        userVec.push_back(pair->key);
    }

    for (const std::string& users : userVec) {
        user_sim data;
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
    return std::vector<Book>();
}

void BookRecommendation::addUserBorrowedBook(Patron &userID, Book &book) {
    // Check if the user already exists in the hash table
    if (!userBorrowedBooks.search(userID.ID)) {
        userBorrowedBooks[userID.ID] = UnorderedSet<Book>();
    }
    userBorrowedBooks[userID.ID].insert(book);
}
