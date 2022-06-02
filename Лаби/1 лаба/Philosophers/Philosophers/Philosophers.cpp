#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <random>
#include <ctime>
using namespace std;
int myrand(int min, int max) {
    static mt19937 rnd(time(nullptr));
    return uniform_int_distribution<>(min, max)(rnd);
}
void phil(int ph, mutex& ma, mutex& mb, mutex& mo) {
    for (;;) {  // prevent thread from termination
        int duration = myrand(200, 800);
        {
            // Block { } limits scope of lock
            lock_guard<mutex> gmo(mo);
            cout << ph << " thinks " << duration << "ms\n";
        }
        this_thread::sleep_for(chrono::milliseconds(duration));
        {
            lock_guard<mutex> gmo(mo);
            cout << "\t\t" << ph << " is hungry\n";
        }
        lock_guard<mutex> gma(ma);
        this_thread::sleep_for(chrono::milliseconds(400));
        lock_guard<mutex> gmb(mb);
        duration = myrand(200, 800);
        {
            lock_guard<mutex> gmo(mo);
            cout << "\t\t\t\t" << ph << " eats " << duration << "ms\n";
        }
        this_thread::sleep_for(chrono::milliseconds(duration));
    }
}
int main() {
    cout << "dining Philosophers C++11 with Resource hierarchy\n";
    mutex m1, m2, m3, m4, m5;   // 3 forks are 3 mutexes
    mutex mo;           // for proper output
    // 3 philosophers are 3 threads
    thread t1([&] {phil(1, m1, m2, mo); });
    thread t2([&] {phil(2, m2, m3, mo); });
    thread t3([&] {phil(3, m3, m4, mo); });
    thread t4([&] {phil(4, m4, m5, mo); });
    thread t5([&] {phil(5, m1, m5, mo); });  // Resource hierarchy
    t1.join();  // prevent threads from termination
    t2.join();
    t3.join(); 
    t4.join();
    t5.join();
}