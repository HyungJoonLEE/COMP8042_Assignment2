Assignment 2 - Book Recommendation using Collaborative Filtering
================================================================
---

Collaborative filtering is a common technique used in recommendation systems to suggest items to users based on the
preferences and behaviors of similar users. In this assignment, we plan to use collaborative filtering to recommend 
books to users based on their borrowing history. To accomplish this task, you will need to implement an `UnorderedSet`, 
and a `HashTable` as the required datastructures (you will also use the `Stack` module which you completed in the 
previous assignment). Browsing `include` directory, you can find the initially necessary datastructures in `Utils.h`. 
As well, you can find the ADT designs of the two required datastructures in `UnorderedSet.h`, and `HashTable.h`. 
You need to implement the specified functions of each header file in a .cpp file with the exact same name (i.e. 
`UnorderedSet.cpp`, and `HashTable.cpp`) inside `src/` directory. Please do not modify the header files as you will not 
be submitting those files at the end.

Step-by-step guide:
-------------------
* **Important note**: for each module, after the implementation make sure its provided test cases run flawlessly.
  * running the test cases will give you the grade that you will get for that module. 
  * You are **not allowed** to use any STL data structures in your final submission except for `vector` and `list`.
  * You are allowed to use the `algorithm` library in the `.cpp` files you implement.

1. Copy your completed `Stack` implementation from assignment 1 into `src` directory.
2. After the _Trees_ lecture, get started on implementation of the `UnorderedSet`.
   Once done, test it using `tests/UnorderedSetTests.h`.
   You need to implement a **balanced Red-Black Tree** as the underlying structure for the `UnorderedSet`.
   Section 12.2 of the textbook covers this data structure in detail.
3. After the _Hashing_ lecture, complete the implementation of the `HashTable`.
   Once done, test it using `tests/HashTableTests.h`.
   You need to implement a **Hopscotch HashTable** as the underlying structure for the `HashTable`.
   Section 5.7.3 of the textbook covers this data structure in detail.   
4. Using the implemented modules implement the `BookRecommendation` class which can be used in the following scenario:

- Call `addUserBorrowedBook` a number of times to store all the borrowed book (loaded in the `Book` data 
  structure), user (loaded in `Patron` data structure) pairs.
- Next, call `getBookRecommendations` with a target `patron` id and the maximum number of required recommendations
  will return the recommendation results for the target `patron` id given their borrowing history similarities with
  others. Here are the methods you will need to implement `getBookRecommendations`: 
  - `calculateSimilarity`: receives two `patron` ids and returns a score (`double`) between 0 and 1 indicating the 
    similarity of the two users. The score is essentially going to be the Jaccard similarity between the set of 
    borrowed books for each user.
  - `getNeighborhood`: returns the `neighborhoodSize` number of most similar `patron` ids. 
    To implement this function:
    - Get the set of all `patron` ids (excluding the target `patron` ids) to calculate similarity.
    - Use a `Stack` to keep track of the most similar users (neighborhood) and calculate/store the similarity scores of 
      all users with the target user.
    - Push the users with the highest similarity onto the `Stack` (top K users).
    - Convert the `Stack` to an `UnorderedSet` to return the neighborhood.
  - `getRecommendedBooks`: returns the set of recommended books for the target `patron` id.
    For this, using a `Stack`, search the books that are frequently borrowed by users in the neighborhood and 
    filter out the books that the target user has already borrowed. Then rank the remaining books based on popularity 
    or ratings within the neighborhood and return the set of recommended books.
    To implement this function:
    - Create a `HashTable` to store the frequency of each book borrowed by users in the neighborhood.
    - Iterate through the frequency map and add books that haven't been borrowed by the target `patron` id to the 
      recommendedBooks `UnorderedSet`.
    - return the recommendedBooks `UnorderedSet`.
- Use `tests/BookRecommendationTests.h` for testing your implementation.

## Submission and Grading:

As stated earlier, you are not supposed to change the content of the files inside `docs`, `include` and `tests` folders.
Therefore, zip out your `src` folder which is expected to contain `UnorderedSet.cpp`, `HashTable.cpp`, and
`BookRecommendation.cpp` and submit it. Your submitted `src` files must be able to be compiled and run with the provided
`CMakeLists.txt` file. If you compile and run the `main.cpp` file, you must be able to see the grade which will be 
submitted to https://learn.bcit.ca for this assignment, if you have done everything properly, you will see the following
output:
```
>> UnorderedSet:		Total tests passed: 68 out of 68 (100%)
>> HashTable:			Total tests passed: 52 out of 52 (100%)
>> Book Recommender System: 	Total tests passed: 20 out of 20 (100%)
```

### Rubric:
As the feedback you will receive an output similar to the one posted above, with a difference; if your tests fail or the
submission crashes you will also receive the error message, so you can follow up on the error to improve your 
understanding of this assignment. Here is the breakdown of the grades for this assignment:

* UnorderedSet: 25 marks
* HashTable: 20 marks
* Book Recommender System: 25 marks
* Documentation inside the code for public functions: 5 marks
* Submission of extra files in addition to requested (`UnorderedSet.cpp`, `HashTable.cpp`, `BookRecommendation.cpp`) penalty: -4 marks

Your final grade for assignment 2 will therefore be calculated from 75 marks.

### Important Notes Regarding Your Submission:
* I will be compiling/testing your code on a Linux OS, so please do not put in any Windows-specific code in your submission.
  I want to emphasize on this point specially since some of you use VSCode in Windows, and it tends to have its own
  default settings which in the past has led to confusion and grade deductions.
  * Please **do make sure** your code
    compiles and runs without any specific dependency on Windows.
  * Please note, it does not matter if the code runs on your
    machine! it must run on my machine. I hope this reduces the need for me to recite this in a few future correspondences!
* Your submission has a maximum allowed time to run! if it does not finish running in 2 minutes, it is considered timed
  out and will receive an automatic grade of 0.
* Please make sure you submit the final source files and download and check your files after the final submission.
  I have had students who accidentally had forgotten to check the files, and it had turned out that they had submitted
  the original assignment files instead of the completed assignment. Please do check this, as once the grading is done
  and the assignment is graded 0, nothing can be done (if such a thing happened please do not email me, as I will give
  you the exact same answer as is written here).
* As mentioned earlier, I will throw away any files submitted in addition to the requested three `.cpp` files (and you
  will receive 4 marks penalty if anything extra is submitted), so please make sure you do not rely on changing the
  content of any file in `include` and `tests` directories as they will not be present in my side and your code will be
  broken and will receive a 0 for compilation errors.

Good luck and happy programming!
