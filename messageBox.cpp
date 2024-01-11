#pragma once
#include<set>
#include<atomic>
#include "utils/threadsafelist.cpp"
#include "parameters.cpp"
using namespace std;

class MessageBox {
public:
    atomic<int> numOfNewMessage {};
    atomic<int> messageId {};
    atomic<int> messagesCount[10000] {};
    ThreadSafeList messagesAllReceived;

    MessageBox() {
        messagesAllReceived.set(128);
    }

    int generateNewMessage() {
        numOfNewMessage++;
        if (numOfNewMessage > newMessageLimit) {
            return -1;
        }
        int res = messageId++;
        return res;
    }

    void clearRestriction() {
        numOfNewMessage = 0;
    }

    void addCount(int messageId) {
        messagesCount[messageId]++;
        if (messagesCount[messageId] == numOfNodes) {
            messagesAllReceived.push(messageId);
        }
    }


};