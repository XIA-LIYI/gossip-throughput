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

void calculateThroughput(Node nodes[], int totalSent) {
    float sumOfThroughput = 0.0;
    float minThroughput = 1.0;
    float maxThroughput = 0.0;
    for (int i = numOfDeadNodes; i < numOfNodes; i++) {
        float throughput = (float) nodes[i].numOfMessagesValid / (float) totalSent;
        minThroughput = min(throughput, minThroughput);
        maxThroughput = max(throughput, maxThroughput);
        sumOfThroughput += throughput;
    }
    float averageThrougput = sumOfThroughput / (float) (numOfNodes - numOfDeadNodes);
    cout << "total throughput max: " << maxThroughput << " min: " << minThroughput << " average: " << averageThrougput << endl;
}

void calculateInstantaneousThroughput(Node nodes[], int totalSent) {
    float sumOfThroughput = 0.0;
    float minThroughput = 1.0;
    float maxThroughput = 0.0;
    for (int i = numOfDeadNodes; i < numOfNodes; i++) {
        float throughput = (float) nodes[i].currMessagesValid / (float) totalSent;
        minThroughput = min(throughput, minThroughput);
        maxThroughput = max(throughput, maxThroughput);
        sumOfThroughput += throughput;
        nodes[i].currMessagesValid = 0;
    }
    float averageThrougput = sumOfThroughput / (float) (numOfNodes - numOfDeadNodes);
    cout << "instantaneous throughput max: " << maxThroughput << " min: " << minThroughput << " average: " << averageThrougput << endl;
}

void calculateGossipLatency(MessageBox& messageBox) {
    // if (messageBox.messageId < 10000) {
    //     return;
    // }
    int count = 0;
    int sumOfLatency = 0;
    int maxLatency = 0;
    int minLatency = 9999;
    for (int i = 0; i < 10000; i++) {
        int id = rand() % messageBox.messageId;
        if (messageBox.nintyfiveRound[id] > 0) {
            int latency = messageBox.nintyfiveRound[id] - messageBox.startRound[id];
            if (latency > maxLatency) {
                maxLatency = latency;
            }
            if (latency < minLatency) {
                minLatency = latency;
            }
            sumOfLatency += latency;
        }
    }
    int averageLatency = sumOfLatency / 10000;
    cout << "max latency: " << maxLatency << " min latency: " << minLatency << " average latency: " << averageLatency << endl;
}

void work(int threadId, Node nodes[], MessageBox& messageBox) {
    for (int i = 1; i <= totalRounds; i++) {
        for (int j = threadId + numOfDeadNodes; j < numOfNodes; j = j + numOfThreads) {
            nodes[j].send(nodes, i);
        }
        sync.wait();
        for (int j = threadId + numOfDeadNodes; j < numOfNodes; j = j + numOfThreads) {
            nodes[j].refresh(i);
        }
        sync.wait();
        for (int j = threadId + numOfDeadNodes; j < numOfNodes; j = j + numOfThreads) {
            nodes[j].removeMessageWithFullCount();
        }
        sync.wait();
        for (int j = threadId + numOfDeadNodes; j < numOfNodes; j = j + numOfThreads) {
            nodes[j].enqueue(nodes);
        }
        sync.wait();
        // for (int j = threadId + numOfDeadNodes; j < numOfNodes; j = j + numOfThreads) {
        //     nodes[j].removeMessageWithFullCount();
        // }
        // sync.wait();

        if (i % logFrequency == 0) {
            // for (int j = threadId; j < numOfNodes; j = j + numOfThreads) {
            //     validateSingleNode(nodes[j]);
            // }
            // sync.wait();
            if (threadId == 0) {
                cout << "Round " << i << " finishes." << endl;
                // validateAll(nodes, i);

                cout << "Round " << i << endl;
                cout << "number of total message is " << nodes[0].messageBox->messageId << endl;
                cout << "number of messages that are received by all is " << nodes[0].messageBox->numOfMessageRemoved.load() << endl;
                calculateThroughput(nodes, i * bandwidth);
                calculateInstantaneousThroughput(nodes, logFrequency * bandwidth);
                cout << endl;
            }
        }
        if (threadId == 0) {
            messageBox.refresh();
        }
        sync.wait();
    }

}

int main() {
    MessageBox messageBox;
    Node* nodes = new Node[numOfNodes];
    for (int i = 0; i < numOfDeadNodes; i++) {
        nodes[i].initialize(i, &messageBox, true);
    }
    for (int i = numOfDeadNodes; i < numOfNodes; i++) {
        nodes[i].initialize(i, &messageBox, false);    
    }

    int range = numOfNodes / numOfThreads;
    thread threads[numOfThreads];
    auto start = chrono::steady_clock::now();
    cout << "Nodes are all ready." << endl;
    for (int i = 0; i < numOfThreads; i++) {
        threads[i] = thread(work, i, nodes, ref(messageBox));
    }

    for (int i = 0; i < numOfThreads; i++) {
        threads[i].join();
    }
    auto end = chrono::steady_clock::now();
    auto usedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    float parsedTime = ((float)usedTime) / 1000;
    cout << "It takes " << parsedTime << " seconds for " << totalRounds << " rounds" << endl;
    cout << "It takes " << parsedTime / totalRounds * 100 << " seconds for 100 rounds" << endl;
    cout << "number of total message is " << nodes[0].messageBox->messageId << endl;
    cout << "number of messages that are received by all is " << nodes[0].messageBox->numOfMessageRemoved.load() << endl;
    calculateGossipLatency(messageBox);
}