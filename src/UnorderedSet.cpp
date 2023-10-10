#include "../include/UnorderedSet.h"
#include "../include/Stack.h"


template<typename Key>
UnorderedSet::Iterator UnorderedSet<Key>::begin() const {
    return UnorderedSet::Iterator(nullptr);
}


