#include<iostream>
#include<string>

#include "Conditional.h"

using namespace std;

// First Implement a constructor which really does nothing other than set the variable to false status.
Conditional::Conditional()
{
	m_count.store(0);
	m_status.store(false);
}

// Implement the wait method. wait blocks until either signalOne [ happens to signal this particular thread ] or signalAll [ guranteed ].
void Conditional::wait()
{
	m_count.fetch_add(1);
	unique_lock<mutex> exclusiveLock(m_mutex);
	m_cond.wait(exclusiveLock, [&](){ return(this->m_status==true); });
	exclusiveLock.unlock();
	if(--m_count==0)
		m_status=false;
}

// Implement the wait method with timeout parameter. If it times out with out signal then returns false else upon signal immediately returns with true.
bool Conditional::wait(const long& waitTimeInMilliSecs) 
{
	m_count.fetch_add(1);
	unique_lock<mutex> exclusiveLock(m_mutex);
	m_cond.wait_for(exclusiveLock, std::chrono::milliseconds(waitTimeInMilliSecs), [&](){ return(this->m_status==true); });
	exclusiveLock.unlock();
	bool returnStatus=false;
	if(m_status)
		returnStatus=m_status;

	m_count.fetch_sub(1);
	if(m_count==0)
		m_status.store(false);
	return(returnStatus);
}

// Implement signalOne. This can signal any one thread , but particular thread can not be targeted. currently not supported but can be implemented.
void Conditional::signalOne()
{
	m_status.store(true);
	m_cond.notify_one();
}

// Implement signalAll. This can signal all threads.
void Conditional::signalAll()
{
	m_status.store(true);
	m_cond.notify_all();
}

// Destructor Implementation. currently does nothing.
Conditional::~Conditional()
{

}
