#include<iostream>
#include "node.cpp"
#include "messageBox.cpp"
#include <thread>
using namespace std;

void work(int threadId, Node nodes[]) {
    for (int j = threadId; j < numOfNodes; j = j + numOfThreads) {
        // send
    }
    for (int j = threadId; j < numOfNodes; j = j + numOfThreads) {
        // clear
        // messageBox
    }
}

int main() {
    MessageBox messageBox;
    Node nodes[numOfNodes];
    for (int i = 0; i < numOfNodes; i++) {
        nodes[i].setMessageBox(&messageBox);
    }

    int range = numOfNodes / numOfThreads;
    thread threads[numOfThreads];
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < numOfThreads; i++) {
        threads[i] = thread(work, i, nodes);
    }

    for (int i = 0; i < numOfThreads; i++) {
        threads[i].join();
    }
    auto end = chrono::steady_clock::now();
    auto usedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    float parsedTime = ((float)usedTime) / 1000;
    cout << "It takes " << parsedTime << " seconds for " << totalRounds << " rounds" << endl;
    cout << "It takes " << parsedTime / totalRounds * 100 << " seconds for 100 rounds" << endl;
}