#pragma once
#include<set>
#include<atomic>
#include<list>
#include "utils/threadsafelist.cpp"
#include "parameters.cpp"
#include<random>
#include "./node.cpp"
using namespace std;

class MessageBox {
public:
    atomic<int> numOfNewMessage {};
    atomic<int> messageId {};
    atomic<int>* messagesCount = new atomic<int>[numOfMessagesTotal]{};
    // list<int> messagesWithFullCount;
    // ThreadSafeList messagesWithFullCount;
    atomic<int> numOfMessageRemoved {};
    atomic<int> numOfMessagesWith95Count {};

    int* startRound = new int[numOfMessageRecord]{};
    int* nintyfiveRound= new int[numOfMessageRecord]{};
    float** progress = new float*[numOfMessageRecord]{};
    int newMessageRate[usefulRound];


    MessageBox() {
        // messagesWithFullCount.set(bandwidth * 5, "MESSAGE_BOX");
        for (int i = 0; i < numOfMessageRecord; i++) {
            progress[i] = new float[messageRecordFrequency] {};
        }
    }

    int generateNewMessage(int round) {
        // if (messageId >= numOfMessagesTotal) {
        //     return -1;
        // }
        if (round > usefulRound) {
            return -1;
        }
        int currNumOfNewMessage = ++numOfNewMessage;
        // if (round < 100) {
        //     if (currNumOfNewMessage > newMessageLimit) {
        //         return -1;
        //     }
        // }

        
        int res = messageId++;
        if (res % messageRecordGap == 0) {
            startRound[res / messageRecordGap] = round;
        }
        
        if (res > numOfMessagesTotal) {
            throw runtime_error("message box is full");
        }
        // messagesCount[res]++;
        return res;
    }

    void refresh(int round) {
        if (round >= usefulRound) {
            return;
        }
        newMessageRate[round] = numOfNewMessage;
        numOfNewMessage = 0;
        // messagesWithFullCount.clear();
        generateData(round);
        
    }

    void addCount(int id, int round) {
        int currMessageCount = ++messagesCount[id];     
        if (currMessageCount == int((numOfNodes - numOfDeadNodes) * 0.95)) {
            numOfMessagesWith95Count++;
            if (id % messageRecordGap == 0) {
                nintyfiveRound[id / messageRecordGap] = round;
                
            }
        }
        if (currMessageCount == numOfNodes - numOfDeadNodes) {
            // messagesWithFullCount.push(messageId);
            numOfMessageRemoved++;
        }
    }


    void generateData(int round) {
        for (int i = 0; i < messageId / messageRecordGap && i < numOfMessageRecord; i++) {
            int roundDiff = round - startRound[i];
            if (roundDiff >= messageRecordFrequency) {
                continue;
            }
            float percentage = float(messagesCount[i * messageRecordGap]) / float(numOfNodes - numOfDeadNodes);
            progress[i][roundDiff] = percentage;
        }
    }



};