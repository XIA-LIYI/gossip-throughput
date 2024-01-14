#include<list>
#include<set>
#include<thread>
#include<iostream>
using namespace std;



void work(list<int> abc) {
    int y = -1;
    for (auto x: abc) {
        if (x - y != 1) {
            cout << x << " " << y << endl;
        }

        y = x;
    }
}

int main() {
    list<int> abc;
    for (int i = 0; i < 5000000; i++) {
        abc.push_back(i);
    }
    thread threads[12];

    for (int i = 0; i < 12; i++) {
        threads[i] = thread(work, abc);
    }

    for (int i = 0; i < 12; i++) {
        threads[i].join();
    }
}