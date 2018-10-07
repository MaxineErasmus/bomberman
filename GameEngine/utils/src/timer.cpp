#include "../includes/timer.hpp"

namespace  GameBoi{

	Timer *Timer::sInstance = NULL;

	Timer *Timer::Instance(void){
		if (sInstance == NULL){
			sInstance = new Timer();
		}
		return sInstance;
	}

	void Timer::Release(void){
		delete sInstance;
		sInstance = NULL;
	}

	Timer::Timer(void){
		Reset();
		return;
	}

	Timer::~Timer(void){
		return;
	}

	Timer::Timer(const Timer &src){
		static_cast<void>(src);
		return;
	}

	Timer &Timer::operator=(const Timer &rhs){
		static_cast<void>(rhs);
		return *this;
	}

	void Timer::Reset(void) {
		mStartTicks = std::chrono::steady_clock::now();
		mDeltaTime = 0.0f;
	}

	float Timer::DeltaTime(void){
		mElapsedTime += mDeltaTime;
		return mDeltaTime;
	}

	void Timer::update(void){
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		//LISTEN FUCKER DONT CHANGE THE NANOSECONDS OR IT WILL BREAK DOOS!
		std::chrono::duration<float> delta = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - mStartTicks);
		mStartTicks = currentTime;
		mDeltaTime = delta.count();
		mElapsedTime += mDeltaTime;
	}

	void	Timer::resetElapsed(){
		mElapsedTime = 0;
	}
	
	float	Timer::getElapsed()
	{
		return mElapsedTime;
	}
}