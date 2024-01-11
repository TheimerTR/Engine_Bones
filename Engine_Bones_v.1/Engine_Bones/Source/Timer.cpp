// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
	running = false;
	paused = false;
	started_at = 0;
	stopped_at = 0;
	paused_at = 0;
	resumed_at = 0;
	time = 0;

	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	paused = false;
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	paused = false;
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Resume()
{
	running = true;
	resumed_at = SDL_GetTicks();
	uint32 timeElipsed = resumed_at - paused_at;
	time += timeElipsed;
}

// ---------------------------------------------
void Timer::Pause()
{
	paused = true;
	running = false;
	stopped_at = SDL_GetTicks();
	paused_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Restart()
{
	paused = false;
	running = false;
	started_at = 0;
	stopped_at = 0;
	paused_at = 0;
	resumed_at = 0;
	time = 0;
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at - time;
	}
	else
	{
		return stopped_at - started_at - time;
	}
}

// ---------------------------------------------
float Timer::ReadDt()
{
	if(running == true)
	{
		return (SDL_GetTicks() - started_at - time) / 1000.0f;
	}
	else
	{
		return (stopped_at - started_at - time) / 1000.0f;
	}
}

