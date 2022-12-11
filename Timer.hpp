#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
public:
	Timer() = default;
	static void wait(unsigned long long t);
	static unsigned long long milliseconds();
};

#endif
