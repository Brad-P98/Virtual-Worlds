#include "Clock.h"

#include "TextFileManager.h"


float Clock::targetFPS = 300;

double Clock::currTime = 0.0;
double Clock::prevTime = 0.0;
double Clock::targetFPSInMS = (1/targetFPS) * 1000;
bool frameLockActive = true;

double Clock::deltaTime;
bool Clock::enabled;

//fps timer
bool Clock::fpsTimerActive = false;
double Clock::fpsTimerStartTime = 0.0;
int Clock::framesPassed = 0;

double Clock::initStartTime = 0.0;

void Clock::start() {
	currTime = glutGet(GLUT_ELAPSED_TIME);
	enabled = true;
	//Start initialization timer here
	initStartTime = currTime;
	TextFileManager::clearFile("StatOutput.txt");
}

void Clock::tick()
{

	if (!enabled) return;
	deltaTime = currTime - prevTime;
	//Frame Lock
	//if (deltaTime < targetFPSInMS && frameLockActive) 
	//{
	//	//Frame time was faster than targeted time per frame, so wait.
	//	Sleep(targetFPSInMS - deltaTime);
	//}
	prevTime = currTime;

	//in MS
	currTime = glutGet(GLUT_ELAPSED_TIME);



	if (fpsTimerActive) {

		framesPassed++;
	}

}

void Clock::stop() {

	enabled = false;
}


void Clock::endInitTimer()
{
	double totalTime = (glutGet(GLUT_ELAPSED_TIME) - initStartTime)/1000;
	TextFileManager::writeFloatStatToFile("StatOutput.txt", "Time taken to initialize", (float)totalTime);
	startFPSTimer();

}

void Clock::startFPSTimer()
{
	if (fpsTimerActive) return;

	fpsTimerActive = true;
	fpsTimerStartTime = glutGet(GLUT_ELAPSED_TIME);
	framesPassed = 0;
}

void Clock::endFPSTimer()
{
	fpsTimerActive = false;
	double fpsTimerEndTime = currTime;
	float avgFPS = framesPassed / ((fpsTimerEndTime - fpsTimerStartTime)/1000);
	TextFileManager::writeFloatStatToFile("StatOutput.txt", "Average FPS", avgFPS);

}
