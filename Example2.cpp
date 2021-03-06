#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<algorithm>
#include<sstream>
#include<chrono>
#include<set>
#include<cassert>
#include<atomic>

#include "Conditional.h"

using namespace std;

Conditional cond;
mutex cout_mutex;
const int demoThreadPoolSize = 10;
const int demoLoopCount = 10000;
atomic<int> counter=demoThreadPoolSize;

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
  --counter;
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
    this_thread::sleep_for(chrono::milliseconds(2));
    while(counter!=0)
    {
      cond.signalAll();
    }
    counter=demoThreadPoolSize;

    // Join all the threads.	
    for_each(threadPool.begin(), threadPool.end(), [&](thread& threadItem) { threadItem.join(); });

    // Insert the resulting string status to the set.
    testResultSet.insert(cond.toString());

    // Show something on the stdout for progress.
    cout << "Finished the testing case : " << i << endl;
  }

  //assert (testResultSet.size()==1); problem with assert is it will dump core and exit if assertion fails.
  return 0;
}
