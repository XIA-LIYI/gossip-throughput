#include<random>
#include<set>
#include<atomic>
#include<list>
#include "parameters.cpp"
#include "utils/threadsafelist.cpp"
using namespace std;

class Node {
public:
    set<int> messageReceived;    
    ThreadSafeList tempMessageReceived;
    list<pair<int, int> > messageForSend;
    mt19937_64 gen;
    atomic<int> numOfMessagesReceived {};
    int numOfMessagesValid = 0;
    Node() {
        tempMessageReceived.set(128);
        random_device rd;
        gen = mt19937_64(rd());
    }

    bool checkDuplicate(int messageId) {

    }
    
    void recieve() {
        numOfMessagesReceived++;
    }

    void findAndSend(Node nodes[]) {
        while (true) {
            int receiver = gen() % numOfNodes;
            for (int message: messageForSend) {
                if (nodes[receiver].checkDuplicate(message)) {
                    continue;
                }

            }
        }
        // generate receiver

        // check duplicate

        // send

        // if cannot send, generate a new message and remove the message with 32
    }

    void send() {
        for (int i = 0; i < bandwidth; i++) {
            findAndSend();
        }

    }
};