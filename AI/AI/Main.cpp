#include "Headers.h"
#include "Scene.h"

bool KeyStates[256];
Scene scene;

void Init()
{
	srand((unsigned int)time(NULL));
	memset(&KeyStates, 0, sizeof(KeyStates));
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
		scene.Draw();
	glPopMatrix();

	glutSwapBuffers();
}

void Idle()
{
	static long long int old_time = 0;

	long long int time_since_start = glutGet(GLUT_ELAPSED_TIME);
	double dt = (time_since_start - old_time) * 0.0001f;
	old_time = time_since_start;
	scene.Update(dt);

	glutPostRedisplay();
}

void KeyPressed(unsigned char key, int x, int y)
{
	KeyStates[key] = true;

	scene.Key(key);
}

void KeyUp(unsigned char key, int x, int y)
{
	KeyStates[key] = false;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(700, 700);
	glMatrixMode(GL_MODELVIEW);
	glutCreateWindow("Basic AI");

	Init();

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyPressed);
	glutKeyboardUpFunc(KeyUp);

	glutMainLoop();

	return 0;
}