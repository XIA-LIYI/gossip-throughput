#include<iostream>
#include "node.cpp"
#include "messageBox.cpp"
#include "utils/barrier.cpp"
#include <thread>
using namespace std;

Barrier sync(numOfThreads);

void validateSingleNode(Node &node) {
    // 1. size of message list + number of removed = number of valid
    // if (node.messageList.size() + node.messageBox->numOfMessageRemoved != node.numOfMessagesValid) {
    //     cout << node.messageList.size() << " " << node.messageBox->numOfMessageRemoved << " " << node.numOfMessagesValid;
    //     throw runtime_error("Fail condition 1");
    // }
    // 2. total received 
}

void validateAll(Node nodes[], int round) {
    int totalReceived = 0;
    for (int i = 0; i < numOfNodes; i++) {
        totalReceived += nodes[i].numOfMessagesReceived;
    }
    int totalSent = 0;
    for (int i = 0; i < numOfNodes; i++) {
        totalSent += nodes[i].numOfMessagesSent;
    }
    cout << totalSent << " " << totalReceived << endl;
    if (totalReceived > round * bandwidth * numOfNodes) {
        throw runtime_error("Fail condition 2");
    }
}

void calculateThroughput(Node nodes[]) {
    float sumOfThroughput = 0.0;
    float minThroughput = 1.0;
    float maxThroughput = 0.0;
    for (int i = 0; i < numOfNodes; i++) {
        float throughput = (float) nodes[i].numOfMessagesValid / (float) nodes[i].numOfMessagesReceived;
        minThroughput = min(throughput, minThroughput);
        maxThroughput = max(throughput, maxThroughput);
        sumOfThroughput += throughput;
    }
    float averageThrougput = sumOfThroughput / (float) numOfNodes;
    cout << "max: " << maxThroughput << " min: " << minThroughput << " average: " << averageThrougput << endl;
}

void work(int threadId, Node nodes[], MessageBox& messageBox) {
    for (int i = 1; i <= totalRounds; i++) {
        for (int j = threadId; j < numOfNodes; j = j + numOfThreads) {
            // send
            nodes[j].send(nodes);
        }
        sync.wait();
        for (int j = threadId; j < numOfNodes; j = j + numOfThreads) {
            nodes[j].refresh();
        }
        sync.wait();

        for (int j = threadId; j < numOfNodes; j = j + numOfThreads) {
            nodes[j].removeMessageWithFullCount();
        }
        sync.wait();

        if (i % 10 == 0) {
            for (int j = threadId; j < numOfNodes; j = j + numOfThreads) {
                validateSingleNode(nodes[j]);
            }
            sync.wait();
            if (threadId == 0) {
                cout << "Round " << i << " finishes." << endl;
                validateAll(nodes, i);

                cout << "Round " << i << endl;
                cout << "number of total message is " << nodes[0].messageBox->messageId << endl;
                cout << "number of messages that are received by all is " << nodes[0].messageBox->numOfMessageRemoved.load() << endl;
                calculateThroughput(nodes);
                cout << endl;
            }
        }
        messageBox.refresh();
        sync.wait();

    }

}

int main() {
    MessageBox messageBox;
    Node* nodes = new Node[numOfNodes];
    for (int i = 0; i < numOfNodes; i++) {
        nodes[i].initialize(i, &messageBox);
    }

    int range = numOfNodes / numOfThreads;
    thread threads[numOfThreads];
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < numOfThreads; i++) {
        threads[i] = thread(work, i, nodes, ref(messageBox));
    }

    for (int i = 0; i < numOfThreads; i++) {
        threads[i].join();
    }
    for (int i = 0; i < numOfMessagesTotal; i++) {
        int count = messageBox.messagesCount[i];
        if (count != numOfNodes) {
            cout << "messageId: " << i << " count: " << count << endl;
            int checkCount = 0;
            for (int j = 0; j < numOfNodes; j++) {
                if (nodes[j].messageList.find(i) != nodes[j].messageList.end()) {
                    checkCount++;
                }
            }
            cout << checkCount << endl;
        }
    }
    auto end = chrono::steady_clock::now();
    auto usedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    float parsedTime = ((float)usedTime) / 1000;
    cout << "It takes " << parsedTime << " seconds for " << totalRounds << " rounds" << endl;
    cout << "It takes " << parsedTime / totalRounds * 100 << " seconds for 100 rounds" << endl;
}