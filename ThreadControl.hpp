#ifndef ThreadControl_HPP_INCLUDED
#define ThreadControl_HPP_INCLUDED
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <chrono>
#define SLEEP_MS [](const int& ms){std::this_thread::sleep_for(std::chrono::milliseconds(ms));}
class ThreadControl
{
public:
	std::mutex m;
	std::condition_variable cond_var;
	//std::function<void> f;
	std::thread T;
	ThreadControl();
	static void wait(int ms);
	void Pause();
	void Start();
	void Abort();
	void WorkThread();
private:
	bool _isWork;
	bool _abort;
	bool _pause;
};
inline ThreadControl::ThreadControl()
{
	_abort = false;
	_pause = false;
	_isWork = false;
	T = std::thread(std::bind(&ThreadControl::WorkThread,this));
}

inline void ThreadControl::WorkThread()
{
	std::unique_lock<std::mutex> lock(m);//only one thread work
	while (!_abort)
	{
		std::cout << "while Thread" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (_pause)
		{
			std::cout << "pause" << std::endl;
			cond_var.wait(lock);
			std::cout << "pause out" << std::endl;
		}
	}
	std::cout << "Abort"<< std::endl;;
}
inline void ThreadControl::Start()
{
	std::cout << "StartBtn" << std::endl;
	_pause = false;
	cond_var.notify_all();
}
inline void ThreadControl::Pause()
{
	std::cout << "PauseBtn" << std::endl;
	_pause = true;
}
inline void ThreadControl::Abort()
{
	std::cout << "AbortBtn" << std::endl;
	_abort = true;
	T.join();
	_abort = false;
	std::cout << "Thread Join out" << std::endl;
}
inline void ThreadControl::wait(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
#endif
