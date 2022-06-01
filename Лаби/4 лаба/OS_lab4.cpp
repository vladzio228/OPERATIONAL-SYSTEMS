#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;
using namespace std::chrono;

int n = 0;
int N = 10000000;


void incr() {
	for (int i = 0; i < N; i++) {
		n++;
	}
}

void decr() {
	for (int i = 0; i < N; i++) {
		n--;
	}
}

void noMutex() {
	n = 0;
	auto start = system_clock::now();
	thread incr(incr);
	thread decr(decr);
	incr.join();
	decr.join();
	auto stop = system_clock::now();
	cout << "No mutex result: " << n << endl;
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Time no mutex: " << duration.count() << " ms" << endl;
}

mutex m;

void incrAll() {
	m.lock();
	for (int i = 0; i < N; i++) {
		n++;
	}
	m.unlock();
}

void decrAll() {
	m.lock();
	for (int i = 0; i < N; i++) {
		n--;
	}
	m.unlock();
}

void allMutex() {
	n = 0;
	auto start = system_clock::now();
	thread incr(incrAll);
	thread decr(decrAll);
	incr.join();
	decr.join();
	auto stop = system_clock::now();
	cout << "Result with mutex all: " << n << endl;
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Time mutex all " << duration.count() << " ms" << endl;
}

void incrEvery() {
	for (int i = 0; i < N; i++) {
		m.lock();
		n++;
		m.unlock();
	}
}

void decrEvery() {
	for (int i = 0; i < N; i++) {
		m.lock();
		n--;
		m.unlock();
	}
}

void everyMutex() {
	n = 0;
	auto start = system_clock::now();
	thread incr(incrEvery);
	thread decr(decrEvery);
	incr.join();
	decr.join();
	auto stop = system_clock::now();
	cout << "Result with mutex every: " << n << endl;
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Time mutex every: " << duration.count() << " ms" << endl;
}

int main() {
	noMutex();
	allMutex();
	everyMutex();
	return 0;
}