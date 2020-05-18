#pragma once
#include <GL/gl3w.h>
#include <freeglut 3.0.0/include/GL/freeglut.h>
class Clock
{
public:
	static void start();
	static void tick();
	static void stop();
	static void endInitTimer();

	static void startFPSTimer();
	static void endFPSTimer();

	static double currTime;
	static double deltaTime;
	static bool enabled;

private:

	static float targetFPS;
	static double targetFPSInMS;

	static double prevTime;

	//fps Timer
	static bool fpsTimerActive;
	static double fpsTimerStartTime;
	static int framesPassed;

	//init Timer
	static double initStartTime;
};

