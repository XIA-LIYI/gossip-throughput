#pragma once
#include<set>
#include<atomic>
#include<list>
#include "utils/threadsafelist.cpp"
#include "parameters.cpp"
using namespace std;

class MessageBox {
public:
    atomic<int> numOfNewMessage {};
    atomic<int> messageId {};
    atomic<int> messagesCount[numOfMessagesTotal] {};
    list<int> messagesWithFullCount;
    atomic<int> numOfMessageRemoved {};

    MessageBox() {
        // messagesWithFullCount.set(128);
    }

    int generateNewMessage() {
        if (messageId >= 10000) {
            return -1;
        }
        numOfNewMessage++;
        if (numOfNewMessage > newMessageLimit) {
            return -1;
        }
        int res = messageId++;
        if (res > numOfMessagesTotal) {
            throw runtime_error("message box is full");
        }
        messagesCount[res]++;
        return res;
    }

    void refresh() {
        numOfNewMessage = 0;
        messagesWithFullCount.clear();
    }

    void addCount(int messageId) {
        messagesCount[messageId]++;
        if (messagesCount[messageId] == numOfNodes) {
            messagesWithFullCount.push_back(messageId);
            numOfMessageRemoved++;
        }
    }



};