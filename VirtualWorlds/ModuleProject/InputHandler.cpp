#include "InputHandler.h"


std::queue<KeyEvent> InputHandler::keyEventQueue;
std::queue<MouseEvent> InputHandler::mouseEventQueue;

unsigned int InputHandler::keyStates[255];
unsigned int InputHandler::mouseStates[5];

glm::vec2 InputHandler::mousePos;
glm::vec2 InputHandler::mouseDelta;

void InputHandler::handleEvents()
{
	//Handle Mouse events
	for (int i = 0; i < mouseEventQueue.size(); i++) {
		MouseEvent currentEvent = mouseEventQueue.front();

		//A button was pressed or released
		if (!currentEvent.state) {
			//Mouse down event
			mouseStates[currentEvent.button] = PRESSED;
			mousePos = currentEvent.pos;
		}
		else {
			//Mouse up event
			mouseStates[currentEvent.button] = RELEASED;
		}

		//Calculate any changes to mouse since last event.
		mouseDelta = mousePos - currentEvent.pos;
		mousePos = currentEvent.pos;


		mouseEventQueue.pop();
	}

	//Handle Key Events
	for (int i = 0; i < keyEventQueue.size(); i++) {
		KeyEvent currentEvent = keyEventQueue.front();

		if (currentEvent.state) {
			keyStates[currentEvent.key] = PRESSED;
		}
		else {
			keyStates[currentEvent.key] = RELEASED;
		}

		keyEventQueue.pop();
	}
}

bool InputHandler::checkKeyPressed(char key)
{
	if (keyStates[(int)key] == PRESSED) return true;

	return false;
}

bool InputHandler::checkMousePressed(int button)
{
	if (mouseStates[button] == PRESSED) return true;

	return false;
}
