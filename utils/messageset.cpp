#pragma once
#include<iostream>
#include<set>
#include<mutex>
using namespace std;

class MessageSet {
public:
    set<int> container;
    mutex mtx;

    MessageSet() {
        
    }

    void set(int size) {

    }

    bool remove(int messageId) {
        // single thread
        return container.erase(messageId);
    }

    void insert(int messageId) {
        // single thread
        container.insert(messageId);
    }

    bool find(int messageId) {
        // find if it exists, single thread
        return container.find(messageId) != container.end();
    }

    bool checkDuplicate(int messageId) {
        // check if it exists and mark it exists, multi thread
        lock_guard<mutex> lk(mtx);
        if (container.find(messageId) != container.end()) {
            return true;
        }
        container.insert(messageId);
        return false;
        
    }
};