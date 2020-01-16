#pragma once
#include <freeglut 3.0.0/include/GL/freeglut.h>
#include <queue>

#include <iostream>

struct KeyEvent {
	unsigned char key; //ASCII key
	bool state;	//if false, key pressed
	int x,		//mouse x at the time of the key event.
		y;		//ditto for y
};

struct MouseEvent {
	int button, //Left, middle or right e.g. GLUT_LEFT_BUTTON
		state,  //Up or down e.g. GLUT_DOWN
		x,		//mouse x at the time of the mouse event
		y;  	//ditto for y
};


class InputHandler
{
public:

	static void handleEvents();

	static std::queue<KeyEvent> keyEventQueue;
	static std::queue<MouseEvent> mouseEventQueue;

	static unsigned char keyStates[255];	//ASCII characters
	static unsigned char mouseStates[5];	//Left, mid, right, scr up, scr down

	//Possible states
	static const int RELEASED = 0;
	static const int PRESSED = 1;

	//Mouse states for scrolling
	static const int GLUT_SCROLL_UP = 3;
	static const int GLUT_SCROLL_DOWN = 4;

};
