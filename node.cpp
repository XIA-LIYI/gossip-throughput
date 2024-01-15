#include<random>
#include<set>
#include<atomic>
#include<list>
#include "parameters.cpp"
#include "utils/threadsafelist.cpp"
#include "messageBox.cpp"
using namespace std;

class Node {
public:
    int id;
    set<int> messageList;    
    ThreadSafeList tempMessagesReceived;
    list<pair<int, int> > messageForSend;
    mt19937_64 gen;
    atomic<int> numOfMessagesReceived {};
    atomic<int> numOfMessagesSent {};
    int numOfMessagesValid = 0;
    int numOfMessagesRemoved = 0;

    MessageBox* messageBox;

    Node() {
        tempMessagesReceived.set(128);
        random_device rd;
        gen = mt19937_64(rd());
    }

    void initialize(int nodeId, MessageBox* box) {
        messageBox = box;
        id = nodeId;
    }

    void refresh() {
        while (!tempMessagesReceived.empty()) {
            int messageReceived = tempMessagesReceived.pop();
            auto res = messageList.insert(messageReceived);
            if (messageReceived )
            if (res.second == true) {
                numOfMessagesValid++;
                messageBox->addCount(messageReceived);
                messageForSend.push_back(make_pair(messageReceived, 0));
            }
        }
        tempMessagesReceived.clear();
        cout << "Node " << id << " has " << numOfMessagesValid << " messages and has " << messageForSend.size() << " messages for send" << endl;
        cout << "Node " << id << " has " << endl;
        for (auto x: messageList) {
            cout << x << " ";
        }
        cout << endl;

    }

    void removeMessageWithFullCount() {
        for (int i = 0; i < messageBox->messagesWithFullCount.tail; i++) {
            auto x = messageList.erase(messageBox->messagesWithFullCount.lst[i]);
            numOfMessagesRemoved += x;
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
        return messageList.find(messageId) != messageList.end();
    }
    
    void recieve(int messageId) {
        numOfMessagesReceived++;
        tempMessagesReceived.push(messageId);
    }

    void sendTo(int receiver, Node nodes[]) {
        cout << "Node " << id << " wants to send to " << receiver << " " << messageForSend.size() << endl;
        int size = messageForSend.size();
        list<pair<int, int> > temp;
        bool isOk = false;
        for (int i = 0; i < size; i++) {

            auto front = messageForSend.front();
            messageForSend.pop_front();
            front.second++;
            if (nodes[receiver].checkDuplicate(front.first)) {
                if (front.second < bandwidth) {
                    temp.push_back(front);
                }
                cout << "fail 1" << endl;
                continue;
            }
            if (messageBox->messagesCount[front.first] >= numOfNodes) {
                if (front.second < bandwidth) {
                    temp.push_back(front);
                }
                cout << "fail 2" << endl;
                continue;
            }
            // send
            isOk = true;
            nodes[receiver].recieve(front.first);
            cout << "Node " << id << " sends to " << receiver << " with message " << front.first << " " << front.second << endl;
            numOfMessagesSent++;
            if (front.second < bandwidth) {
                temp.push_back(front);
            }
            break;
        }
        temp.splice(temp.end(), messageForSend);
        messageForSend = temp;
        if (isOk) {
            return;
        }
        int newMessageId = messageBox->generateNewMessage();
        if (newMessageId == -1) {
            return;
        }
        cout << "Node " << id << " generates " << newMessageId << endl;
        cout << "Node " << id << " sends to " << receiver << " with message " << newMessageId << endl;

        messageForSend.push_back(make_pair(newMessageId, 1));
        messageList.insert(newMessageId);        
        numOfMessagesValid++;
        if (receiver == id) {
            nodes[receiver].recieve(newMessageId);
            numOfMessagesSent++;
        }

    }

    void send(Node nodes[]) {
        set<int> receivers;
        while (receivers.size() < bandwidth) {
            int receiver = gen() % numOfNodes;
            receivers.insert(receiver);
        }
        for (auto receiver: receivers) {
            sendTo(receiver, nodes);
        }

    }
};