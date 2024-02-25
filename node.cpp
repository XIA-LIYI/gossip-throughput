#include<random>
#include<set>
#include<atomic>
#include<queue>
#include<list>
#include "parameters.cpp"
#include "utils/threadsafelist.cpp"
#include "utils/messagelist.cpp"
#include "utils/helper.cpp"
#include "messageBox.cpp"
#include<vector>
using namespace std;

class Node {
public:
    int id;
    MessageList messageList;
    // set<int> messageList;

    ThreadSafeList tempMessagesReceived;
    queue<int> messagesToEnqueue;

    vector<queue<int>> messageQueues;
    mt19937_64 gen;
    atomic<int> numOfMessagesReceived {};
    atomic<int> numOfMessagesSent {};
    int numOfMessagesValid = 0;
    int currMessagesValid = 0;
    // int numOfMessagesRemoved = 0;

    // for attack
    bool isDead = false;

    MessageBox* messageBox;

    // random generator
    int t, b, current = 0;

    Node() {
        
        random_device rd;
        gen = mt19937_64(rd());
    }

    void initialize(int nodeId, MessageBox* box, bool dead) {
        messageBox = box;
        id = nodeId;
        isDead = dead;
        messageList.set(numOfMessagesTotal);
        tempMessagesReceived.set(int(bandwidth * 1.1), "NODE " + to_string(nodeId));
        for (int i = 0; i < numOfNodes; i++) {
            queue<int> q;
            messageQueues.push_back(q);
        }
    }


    void refresh(Node nodes[], Helper &helper, int round) {
        while (!tempMessagesReceived.empty()) {
            int messageReceived = tempMessagesReceived.pop();
            numOfMessagesValid++;
            currMessagesValid++;
            messageBox->addCount(messageReceived, round);

            set<int> nextReceivers;
            while (nextReceivers.size() < gossipRate) {
                int nextReceiver = gen() % numOfNodes;
                nextReceivers.insert(nextReceiver);
            }
            for (auto receiver: nextReceivers) {
                if (nodes[receiver].find(messageReceived)) {
                    continue;
                }
                messageQueues[receiver].push(messageReceived);
            }

            if (!messageList.find(messageReceived)) {
                throw runtime_error("message not in message list");
            }
        }
        tempMessagesReceived.clear();

        b = helper.getB();
        t = rand() % numOfNodes;
        current = rand() % numOfNodes;

        numOfMessagesReceived = 0;
    }

    bool find(int messageId) {
        return messageList.find(messageId);
    }

    bool checkDuplicate(int messageId) {
        if (isDead) {
            return false;
        }
        return messageList.checkDuplicate(messageId);
    }
    
    void receive(int messageId) {
        // cout << "Node " << id << " receives " << messageId << endl;
        if (isDead) {
            return;
        }
        int currNumOfMessagesReceived = numOfMessagesReceived++;
        if (currNumOfMessagesReceived >= int(bandwidth * 1.1)) {
            // dropped
            return;
        }

        tempMessagesReceived.push(messageId);

    }

    void sendTo(int receiver, Node nodes[], int round) {
        int size = messageQueues[receiver].size();
        int k = 0;
        while (k < size) {
            k++;
            auto messageId = messageQueues[receiver].front();
            messageQueues[receiver].pop();
            if (nodes[receiver].checkDuplicate(messageId)) {
                // duplicate
                continue;
            }
            if (messageBox->messagesCount[messageId] == (numOfNodes - numOfDeadNodes)) {
                continue;
            }           
            // if (isSend) {
            //     messageQueues[receiver].push(messageId);
            //     continue;
            // }

            nodes[receiver].receive(messageId);
            return;
        }
        int newMessageId = messageBox->generateNewMessage(round);
        if (newMessageId == -1) {
            return;
        }
        if (checkDuplicate(newMessageId)) {
            // duplicate
            return;
        }
        receive(newMessageId);
    }


    void send(Node nodes[], int round, Helper &helper) {
        // for (int i = 0; i < bandwidth; i++) {
            unsigned int inter = current * b + t;
            unsigned int receiver = inter % numOfNodes;
            current++;
            if (current == numOfNodes) {
                current = 0;
            }
            sendTo(receiver, nodes, round);
        // }
    }
};

