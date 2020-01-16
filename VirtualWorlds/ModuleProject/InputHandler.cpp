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

		if (currentEvent.button == NULL && currentEvent.state == NULL) {
			//Mouse is moving, either freely or dragged.

			//Identify which button(s) are held down, if any
			if (mouseStates[0] == PRESSED) {
				std::cout << "LDrag";
			}
			if (mouseStates[1] == PRESSED) {
				std::cout << "MDrag";
			}
			if (mouseStates[2] == PRESSED) {
				std::cout << "RDrag";
			}

		}
		else {
			std::cout << "click";
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

	}
}
