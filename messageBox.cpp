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
    atomic<int> messagesCount[numOfMessagesTotal] = {};
    // list<int> messagesWithFullCount;
    ThreadSafeList messagesWithFullCount;
    atomic<int> numOfMessageRemoved {};

    int startRound[numOfMessagesTotal] = {};
    int nintyfiveRound[numOfMessagesTotal] = {};

    MessageBox() {
        messagesWithFullCount.set(bandwidth * 100);
    }

    int generateNewMessage(int round) {
        if (messageId >= numOfMessagesTotal) {
            return -1;
        }
        numOfNewMessage++;
        if (numOfNewMessage > newMessageLimit) {
            return -1;
        }
        int res = messageId++;
        startRound[res] = round;
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

    void addCount(int messageId, int round) {
        messagesCount[messageId]++;
        if (messagesCount[messageId] == int((numOfNodes - numOfDeadNodes) * 0.95)) {
            nintyfiveRound[messageId] = round;
        }

        if (messagesCount[messageId] == numOfNodes - numOfDeadNodes) {
            messagesWithFullCount.push(messageId);
            numOfMessageRemoved++;
        }
    }



};