#pragma once

#include<iostream>
#include<atomic>
using namespace std;

class ThreadSafeList {
public:
    atomic<int> size = {};
    vector<int> lst;
    int maxSize;
    int numOfRemoved = 0;


    ThreadSafeList() {
    }

    void set(int size) {
        lst.resize(size);
        maxSize = size;
    }

    void push(int element) {
        int currentTail = size++;
        if (currentTail < maxSize) {
            lst[currentTail] = element;
        } else {
            throw runtime_error(" queue is full");
        }
    }

    bool empty() {
        if (size == 0) {
            return true;
        }
        return false;
    }

    int pop() {
        int pos = size--;
        numOfRemoved++;
        return lst[pos - 1];
    }

    void clear() {
        size = 0;
        numOfRemoved = 0;
    }
};