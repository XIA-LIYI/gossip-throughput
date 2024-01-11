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
    set<int> messageList;    
    ThreadSafeList tempMessagesReceived;
    list<pair<int, int> > messageForSend;
    mt19937_64 gen;
    atomic<int> numOfMessagesReceived {};
    int numOfMessagesValid = 0;

    MessageBox* messageBox;

    Node() {
        tempMessageReceived.set(128);
        random_device rd;
        gen = mt19937_64(rd());
    }

    void setMessageBox(MessageBox* box) {
        messageBox = box;
    }

    void refresh() {
        // remove message from messageList
        
        // put message in messageReceived
        while (!tempMessagesReceived.empty()) {
            int messageReceived = tempMessagesReceived.pop();
            messageList.insert(messageReceived);
        }
        tempMessageReceived.clear();

        // set all to be 0



    }

    bool checkDuplicate(int messageId) {
        return messageList.find(messageId) != messageList.end();
    }
    
    void recieve(int messageId) {
        numOfMessagesReceived++;
        tempMessageReceived.push(messageId);
        messageBox->addCount(messageId);
    }

    void findAndSend(Node nodes[]) {
        int receiver = gen() % numOfNodes;
        for (auto message: messageForSend) {
            if (nodes[receiver].checkDuplicate(message.first)) {
                continue;
            }
            // send
            nodes[receiver].recieve(message.first);
            message.second++;
            if (message.second == 32) {
                messageForSend.remove(message);
            }
            return;
        }
        int newMessageId = messageBox->generateNewMessage();
        messageForSend.push_back(make_pair(newMessageId, 1));
        nodes[receiver].recieve(newMessageId);
    }

    void send(Node nodes[]) {
        for (int i = 0; i < bandwidth; i++) {
            findAndSend(nodes);
        }
    }
};