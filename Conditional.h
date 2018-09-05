#include<iostream>
#include<string>
#include<mutex>
#include<condition_variable>
#include<chrono>
#include<atomic>

#ifndef Conditional_H
#define Conditional_H

class Conditional
{
	private:
		std::atomic<unsigned int> m_count;
		std::atomic<bool> m_status;
		std::mutex m_mutex;
		std::condition_variable m_cond;

	public:
		Conditional();
        ~Conditional() = default;
		Conditional(const Conditional&) = delete;
		Conditional& operator=(const Conditional&) = delete;
		void wait();
		bool wait(const long&);
		void signalOne();
		void signalAll();
        std::string toString() const;
};

#endif

