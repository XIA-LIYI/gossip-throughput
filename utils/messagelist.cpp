#pragma once
#include<atomic>
#include<iostream>
#include<mutex>
using namespace std;

class MessageList {
public:

    bool* lst;
    int maxSize;
    mutex mtx;

    MessageList() {

    }

    void set(int size) {
        maxSize = size;
        lst = new bool[size] {};
    }

    // bool insert(int messageId) {
    //     lock_guard<mutex> lk(mtx);
    //     if (messageId > maxSize) {
    //         throw runtime_error("Out of capacity");
    //     }
    //     if (lst[messageId]) {
    //         return false;
    //     }
    //     lst[messageId] = true;
    //     return true;
    // }

    void insert(int messageId) {
        lst[messageId] = true;
    }

    bool find(int messageId) {
        return lst[messageId];
    }

    bool checkDuplicate(int messageId) {
        lock_guard<mutex> lk(mtx);
        if (lst[messageId] == true) {
            return true;
        }
        lst[messageId] = true;
        return false;
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