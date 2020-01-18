#include "InputHandler.h"


std::queue<KeyEvent> InputHandler::keyEventQueue;
std::queue<MouseEvent> InputHandler::mouseEventQueue;

unsigned char InputHandler::keyStates[255];
unsigned char InputHandler::mouseStates[5];

void InputHandler::handleEvents()
{
	//Handle Mouse events
	for (size_t i = 0; i < mouseEventQueue.size(); i++) {
		MouseEvent currentEvent = mouseEventQueue.front();

		if (currentEvent.button == -1 && currentEvent.state == -1) {
			//Mouse is moving, either freely or dragged.
			//Identify which button(s) are held down, if any
			if (mouseStates[0] == PRESSED) {
				//Left click drag
			}
			if (mouseStates[1] == PRESSED) {
			}
			if (mouseStates[2] == PRESSED) {
			}

		}
		else {
			//A button was pressed or released
			if (!currentEvent.state) {
				//Mouse down event
				mouseStates[currentEvent.button] = PRESSED;
			}
			else {
				//Mouse up event
				mouseStates[currentEvent.button] = RELEASED;
			}
		}

		mouseEventQueue.pop();
	}

	//Handle Key Events
	for (int i = 0; i < keyEventQueue.size(); i++) {
		KeyEvent currentEvent = keyEventQueue.front();

		if (!currentEvent.state) {
			keyStates[currentEvent.key] = PRESSED;
		}
		else {
			keyStates[currentEvent.key] = RELEASED;
		}
		std::cout << currentEvent.key << keyStates[currentEvent.key] << std::endl;

		keyEventQueue.pop();
	}
}
