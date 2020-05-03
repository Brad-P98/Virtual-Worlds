#include "InputHandler.h"


std::queue<KeyEvent> InputHandler::keyEventQueue;
std::queue<MouseEvent> InputHandler::mouseEventQueue;

unsigned int InputHandler::keyStates[255];
unsigned int InputHandler::mouseStates[5];

std::vector<unsigned char> InputHandler::keysUpThisFrame;

glm::vec2 InputHandler::mousePos;
glm::vec2 InputHandler::mouseDelta;
glm::vec2 InputHandler::prevMousePos;

void InputHandler::update()
{
	//Reset delta per frame
	mouseDelta.x = 0.0f;
	mouseDelta.y = 0.0f;

	//Now check for any movement events
	handleEvents();
}

void InputHandler::lateUpdate()
{
	keysUpThisFrame.clear();
}

void InputHandler::handleEvents()
{
	//Handle Mouse events
	for (int i = 0; i < mouseEventQueue.size(); i++) {

		MouseEvent currentEvent = mouseEventQueue.front();
		mousePos = currentEvent.pos;
		//A button was pressed or released
		if (!currentEvent.state) {
			//Mouse down event
			mouseStates[currentEvent.button] = PRESSED;
			mousePos = currentEvent.pos;
			prevMousePos = currentEvent.pos;
		}
		else if(currentEvent.state == -1){
			//mouse moved whilst clicked down
			mouseDelta = mousePos - prevMousePos;
		}
		else {
			//Mouse up event
			mouseStates[currentEvent.button] = RELEASED;
		}

		prevMousePos = mousePos;
		mouseEventQueue.pop();
	}

	//Handle Key Events
	for (int i = 0; i < keyEventQueue.size(); i++) {
		KeyEvent currentEvent = keyEventQueue.front();

		if (currentEvent.state) {
			//Set to pressed
			keyStates[currentEvent.key] = PRESSED;

		}
		else {
			keyStates[currentEvent.key] = RELEASED;
		}

		keyEventQueue.pop();
	}
}

bool InputHandler::checkKeyHeld(char key)
{
	if (keyStates[(int)key] == PRESSED) {
		return true;
	}

	return false;
}

bool InputHandler::checkKeyHeld(int key)
{
	if (keyStates[key] == PRESSED) return true;

	return false;
}

bool InputHandler::checkMousePressed(int button)
{
	if (mouseStates[button] == PRESSED) return true;

	return false;
}

//Returns true during the frame that the key down was measured, else is false.
bool InputHandler::keyDownTriggered(char key)
{
	return false;
}

//Returns true during the frame that the key up was measured, else is false.
bool InputHandler::keyUpTriggered(unsigned char key)
{
	for (int i = 0; i < keysUpThisFrame.size(); i++) {
		if (key == keysUpThisFrame[i]) {
			//Key up this frame
			return true;
		}
	}
	return false;
}


void InputHandler::mouseFunction(int button, int state, int x, int y)
{
	MouseEvent evt = {
		evt.button = button,
		evt.state = state,
		evt.pos = glm::vec2(x,y)
	};
	InputHandler::mouseEventQueue.push(evt);
}

void InputHandler::mouseMove(int x, int y)
{
	MouseEvent evt = {
	evt.button = -1,
	evt.state = -1,
	evt.pos = glm::vec2(x,y)
	};
	InputHandler::mouseEventQueue.push(evt);
}

void InputHandler::mouseDrag(int x, int y)
{
	MouseEvent evt = {
		evt.button = -1,
		evt.state = -1,
		evt.pos = glm::vec2(x,y)
	};
	InputHandler::mouseEventQueue.push(evt);
}

void InputHandler::keyPress(unsigned char key, int x, int y)
{
	KeyEvent evt = {
		evt.key = key,
		evt.state = true,
		evt.pos = glm::vec2(x,y)
	};
	InputHandler::keyEventQueue.push(evt);

}

void InputHandler::keyUp(unsigned char key, int x, int y)
{
	KeyEvent evt = {
	evt.key = key,
	evt.state = false,
	evt.pos = glm::vec2(x,y)
	};
	InputHandler::keyEventQueue.push(evt);
	keysUpThisFrame.push_back(key);
}