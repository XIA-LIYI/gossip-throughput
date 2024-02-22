#include<list>
#include<set>
#include<thread>
#include<iostream>
#include<chrono>
#include<random>
#include"utils/helper.cpp"
using namespace std;




int main() {
    Helper helper(20000);
   chrono::steady_clock sc;   // create an object of `steady_clock` class
   auto start = sc.now();     // start timer
    mt19937_64 gen;
    random_device rd;
    gen = mt19937_64(rd());
int b,t,current;
    for (unsigned int i = 0; i < 1000000000; i++) {
//         if (current == 20000) {
//             current = 0;
// b = helper.getB();
// t = rand() % 20000;
// current = 0;
//         }
//             unsigned int inter = current * b + t;
//             current++;
//             unsigned int receiver = inter % 20000;
int k = gen() % 20000;
    }

   auto end = sc.now();       // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
   auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
   cout<<"Operation took: "<<time_span.count()<<" seconds !!!";
   return 0;

}