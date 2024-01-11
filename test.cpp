#include<list>
#include<iostream>
using namespace std;

int main() {
    list<pair<int, int> > lst;
    lst.push_back(make_pair(1, 11));
    lst.push_back(make_pair(2, 22));
    lst.push_back(make_pair(3, 33));

    for (auto ele: lst) {
            lst.remove(ele);
            break;
        cout << ele.second << endl;
    }
    for (auto ele: lst) {

        cout << ele.second << endl;
    }
}