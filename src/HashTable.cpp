#include "../include/HashTable.h"


/**
 * @brief Checks if a number is prime.
 *
 * This function determines if a given integer is a prime number or not. A prime number
 * is a number greater than 1 that has no positive divisors other than 1 and itself.
 *
 * @param num The integer number to check for primality.
 * @return Returns 'true' if the number is prime, 'false' otherwise.
 */
inline bool isPrime(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) return false;
    }
    return true;
}


/**
 * @brief Finds the largest prime number below a given integer.
 *
 * This function determines the largest prime number that is strictly less than the provided
 * integer 'n'. If no prime number is found below the provided integer, it returns -1.
 *
 * @param n The integer threshold below which to find the largest prime.
 * @return Returns the largest prime number less than 'n' if it exists, or -1 if no such prime is found.
 */
inline int largestPrimeBelow(int n) {
    for (int i = n - 1; i >= 2; i--) {
        if (isPrime(i)) {
            return i;
        }
    }
    return -1;
}


/**
 * @brief Computes a hash index for a given key in a hash table.
 *
 * This function calculates a hash index for an integer key to be used in a hash table of a specified size.
 * It uses a prime-based multiplication approach, taking advantage of the properties of prime numbers to
 * distribute keys more uniformly.
 *
 * @param key The integer key to be hashed.
 * @param tableSize The size of the hash table.
 * @return Returns an index in the range [0, tableSize-1] to which the key will be mapped in the hash table.
 */
inline unsigned int hashKey(const int &key, int tableSize) {
    unsigned int index = 0;
    unsigned int prime = largestPrimeBelow(tableSize);

    index = index * prime;
    return index % tableSize;
}


/**
 * @brief Computes a hash index for a given string key in a hash table.
 *
 * This function calculates a hash index for a string key to be used in a hash table of a specified size.
 * It employs a prime-based multiplication and addition approach for hashing, leveraging the properties
 * of prime numbers to disperse keys more uniformly across the hash table.
 *
 * @param key The string key to be hashed.
 * @param tableSize The size of the hash table.
 * @return Returns an index in the range [0, tableSize-1] to which the string key will be mapped in the hash table.
 */
inline unsigned int hashKey(const std::string &key, int tableSize) {
    unsigned int index = 0;
    unsigned int prime = largestPrimeBelow(tableSize);

    for (char c : key) {
        index = index + c * prime;
    }

    return index % tableSize;
}

/**
 * @brief Constructs a new HashTable object with a specified size and load factor threshold.
 *
 * This constructor initializes the HashTable with an empty table of the specified size and sets
 * the load factor threshold for resizing. Each entry in the table is represented as a 'Bucket'.
 *
 * @param size The initial size of the hash table.
 * @param threshold The load factor threshold used for determining when to resize the table.
 *        Load factor is calculated as (number of entries / table size). When the current load
 *        factor exceeds this threshold, it may be a good idea to consider resizing the table
 *        to maintain efficient performance.
 */
template<typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable(unsigned int size, double threshold) {
    this->tableSize = size;
    this->loadFactorThreshold = threshold;
    hashTable = std::vector<Bucket>(tableSize);
}


/**
 * @brief Returns an iterator pointing to the first occupied bucket in the HashTable.
 *
 * This function iterates over the buckets in the hash table and returns an iterator
 * to the first bucket that is occupied. If no occupied buckets are found, it returns
 * an iterator pointing to the 'end' of the hash table.
 *
 * @return An Iterator pointing to the first occupied bucket in the HashTable, or the 'end'
 *         iterator if the HashTable is empty or if no occupied buckets are found.
 */
template<typename KeyType, typename ValueType>
typename HashTable<KeyType, ValueType>::Iterator HashTable<KeyType, ValueType>::begin() {
    for (auto it = hashTable.begin(); it != hashTable.end(); ++it) {
        if (it->occupied) {
            return Iterator(it, hashTable.end());
        }
    }
    return end();
}


/**
 * @brief Returns an iterator pointing just past the end of the HashTable.
 *
 * This function provides a standard way to acquire an iterator that represents
 * the end of the hash table. It doesn't point to any valid bucket but serves as
 * a marker to indicate that there are no more occupied buckets to iterate over.
 *
 * @return An Iterator pointing just past the last bucket in the HashTable.
 */
template<typename KeyType, typename ValueType>
typename HashTable<KeyType, ValueType>::Iterator HashTable<KeyType, ValueType>::end() {
    return Iterator(hashTable.end(), hashTable.end());
}


