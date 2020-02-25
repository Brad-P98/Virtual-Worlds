#include "Instance.h"

Scene* Instance::m_scene = nullptr;



Instance::Instance()
{
	assert(wrapperInstance == nullptr);
	wrapperInstance = this;
}


Instance::~Instance()
{
}

int Instance::init(int argc, char** argv, int winWidth, int winHeight, const char* winName, Scene* scene)
{
	m_scene = scene;
	glutInit(&argc, argv);
	glutInitContextVersion(3, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Virtual World");

	gl3wInit();

	std::cout << "OpenGL version: " << (char*)(glGetString(GL_VERSION)) << std::endl;
	std::cout << "GLUT version: " << glutGet(GLUT_VERSION) << std::endl;

	glutDisplayFunc(drawFrame);

	glutMouseFunc(InputHandler::mouseFunction);
	//glutPassiveMotionFunc(mouseMove);
	glutMotionFunc(InputHandler::mouseDrag);

	glutKeyboardFunc(InputHandler::keyPress);
	glutKeyboardUpFunc(InputHandler::keyUp);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	return 0;
}


void drawFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wrapperInstance->m_scene->update();
	wrapperInstance->m_scene->render();


	glutPostRedisplay();
	glutSwapBuffers();
}
