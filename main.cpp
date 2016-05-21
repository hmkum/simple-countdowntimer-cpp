#include <iostream>
#include <sstream>

#include "countdowntimer.h"

void onTick(double remainingMs)
{
	std::stringstream ss;
	ss << "Tick 1: " << remainingMs / 1000.0 << "\n";
	std::cout << ss.str();
}

void onTick2(double remainingMs)
{
	std::stringstream ss;
	ss << "Tick 2: " << remainingMs / 1000.0 << "\n";
	std::cout << ss.str();
}

void onFinish()
{
	std::cout << "Finish\n";
}

int main()
{
	// this will call onTick function on every 3 seconds
	// and call onFinish function when 20 seconds expires
	hmk::CountDownTimer cdt(20 * 1000.0, 3 * 1000.0, onTick, onFinish);
	cdt.start();

	hmk::CountDownTimer cdt2(10 * 1000.0, 1 * 1000.0, onTick2, onFinish);
	cdt2.start();

	// after 5 seconds, first timer will be canceled.
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	cdt.cancel();

	return 0;
}
