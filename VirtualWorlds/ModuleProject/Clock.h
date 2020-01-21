#pragma once
#include <GL/gl3w.h>
#include <freeglut 3.0.0/include/GL/freeglut.h>
class Clock
{
public:
	static void start();
	static void tick();
	static void stop();
	static void reportTimingData();

	static double currTime;
	static double deltaTime;
	static bool enabled;

private:
	static float targetFPS;
	static double FPSInMS;


	static double prevTime;
};

