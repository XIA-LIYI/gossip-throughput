#include<random>
#include<set>
#include<atomic>
#include<queue>
#include<list>
#include "parameters.cpp"
#include "utils/threadsafelist.cpp"
#include "utils/messagelist.cpp"
#include "messageBox.cpp"
#include<vector>
using namespace std;

class Node {
public:
    int id;
    // MessageList messageList;
    set<int> messageList;

    ThreadSafeList tempMessagesReceived;
    queue<int> messagesToEnqueue;

    queue<int>* messageQueues;
    mt19937_64 gen;
    atomic<int> numOfMessagesReceived {};
    atomic<int> numOfMessagesSent {};
    int numOfMessagesValid = 0;
    int currMessagesValid = 0;
    // int numOfMessagesRemoved = 0;

    // for attack
    bool isDead = false;

    MessageBox* messageBox;

    Node() {
        
        random_device rd;
        gen = mt19937_64(rd());
    }

    void initialize(int nodeId, MessageBox* box, bool dead) {
        messageBox = box;
        id = nodeId;
        isDead = dead;
        // messageList.set(numOfMessagesTotal);
        tempMessagesReceived.set(int(bandwidth * 1.5));
        messageQueues = new queue<int>[numOfNodes];
    }

    void enqueue(Node nodes[]) {
        while (!messagesToEnqueue.empty()) {
            auto message = messagesToEnqueue.front();
            messagesToEnqueue.pop();
            set<int> nextReceivers;
            while (nextReceivers.size() < gossipRate) {
                int nextReceiver = gen() % numOfNodes;
                nextReceivers.insert(nextReceiver);
            }
            for (auto receiver: nextReceivers) {
                if (nodes[receiver].checkDuplicate(message)) {
                    continue;
                }
                messageQueues[receiver].push(message);
            }
        }

    }

    void refresh(int round) {
        numOfMessagesSent = 0;

        while (!tempMessagesReceived.empty()) {
            int messageReceived = tempMessagesReceived.pop();
            auto res = messageList.insert(messageReceived);
            // cout << "Node " << id << " receive ";
            // cout << messageReceived << " ";
            if (res.second == true) {
                numOfMessagesValid++;
                currMessagesValid++;
                messageBox->addCount(messageReceived, round);
                messagesToEnqueue.push(messageReceived);
            }
            // if (!messageList.find(messageReceived)) {
            //     throw runtime_error("message not in message list");
            // }
        }
        tempMessagesReceived.clear();
    }

    void removeMessageWithFullCount() {
        for (int i = 0; i < messageBox->messagesWithFullCount.size; i++) {
            auto x = messageList.erase(messageBox->messagesWithFullCount.lst[i]);
            if (x == 0) {
                cout << messageBox->messagesCount[messageBox->messagesWithFullCount.lst[i]] << endl;
            }
        }

        // TODO: test
        // for (auto message: lst) {
        //     auto x = messageList.erase(message);
        //     numOfMessagesRemoved += x;
        //     if (x == 0) {
        //         cout << messageBox->messagesCount[message] << endl;
        //     }
        // }

    }

    bool checkDuplicate(int messageId) {
        if (isDead) {
            return false;
        }
        return messageList.find(messageId) != messageList.end();
    }
    
    void receive(int messageId) {
        // cout << "Node " << id << " receives " << messageId << endl;
        if (isDead) {
            return;
        }
        // numOfMessagesReceived++;
        tempMessagesReceived.push(messageId);
    }

    void sendTo(int receiver, Node nodes[], int round) {
        // cout << "Node " << id << " wants to send to " << receiver << endl;
        // TODO: see the queue length
        bool isSend = false;
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
            if (isSend == true) {
                messageQueues[receiver].push(messageId);
                continue;
            }
            nodes[receiver].receive(messageId);
            isSend = true;
        }
        if (isSend == true) {
            return;
        }
        // First trial: find a arbitrary message to send
        // for (auto messageId: ) {

        // }

        // Second trial: generate a new message to send
        int newMessageId = messageBox->generateNewMessage(round);
        if (newMessageId == -1) {
            return;
        }
        // cout << "Node " << id << " generates " << newMessageId << endl;
        // cout << "Node " << id << " sends to " << receiver << " with message " << newMessageId << endl;
        
        // TODO: see any other implementation method
        // messageList.insert(newMessageId);        
        // numOfMessagesValid++;
        nodes[receiver].receive(newMessageId);
        numOfMessagesSent++;

    }


    void send(Node nodes[], int round) {
        set<int> receivers;
        while (receivers.size() < bandwidth) {
            int receiver = gen() % numOfNodes;
            receivers.insert(receiver);
        }
        for (auto receiver: receivers) {
            sendTo(receiver, nodes, round);
        }
    }
};

