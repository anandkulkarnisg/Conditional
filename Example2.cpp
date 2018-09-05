#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<algorithm>
#include<sstream>
#include<chrono>
#include<set>
#include<cassert>

#include "Conditional.h"

using namespace std;

Conditional cond;
mutex cout_mutex;
const int demoThreadPoolSize = 10;
const int demoLoopCount = 1000;
void printMessage(string message) // remove const ref to facilitate std::move
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
	return resultString;
}

void waitThread()
{
	cond.wait();
}

int main(int argc, char* argv[])
{
	set<string> testResultSet;
	
	for(unsigned int i=0; i<demoLoopCount; ++i)
	{
		// create and reserve a thread pool.	
		vector<thread> threadPool;
		threadPool.reserve(demoThreadPoolSize);

		// spwan the threads.
		for(unsigned int i=0; i<demoThreadPoolSize;++i)
			threadPool.emplace_back(thread(&waitThread));

		// Now wait for around 10 seconds and signal all of them to quit.
		this_thread::sleep_for(chrono::milliseconds(10));

		cond.signalAll();

		// Join all the threads.	
		for_each(threadPool.begin(), threadPool.end(), [&](thread& threadItem) { threadItem.join(); });

		// Insert the resulting string status to the set.
		testResultSet.insert(cond.toString());
	
		// Show something on the stdout for progress.
		cout << "Finished the testing case : " << i << endl;
	}

	//assert (testResultSet.size()==1); problem with assert is it will dump core and exit if assertion fails.
	
	if(testResultSet.size() == 1)
		cout << "Test case result : Successfull!!" << endl;
	else
		cout << "Test case failed. There is a race somewhere" << endl;
	return 0;
}
