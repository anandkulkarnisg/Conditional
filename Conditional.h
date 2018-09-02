#include<iostream>
#include<string>
#include<mutex>
#include<condition_variable>
#include<chrono>

#ifndef Conditional_H
#define Conditional_H

class Conditional
{
	private:
		unsigned int m_count = 0;
		bool m_status = false;
		std::mutex m_mutex;
		std::condition_variable m_cond;

	public:
		Conditional();
		Conditional(const Conditional&) = delete;
		Conditional& operator=(const Conditional&) = delete;
		void wait();
		bool wait(const long&);
		void signalOne();
		void signalAll();
		~Conditional();
};

#endif