/**
 * @brief Provides access to the value associated with the given key.
 *
 * This overload of the subscript operator allows for easy access or assignment
 * to values in the hash table based on a given key. If the key does not exist
 * in the hash table, a default-constructed instance of the ValueType is inserted
 * with the given key, and then its reference is returned.
 *
 * @param key The key for which the associated value needs to be accessed or set.
 * @return Reference to the value associated with the specified key.
 *
 * Note:
 * If the key doesn't exist in the hash table and you access it using this operator,
 * it will add the key with a default-constructed value. Always be cautious when
 * using this operator as it might inadvertently increase the size of the hash table.
 */
template<typename KeyType, typename ValueType>
ValueType &HashTable<KeyType, ValueType>::operator[](const KeyType &key) {
    ValueType* value = search(key);
    if (!value) {
        insert(key, ValueType());
        value = search(key);
    }
    return *value;
}


/**
 * @brief Updates the value for a given key or inserts a new key-value pair if the key does not exist.
 *
 * This function is designed to provide a mechanism to either update an existing value associated
 * with a given key or to insert a new key-value pair into the hash table if the key does not
 * already exist.
 *
 * @param key The key for which the associated value needs to be updated or set.
 * @param newValue The new value to be associated with the specified key.
 *
 * Note:
 * The function does a search operation first to check if the key exists. If the key is found,
 * it updates the value. If the key is not found, it performs an insert operation. Therefore,
 * depending on the scenario, it might be slightly more efficient than separately using the
 * search and insert functions, especially in cases where updates are frequent.
 */
template<typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::updateValueForKey(const KeyType &key, ValueType newValue) {
    ValueType* value = search(key);
    if (value) {
        *value = newValue;
    } else {
        insert(key, newValue);
    }
}


/**
 * @brief Inserts a key-value pair into the hash table or updates the value if the key already exists.
 *
 * This function is designed to insert a new key-value pair into the hash table using hop-scotch hashing.
 * If the current load factor of the hash table exceeds the specified threshold, the hash table is rehashed
 * to ensure efficient lookup and insertion operations.
 *
 * @param key The key to be inserted into the hash table.
 * @param value The value associated with the specified key.
 *
 * Note:
 * The function utilizes hop-scotch hashing, which attempts to insert the key-value pair within a specified
 * 'hop range' from the hash index of the key. If there's no free bucket within the hop range, the function
 * looks for free slots further away from the hash index.
 */
template<typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::insert(const KeyType &key, const ValueType &value) {
    unsigned int index = 0, offset = 0, hop = 0, i = 0;

    if (loadFactor() >= loadFactorThreshold) {
        rehash();
    }

    index = hashKey(key, tableSize);
    offset = 0;
    hop = 1;
    while (offset < hopRange) {
        if (!hashTable[(index + offset) % tableSize].occupied) {
            auto& bucket = hashTable[(index + offset) % tableSize];
            bucket.key = key;
            bucket.value = value;
            bucket.occupied = true;
            bucket.hopInfo = hop;
            return;
        }
        offset++;
        hop <<= 1;
    }

    // TODO: No free bucket within hop range
    //          need to move items
    while (true) {
        i = findFreeSlot(hashTable, 0, hop);
        if (!hashTable[i].occupied) {
            hashTable[i].key = key;
            hashTable[i].value = value;
            hashTable[i].occupied = true;
            hashTable[i].hopInfo = hop; // Hop info will be all zeros as it's beyond the hop range
            return;
        }
    }
}


/**
 * @brief Searches for the value associated with the specified key in the hash table.
 *
 * This function attempts to locate the value associated with the given key in the hash table
 * using linear probing. If the key is found, a pointer to the value is returned. Otherwise,
 * it returns a nullptr indicating that the key does not exist in the table.
 *
 * @param key The key whose associated value needs to be searched for.
 * @return ValueType* Pointer to the value associated with the key. If the key is not found, returns nullptr.
 */
template<typename KeyType, typename ValueType>
ValueType *HashTable<KeyType, ValueType>::search(const KeyType &key) {
    unsigned int index = hashKey(key, tableSize);
    while (hashTable[index].occupied) {
        if (hashTable[index].key == key) {
            return &hashTable[index].value;
        }
        index = (index + 1) % tableSize;  // Linear probing
    }
    return nullptr;
}


