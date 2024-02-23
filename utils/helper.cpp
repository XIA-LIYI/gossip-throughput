#pragma once
#include <cstdlib>
#include <vector>
using namespace std;

class Helper {
    public:
        vector<int> primes;
        int n, numOfPrimes;
        Helper(int n);
        void generatePrimes(int n);
        int getB();
};

bool isPrime(int x) {
    for (int i = 2; i < x; i++) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

void Helper::generatePrimes(int n) {
    int max_prime = 3000;
    if (n > max_prime) {
        n = max_prime;
    }
    for (int i = 2; i < n; i++) {
        if (isPrime(i)) {
            if (n % i != 0) {
                primes.push_back(i);

            }
        }
    }
    numOfPrimes = primes.size();

}

Helper::Helper(int n) {
    generatePrimes(n);
}

int Helper::getB() {
    int index = rand() % numOfPrimes;

    return primes[index];
}