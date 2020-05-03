#pragma once
#include <GL/gl3w.h>
#include <freeglut 3.0.0/include/GL/freeglut.h>
#include <glm/glm/glm.hpp>
#include <queue>
#include <iostream>

struct KeyEvent {
	unsigned char key; //ASCII key
	bool state;	//if false, key pressed
	glm::vec2 pos;
};

struct MouseEvent {
	int button, //Left, middle or right e.g. GLUT_LEFT_BUTTON
		state;  //Up or down e.g. GLUT_DOWN
	glm::vec2 pos;
};


class InputHandler
{
public:

	static void update();
	static void lateUpdate();

	//Callbacks
	static void mouseFunction(int button, int state, int x, int y);
	static void mouseMove(int x, int y);
	static void mouseDrag(int x, int y);
	static void keyPress(unsigned char key, int x, int y);
	static void keyUp(unsigned char key, int x, int y);


	static bool checkKeyHeld(char key);
	static bool checkKeyHeld(int key);
	static bool checkMousePressed(int button);

	static bool keyDownTriggered(char key);
	static bool keyUpTriggered(unsigned char key);

	static std::queue<KeyEvent> keyEventQueue;
	static std::queue<MouseEvent> mouseEventQueue;

	static unsigned int keyStates[255];	//ASCII characters
	static unsigned int mouseStates[5];	//Left, mid, right, scr up, scr down

	//Possible states
	static const int RELEASED = 0;
	static const int PRESSED = 1;

	//Key state changes per frame
	static std::vector<unsigned char> keysUpThisFrame;

	//Change in mouse position
	static glm::vec2 mouseDelta;
	static glm::vec2 mousePos;
	static glm::vec2 prevMousePos;
private:

	static void handleEvents();

	//Mouse states for scrolling
	static const int GLUT_SCROLL_UP = 3;
	static const int GLUT_SCROLL_DOWN = 4;



};
