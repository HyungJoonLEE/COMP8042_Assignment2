#include "../include/HashTable.h"


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
    unsigned int index, offset, hop;

    if (loadFactor() >= loadFactorThreshold) {
        rehash();
    }

    index = std::hash<KeyType>{}(key) % tableSize;
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
    unsigned int i = index + hopRange - 1;
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
    unsigned int index = std::hash<KeyType>{}(key) % tableSize;
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
    unsigned int index = std::hash<KeyType>{}(key) % tableSize;
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