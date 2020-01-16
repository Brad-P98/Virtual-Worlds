#pragma once
#include <freeglut 3.0.0/include/GL/freeglut.h>

class Clock
{
public:

	static void tick();
	static void reportTimingData();

	static double deltaTime;
	static bool enabled;

private:
	static float targetFPS;
	static double FPSInMS;

	static double currTime;
	static double prevTime;
};

