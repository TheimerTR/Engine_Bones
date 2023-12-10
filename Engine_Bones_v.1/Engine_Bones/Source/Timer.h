#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "External\SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	void Restart();
	void Resume();
	void Pause();

	Uint32 Read();
	float ReadDt();

	bool running;

private:

	Uint32	started_at;
	Uint32	stopped_at;
	Uint32	paused_at;
	Uint32	resumed_at;
	Uint32	time;
};

#endif //__TIMER_H__