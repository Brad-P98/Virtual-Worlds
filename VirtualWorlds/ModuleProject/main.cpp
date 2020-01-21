#include "main.h"


int main(int argc, char** argv)
{

	init(argc, argv);

	Clock::start();
	scene = new Scene();
	scene->initScene();


	glutMainLoop();

	//Exit
	return 0;
}

int init(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitContextVersion(3, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Virtual World");

	gl3wInit();

	std::cout << "OpenGL version: " << (char*)(glGetString(GL_VERSION)) << std::endl;
	std::cout << "GLUT version: " << glutGet(GLUT_VERSION) << std::endl;

	glutDisplayFunc(drawFrame);

	glutMouseFunc(mouseFunction);
	glutPassiveMotionFunc(mouseMove);
	glutMotionFunc(mouseDrag);

	glutKeyboardFunc(keyPress);
	glutKeyboardFunc(keyUp);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	return 0;
}

void drawFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	scene->update();
	scene->render();


	glutPostRedisplay();
	glutSwapBuffers();
}

void mouseFunction(int button, int state, int x, int y)
{
	MouseEvent evt = {
		evt.button = button,
		evt.state = state,
		evt.x = x,
		evt.y = y
	};
	InputHandler::mouseEventQueue.push(evt);
}

void mouseMove(int x, int y)
{
	MouseEvent evt = {
	evt.button = -1,
	evt.state = -1,
	evt.x = x,
	evt.y = y
	};
	InputHandler::mouseEventQueue.push(evt);
}

void mouseDrag(int x, int y)
{
	MouseEvent evt = {
		evt.button = -1,
		evt.state = -1,
		evt.x = x,
		evt.y = y
	};
	InputHandler::mouseEventQueue.push(evt);
}

void keyPress(unsigned char key, int x, int y)
{
	KeyEvent evt = {
		evt.key = key,
		evt.state = false,
		evt.x = x,
		evt.y = y
	};
	InputHandler::keyEventQueue.push(evt);

}

void keyUp(unsigned char key, int x, int y)
{
	KeyEvent evt = {
	evt.key = key,
	evt.state = true,
	evt.x = x,
	evt.y = y
	};
	InputHandler::keyEventQueue.push(evt);
}

void cleanup()
{
}