/**
 * @brief Removes the key-value pair associated with the specified key from the hash table.
 *
 * This function attempts to remove the key-value pair associated with the given key
 * from the hash table using linear probing. If the key is found and successfully removed,
 * (The removing use Lazy Deletion, set occupied to false)
 * the function returns true. If the key does not exist in the table, it returns false.
 *
 * @param key The key whose associated key-value pair needs to be removed.
 * @return bool True if the key-value pair was successfully removed. False if the key was not found.
 */
template<typename KeyType, typename ValueType>
bool HashTable<KeyType, ValueType>::remove(const KeyType &key) {
    unsigned int index = hashKey(key, tableSize);
    while (hashTable[index].occupied) {
        if (hashTable[index].key == key) {
            hashTable[index].occupied = false;
            return true;
        }
        index = (index + 1) % tableSize;  // Linear probing
    }
    return false;
}


/**
 * @brief Clears the hash table by marking all the buckets as unoccupied(occupied = false).
 *
 * This function goes through each bucket in the hash table and sets its "occupied" status to false,
 * effectively removing all the key-value pairs from the table. The actual data within each bucket remains,
 * but since they are marked unoccupied, they will not be taken into account during operations like search or insert.
 */
template<typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::clear() {
    for (auto& bucket : hashTable) {
        bucket.occupied = false;
    }
}


/**
 * @brief Returns the number of occupied buckets (key-value pairs) in the hash table.
 *
 * The function iterates over each bucket in the hash table and counts how many of them are marked as occupied.
 *
 * @return unsigned int representing the number of key-value pairs currently present in the hash table.
 */
template<typename KeyType, typename ValueType>
unsigned int HashTable<KeyType, ValueType>::size() const {
    unsigned int count = 0;
    for (const auto& bucket : hashTable) {
        if (bucket.occupied) {
            ++count;
        }
    }
    return count;
}


/**
 * @brief Calculates and returns the current load factor of the hash table.
 *
 * The load factor is defined as the ratio of the number of occupied buckets to
 * the total number of buckets (capacity) in the hash table. A higher load factor
 * indicates that the table is becoming full and may soon need to be resized.
 *
 * @return The current load factor of the hash table, which is a double value between 0.0 (empty) and 1.0 (full).
 */
template<typename KeyType, typename ValueType>
double HashTable<KeyType, ValueType>::loadFactor() const {
    return static_cast<double>(size()) / tableSize;
}


/**
 * @brief Finds an unoccupied slot in the hash table, starting from a specified index and considering a given hop range.
 *
 * This function find a free slot within the hash table to insert a new key-value pair.
 * The method iterates through the hash table starting from the 'startIndex' and uses the 'currentHop'
 * value to determine the offset from the starting position. As the function progresses, it updates
 * the 'currentHop' value until it finds an empty slot or reaches the end of the table.
 *
 * @param cTable Reference to the hash table vector (comprising of buckets).
 * @param startIndex The initial index in the table from where the search begins.
 * @param currentHop Reference to the current hop range, which indicates the offset from the starting position.
 *                   The function will update this value while searching for a free slot.
 *
 * @return The index of the first unoccupied slot found in the hash table based on the given criteria.
 */
template<typename KeyType, typename ValueType>
unsigned int HashTable<KeyType, ValueType>::findFreeSlot(std::vector<Bucket> &cTable, unsigned int startIndex,
                                                         unsigned int &currentHop) {
    unsigned int index = (startIndex + currentHop) % tableSize;
    while (cTable[index].occupied) {
        ++currentHop;
        index = (startIndex + currentHop) % tableSize;
    }
    return index;
}


/**
 * @brief Rehashes the hash table to accommodate a larger number of elements.
 *
 * The function enlarges the hash table's size by doubling its current size. After increasing the size,
 * it then re-inserts all the existing key-value pairs from the old hash table into the new, larger hash table.
 * The rehashing process helps ensure that the hash table maintains an efficient load factor
 * and minimizes the chances of collision during insertions.
 *
 * @note:
 * After rehashing, the overall structure of the hash table may change, as the key-value pairs
 * will be inserted based on the new table size.
 */
template<typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::rehash() {
    auto oldTable = hashTable;
    tableSize *= 2;
    hashTable.clear();
    hashTable.resize(tableSize);

    for (const auto& bucket : oldTable) {
        if (bucket.occupied) {
            insert(bucket.key, bucket.value);
        }
    }
}