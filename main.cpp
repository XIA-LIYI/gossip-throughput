#include<iostream>
#include "node.cpp"
#include "messageBox.cpp"
#include "utils/barrier.cpp"
#include "utils/helper.cpp"
#include <thread>
#include <random>
using namespace std;

Barrier sync(numOfThreads);

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
    int valid = 0;
    for (int i = 0; i < numOfMessageRecord; i++) {
        if (messageBox.nintyfiveRound[i] > 0) {
            int latency = messageBox.nintyfiveRound[i] - messageBox.startRound[i] - 1;
            if (latency > maxLatency) {
                maxLatency = latency;
            }
            if (latency < minLatency) {
                minLatency = latency;
            }
            sumOfLatency += latency;
            valid++;
        }
    }
    int averageLatency = sumOfLatency / valid;
    cout << "valid: " << valid << " max latency: " << maxLatency << " min latency: " << minLatency << " average latency: " << averageLatency << endl;
}
random_device rd;
mt19937 gen(rd());
poisson_distribution<> d(2 * bandwidth);

void givePermission(Node nodes[], int numOfNewMessageGenerated) {
    set<int> nodesSelected;
    for (int i = 0; i < numOfNewMessageGenerated; i++) {
        int nodesSelected = gen() % numOfNodes;
        nodes[nodesSelected].newMessagePermission++;
    }
}

void work(int threadId, Node nodes[], MessageBox& messageBox, Helper& helper) {

    for (int i = 1; i <= totalRounds; i++) {
        // each small round
        for (int j = 0; j < int(bandwidth); j++) {
            if (threadId == 0) {
                b = helper.getB();
                t = rand() % numOfNodes;
            }
            sync.wait();
            int start = (numOfNodes / numOfThreads) * threadId;
            int end;
            if (threadId == numOfThreads - 1) {
                end = numOfNodes;
            } else {
                end = start + int(numOfNodes / numOfThreads);
            }
            for (int k = start; k < end; k++) {
                unsigned int inter = k * b + t;
                unsigned int curr = inter % numOfNodes;
                if (curr < numOfDeadNodes) {
                    continue;
                }
                nodes[curr].send(nodes, i, helper);
            }
            sync.wait();
        }
        sync.wait();
        for (int j = threadId + numOfDeadNodes; j < numOfNodes; j = j + numOfThreads) {
            // refresh
            nodes[j].refresh(nodes, helper, i);
        }
        sync.wait();
        if ((threadId == 0) && (i < usefulRound)) {
            int numOfNewMessageGenerated = d(gen);
            givePermission(nodes, numOfNewMessageGenerated);

        }


        if (i % logFrequency == 0 && i < usefulRound) {

            if (threadId == 0) {
                cout << "Round " << i << " finishes." << endl;

                cout << "Round " << i << endl;
                cout << "number of total message is " << nodes[0].messageBox->messageId << endl;
                cout << "number of messages that are received by all is " << nodes[0].messageBox->numOfMessageRemoved.load() << endl;
                cout << "number of messages that are received by 95% of nodes is " << nodes[0].messageBox->numOfMessagesWith95Count.load() << endl;                
                cout << "New message: " << messageBox.numOfNewMessage << endl;
                // cout << nodes[0].messageBox->messagesCount[1] << endl;
                calculateThroughput(nodes, i * bandwidth);
                calculateInstantaneousThroughput(nodes, logFrequency * bandwidth);
                cout << endl;
            }
        }
        if (threadId == 0) {
            messageBox.refresh(i);
        }
        sync.wait();
    }

}

void printInjectionRate(MessageBox& messageBox) {
    int sumInjection = 0;
    int maxInjection = 0;
    int minInjection = 100;
    for (int i = 0; i < usefulRound; i++) {
        int curr = messageBox.newMessageRate[i];
        sumInjection += curr;
        maxInjection = max(maxInjection, curr);
        minInjection = min(minInjection, curr);
    }
    cout << "New message rate max: " << maxInjection << " ave: " << int(sumInjection / usefulRound) << " min: " << minInjection << endl;
}

void printCompleteness(MessageBox& messageBox) {
    // Row is round. Each row contains min, max, ave;
    int requiredMessage = messageBox.messageId * 0.6;
    for (int round = 0; round < messageRecordFrequency; round++) {
        float sum = 0.0;
        int valid = 0;
        for (int message = 0; message < requiredMessage; message++) {
            if (messageBox.progress[message][round] == 0.0) {
                continue;
            }
            sum += messageBox.progress[message][round];
            valid++;
        }
        float average = sum / float(valid);
        float varianceSum = 0.0;
        for (int message = 0; message < requiredMessage; message++) {
            varianceSum += pow(messageBox.progress[message][round] - average, 2.0);
        }
        float std = sqrt(varianceSum / requiredMessage);
        cout << round << " " << average << " " << std << endl; 
    }
}

int main() {
    Helper helper(numOfNodes);
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
    cout << "Number of nodes: " << numOfNodes << endl;
    cout << "Number of dead nodes:" << numOfDeadNodes << endl;
    cout << "Gossip rate: " << gossipRate << endl;
    cout << "bandwith: " << bandwidth << endl;
    cout << "new message limit: " << newMessageLimit << endl;

    cout << "Nodes are all ready." << endl;
    for (int i = 0; i < numOfThreads; i++) {
        threads[i] = thread(work, i, nodes, ref(messageBox), ref(helper));
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
    printCompleteness(messageBox);
    printInjectionRate(messageBox);
}