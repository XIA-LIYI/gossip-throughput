#pragma once
#include<atomic>
#include<iostream>
using namespace std;

class MessageList {
public:
    atomic<int> head;

    bool* lst;
    int maxSize;

    MessageList() {

    }

    void set(int size) {
        maxSize = size;
        lst = new bool[size];
    }

    bool insert(int messageId) {
        if (messageId > maxSize) {
            throw runtime_error("Out of capacity");
        }
        if (lst[messageId]) {
            return false;
        }
        lst[messageId] = true;
        return true;
    }

    bool find(int messageId) {
        return lst[messageId];
    }

};

// int main() {
//     MessageList l;
//     l.set(1000);
//     l.insert(0);
//     if (l.find(0)) {
//         cout << 1;
//     }
// }