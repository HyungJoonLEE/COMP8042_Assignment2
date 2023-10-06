#include "../include/Stack.h"


/**
 * Constructs an empty stack.
 *
 * Initializes an empty stack.
 */
template<typename T>
Stack<T>::Stack() {
    buffer = std::vector<T>();
    currentSize = 0;
}


/**
 * Checks if the stack is empty.
 *
 * @return True if the stack is empty, false if it is not empty.
 */
template<typename T>
bool Stack<T>::isEmpty() const {
    return currentSize == 0;
}


/**
 * Returns the number of elements in the stack.
 *
 * @return The number of elements currently in the stack.
 */
template<typename T>
size_t Stack<T>::size() const {
    return currentSize;
}


/**
 * Pushes an element onto the stack.
 *
 * Adds the element to the top of the stack.
 *
 * @param element The element to be added to the stack.
 */
template<typename T>
void Stack<T>::push(const T &element) {
    buffer.push_back(element);
    currentSize++;
}


/**
 * Removes the top element from the stack.
 *
 * Removes the element at the top of the stack, reducing the stack's size by one.
 * If the stack is empty, no elements will be popped and throw the exception.
 */
template<typename T>
void Stack<T>::pop() {
    if (!isEmpty()) {
        buffer.pop_back();
        currentSize--;
    }
    else throw std::out_of_range("Stack is empty");
}


/**
 * Returns a reference to the top element of the stack.
 *
 * Retrieves a reference to the element at the top of the stack without removing it.
 * If the stack is empty, no elements will be returned and throw the exception.
 *
 * @return A reference to the top element of the stack.
 */
template<typename T>
T &Stack<T>::top() {
    if (!isEmpty())
        return buffer.back();
    else throw std::out_of_range("Stack is empty");
}


/**
 * Returns a constant reference to the top element of the stack.
 *
 * Retrieves a constant reference to the element at the top of the stack without removing it.
 * If the stack is empty, no elements will be returned and throw the exception.
 *
 * @return A constant reference to the top element of the stack.
 */
template<typename T>
const T &Stack<T>::top() const {
    if (!isEmpty())
        return buffer.back();
    else throw std::out_of_range("Stack is empty");
}

