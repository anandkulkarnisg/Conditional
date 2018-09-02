#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<algorithm>
#include<sstream>
#include<chrono>

#include "Conditional.h"

using namespace std;

Conditional cond;
mutex cout_mutex;
const int demoThreadPoolSize = 10;

void printCout(const string& message)
{
	unique_lock<mutex> exclusiveLock(cout_mutex);
	cout << message << endl;
}

string getThreadId()
{
	auto myid = this_thread::get_id();
	stringstream ss;
	ss << myid;
	string resultString = ss.str();
	return(resultString);
}

void waitThread()
{
	string message = "currently i am waiting from this thread id = " + getThreadId();
	printCout(message);
	cond.wait();
	message = "thread id = " + getThreadId() + " is quitting now";
	printCout(message);
}

int main(int argc, char* argv[])
{
	// create and reserve a thread pool.	
	vector<thread> threadPool;
	threadPool.reserve(demoThreadPoolSize);
	
	// spwan the threads.
	for(unsigned int i=0; i<demoThreadPoolSize;++i)
		threadPool.emplace_back(thread(&waitThread));

	// Now wait for around 10 seconds and signal all of them to quit.
	cout << "Main thread sleeping now for 10 secs and then signalling to quit" << endl;
	this_thread::sleep_for(chrono::seconds(10));
	cout << "Main thread signalling now to quit one by one" << endl;
	for(unsigned int i=1; i<=demoThreadPoolSize; ++i)
	{
		cond.signalOne();
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
	
	// Join all the threads.	
	for_each(threadPool.begin(), threadPool.end(), [&](thread& threadItem) { threadItem.join(); });
	return(0);
}
