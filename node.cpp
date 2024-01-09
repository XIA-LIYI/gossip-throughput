#include<random>
#include<set>
#include "parameters.cpp"
using namespace std;

class Node {
public:
    set<int> messageList;

    Node() {


    }

    void checkDuplicate(int messageId) {

    }

    void findAndSend() {
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