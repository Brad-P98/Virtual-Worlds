#include "Clock.h"


float Clock::targetFPS = 300;

double Clock::currTime = 0.0;
double Clock::prevTime = 0.0;
double Clock::FPSInMS = (1/targetFPS) * 1000;

double Clock::deltaTime;
bool Clock::enabled;

void Clock::tick()
{
	if (!enabled) return;
	deltaTime = currTime - prevTime;
	if (deltaTime < FPSInMS) 
	{
		Sleep(FPSInMS - deltaTime);
	}
	prevTime = currTime;
}

void Clock::reportTimingData()
{

}
