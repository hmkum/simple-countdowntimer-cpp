#pragma once

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 - Hakan Mert KUM(hmkum@protonmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies 
 * or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <functional>
#include <chrono>
#include <thread>

namespace hmk
{
using namespace std::chrono;

using TickCallback = std::function<void(double)>;
using FinishCallback = std::function<void()>;

class CountDownTimer
{
public:
	CountDownTimer(double ms, double interval, TickCallback onTick, FinishCallback onFinish)
		: mMilliseconds{ms}
		, mInterval{interval}
		, mOnTickCb{onTick}
		, mOnFinishCb{onFinish}
		, mTickCompleted{true}
		, mCancelled{false}
		, mStarted{false}
	{ }

	~CountDownTimer()
	{	
		if(mTimerThread.joinable())
		{
			mTimerThread.join();
		}
	}

	void start()
	{
		mTimerThread = std::thread(&CountDownTimer::startThread, this);
		mStarted = true;
	}

	void cancel()
	{
		mCancelled = true;
		if(mStarted)
		{
			mTimerThread.join();
		}
	}

private:
	void startThread()
	{
		auto start = high_resolution_clock::now();
		double interval = 0.0;
		while(!mCancelled)
		{
			auto end = high_resolution_clock::now();
			double diff = duration_cast<nanoseconds>(end - start).count();
			diff /= 1000000.0;	
			
			mMilliseconds -= diff;
			if(mMilliseconds <= 0)
			{	
				break;
			}

			interval += diff;
			if(interval >= mInterval && mTickCompleted)
			{
				interval = 0.0;
				mTickCompleted = false;
				mOnTickCb(mMilliseconds);
				mTickCompleted = true;
			}		
		
			start = end;
		}
		mOnFinishCb();
	}

private:
	double mMilliseconds;
	double mInterval;
	TickCallback mOnTickCb;
	FinishCallback mOnFinishCb;
	bool mTickCompleted;
	bool mCancelled;
	bool mStarted;
	std::thread mTimerThread;
};
}

