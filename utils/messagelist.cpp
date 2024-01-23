#pragma once
#include<atomic>

class MessageList {
public:
    atomic<int> head;

    bool* lst;
    int maxSize;

    MessageList() {

    }

    void set(int size) {
        maxSize = size;
        lst = new bool*[size] {};
    }

    bool insert(int messageId) {
        if (lst[messageId]) {
            return false;
        }
        lst[messageId] = true;
        return true;
    }

    bool find(int messageId) {
        return lst[messageId];
    }

}