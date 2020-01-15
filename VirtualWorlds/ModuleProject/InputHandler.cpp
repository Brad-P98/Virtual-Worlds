#include "InputHandler.h"

using namespace InputHandler;

std::queue<KeyEvent> keyEventQueue;
std::queue<MouseEvent> mouseEventQueue;


void InputHandler::Input::handleEvents()
{

	//Handle Mouse events
	for (size_t i = 0; i < mouseEventQueue.size(); i++) {
		MouseEvent currentEvent = mouseEventQueue.front();
		std::cout << currentEvent.button;

		if (currentEvent.button == GLUT_LEFT_BUTTON) {
			std::cout << "left";
		}
		if (currentEvent.button == GLUT_RIGHT_BUTTON) {
			std::cout << "right";
		}

		//mouseEventQueue.pop();
	}

	//Handle Key Events
	for (int i = 0; i < keyEventQueue.size(); i++) {

	}
}
