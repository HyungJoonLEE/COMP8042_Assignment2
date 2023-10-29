#include "../include/HashTable.h"


inline bool isPrime(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false; // All even numbers except 2 are not prime
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) return false;
    }
    return true;
}


inline int largestPrimeBelow(int n) {
    for (int i = n - 1; i >= 2; i--) {
        if (isPrime(i)) {
            return i;
        }
    }
    return -1;
}


inline unsigned int hashKey(const int &key, int tableSize) {
    unsigned int index = 0;
    unsigned int prime = largestPrimeBelow(tableSize);

    index = index * prime;
    return index % tableSize;
}


inline unsigned int hashKey(const std::string &key, int tableSize) {
    unsigned int index = 0;
    unsigned int prime = largestPrimeBelow(tableSize);

    for (char c : key) {
        index = index + c * prime;
    }

    return index % tableSize;
}


template<typename KeyType, typename ValueType>
HashTable<KeyType, ValueType>::HashTable(unsigned int size, double threshold) {
    this->tableSize = size;
    this->loadFactorThreshold = threshold;
    hashTable = std::vector<Bucket>(tableSize);
}


template<typename KeyType, typename ValueType>
typename HashTable<KeyType, ValueType>::Iterator HashTable<KeyType, ValueType>::begin() {
    for (auto it = hashTable.begin(); it != hashTable.end(); ++it) {
        if (it->occupied) {
            return Iterator(it, hashTable.end());
        }
    }
    return end();
}


template<typename KeyType, typename ValueType>
typename HashTable<KeyType, ValueType>::Iterator HashTable<KeyType, ValueType>::end() {
    return Iterator(hashTable.end(), hashTable.end());
}


template<typename KeyType, typename ValueType>
ValueType &HashTable<KeyType, ValueType>::operator[](const KeyType &key) {
    ValueType* value = search(key);
    if (!value) {
        insert(key, ValueType());
        value = search(key);
    }
    return *value;
}


template<typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::updateValueForKey(const KeyType &key, ValueType newValue) {
    ValueType* value = search(key);
    if (value) {
        *value = newValue;
    } else {
        insert(key, newValue);
    }
}


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

    // No free bucket within hop range, need to move items around
    // This can be enhanced with more advanced strategies
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


template<typename KeyType, typename ValueType>
void HashTable<KeyType, ValueType>::clear() {
    for (auto& bucket : hashTable) {
        bucket.occupied = false;
    }
}


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


template<typename KeyType, typename ValueType>
double HashTable<KeyType, ValueType>::loadFactor() const {
    return static_cast<double>(size()) / tableSize;
}


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