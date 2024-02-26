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
    // ThreadSafeList messagesWithFullCount;
    atomic<int> numOfMessageRemoved {};
    atomic<int> numOfMessagesWith95Count {};

    int startRound[numOfMessagesTotal] = {};
    int nintyfiveRound[numOfMessagesTotal] = {};
    float progress[numOfMessagesTotal][messageRecordFrequency] = {};

    MessageBox() {
        // messagesWithFullCount.set(bandwidth * 5, "MESSAGE_BOX");
    }

    int generateNewMessage(int round) {
        // if (messageId >= numOfMessagesTotal) {
        //     return -1;
        // }
        int currNumOfNewMessage = ++numOfNewMessage;
        if (currNumOfNewMessage > newMessageLimit) {
            return -1;
        }
        int res = messageId++;
        startRound[res] = round;
        if (res > numOfMessagesTotal) {
            throw runtime_error("message box is full");
        }
        // messagesCount[res]++;
        return res;
    }

    void refresh(int round) {
        numOfNewMessage = 0;
        // messagesWithFullCount.clear();
        generateData(round);
    }

    void addCount(int messageId, int round) {
        int currMessageCount = ++messagesCount[messageId];
        if (currMessageCount == int((numOfNodes - numOfDeadNodes) * 0.95)) {
            nintyfiveRound[messageId] = round;
            numOfMessagesWith95Count++;
        }

        if (currMessageCount == numOfNodes - numOfDeadNodes) {
            // messagesWithFullCount.push(messageId);
            numOfMessageRemoved++;
        }
    }


    void generateData(int round) {
        for (int i = 0; i < messageId; i++) {
            int roundDiff = round - startRound[i];
            if (roundDiff >= messageRecordFrequency) {
                continue;
            }
            float percentage = float(messagesCount[i]) / float(numOfNodes - numOfDeadNodes);
            progress[i][roundDiff] = percentage;
        }
    }



};