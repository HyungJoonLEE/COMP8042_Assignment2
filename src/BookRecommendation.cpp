#include "../include/BookRecommendation.h"
#include "../include/Stack.h"


/**
 * @brief Generates a set of recommended books for a target user.
 *
 * This function provides a set of book recommendations based on the books borrowed
 * by a neighborhood of users. The recommendations are made by considering books
 * borrowed by the neighborhood users but not borrowed by the target user.
 *
 * 1. All books borrowed by the neighborhood (excluding the target user) are pushed onto a stack.
 * 2. An initialization process ensures every book borrowed by users in the neighborhood is
 *    represented in the bookBorrowedByUsers hashmap.
 * 3. Each book from the stack is compared against the books borrowed by the target user.
 *    (If a match is not found, the book is added to the recommended books set.)
 *
 * @param neighborhood An UnorderedSet of user IDs representing the neighborhood users.
 * @param targetUserID A string representing the ID of the target user for whom the
 *                     recommendations are to be generated.
 * @return An UnorderedSet of Book objects representing the recommended books for the target user.
 */
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


/**
 * @brief Identifies a set of users that are most similar to a target user.
 *
 * This function determines a neighborhood of users who have the most similar borrowing patterns
 * to a given target user. The similarity is determined based on a calculated similarity score.
 *
 * 1. Gather all user IDs.
 * 2. Calculate the similarity score between each user and the target user.
 * 3. Sort the users based on their similarity scores in ascending order.
 * 4. Populate the neighborhood set from the sorted list, prioritizing users with higher similarity scores.
 * 5. Return the neighborhood once the desired neighborhood size is achieved or all users are considered.
 *
 * @param targetUserID A string representing the ID of the target user for whom the neighborhood is to be found.
 * @param neighborhoodSize An integer representing the desired size of the neighborhood.
 * @return An UnorderedSet of strings containing user IDs that represent the neighborhood for the target user.
 */
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


/**
 * @brief Calculates the Jaccard similarity between two users based on their borrowed books.
 *
 * This function determines the similarity score between two users by using the Jaccard similarity
 * coefficient. It is determined by the ratio of the intersection of books borrowed by both users
 * to the union of books borrowed by either user.
 *
 * 1. Fetch the books borrowed by both users.
 * 2. Calculate the intersection size - number of books borrowed by both users.
 * 3. Calculate the union size - total number of unique books borrowed by either user.
 * 4. Return the ratio of intersection size to union size as the similarity score.
 *
 * @param userID1 A string representing the ID of the first user.
 * @param userID2 A string representing the ID of the second user.
 * @return A double representing the Jaccard similarity score between the two users.
 * The value is in the range [0, 1], with 0 indicating no similarity and 1 indicating full similarity.
 */
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


/**
 * @brief Retrieves a list of book recommendations for a target user.
 *
 * This function provides book recommendations for a given user based on the borrowing habits of their neighbors.
 * The neighbors are users who have a high Jaccard similarity score with the target user.
 *
 * 1. Determine the 'neighborhoodSize' most similar users to the target user.
 * 2. Retrieve books that are popular among these neighbors but not borrowed by the target user.
 * 3. Return the top 'numRecommendations' books from this list.
 *
 * @param targetUserID A string representing the ID of the target user.
 * @param numRecommendations An integer specifying the number of book recommendations to be returned.
 * @param neighborhoodSize An integer indicating the number of similar users to consider when generating recommendations.
 * @return A vector containing the list of recommended books for the target user.
 */
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


/**
 * @brief Adds a borrowed book record for a specific user.
 *
 * This function registers a book as borrowed by a particular user. If the user hasn't borrowed any books previously
 * (i.e., the user doesn't exist in the hash table), a new entry for the user is created.
 *
 * 1. Checks if the user already has an entry in the userBorrowedBooks hash table.
 * 2. If not, initializes a new UnorderedSet for the user.
 * 3. Inserts the borrowed book to the user's record.
 *
 * @param userID Reference to a Patron object representing the user who borrowed the book.
 * @param book Reference to a Book object representing the borrowed book.
 */
void BookRecommendation::addUserBorrowedBook(Patron &userID, Book &book) {
    // Check if the user already exists in the hash table
    if (!userBorrowedBooks.search(userID.ID)) {
        userBorrowedBooks[userID.ID] = UnorderedSet<Book>();
    }
    userBorrowedBooks[userID.ID].insert(book);
}
