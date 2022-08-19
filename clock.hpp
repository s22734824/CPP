#ifndef Clock_HPP
#define Clock_HPP

#include <iostream>
#include <thread>
#include <functional>

class Clock
{
public:
	Clock();
	Clock(const Clock&) = delete;
	Clock& operator=(const Clock&) = delete;
	Clock(Clock&& other) = delete;
	Clock& operator=(Clock&& other)= delete;
	void Print();
	void Stop();
private:
	std::thread t;
	bool stop;
};


void Clock::Print()
{
	while (!stop)
	{
		auto TimeNow = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(TimeNow);
		std::cout << std::ctime(&end_time) << std::endl;
		std::this_thread::sleep_for(1000ms);
	}
}
Clock::Clock()
{
	stop = false;
	t= std::thread(std::bind(&Clock::Print, this));
}
void Clock::Stop()
{
	stop = true;
	t.join();
}
#endif // !1
