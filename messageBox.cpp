#include<set>
#include<atomic>
#include "utils/threadsafelist.cpp"
#include "parameters.cpp"
using namespace std;

class MessageBox {
public:
    atomic<int>  numOfNewMessage {};
    atomic<int> messageId {};
    MessageBox() {
        
    }

    int generateNewMessage() {
        numOfNewMessage++;
        if (numOfNewMessage > newMessageLimit) {
            return -1;
        }
        int res = messageId++;
        return res;
    }

    void clearRestriction() {
        numOfNewMessage = 0;
    }
};