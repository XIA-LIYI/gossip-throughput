#pragma once

#include<iostream>
#include<atomic>
using namespace std;

class ThreadSafeList {
public:
    atomic<int> tail {};
    int* lst;
    int maxSize;
    int numOfRemoved;

    ThreadSafeList() {
    }

    void set(int size) {
        lst = new int[size];
        maxSize = size;
    }

    void push(int element) {
        int currentTail = tail++;
        if (currentTail < maxSize) {
            lst[currentTail] = element;
        } else {
            throw runtime_error(" queue is full");
        }
    }

    bool empty() {
        if (numOfRemoved >= tail) {
            return true;
        }
        return false;
    }

    int pop() {
        int pos = tail--;
        numOfRemoved++;
        return lst[pos - 1];
    }

    void clear() {
        tail = 0;
        numOfRemoved = 0;
    }
};