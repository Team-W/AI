#include "Headers.h"
#include "Scene.h"

Scene scene;	

void RenderScene(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glRasterPos2i(0, 0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"kurwa");

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
	scene.KeyState(key,true);
}

void KeyUp(unsigned char key, int x, int y)
{
	scene.KeyState(key,false);
}

void MouseCoords(int x, int y)
{ 
	float a, b;
	a = (float)(33.33*( x - 350) / 350);
	b = (float)(33.33*(-y + 350) / 350);
	scene.PlayerRotate(glm::vec2(a, b));
}

void MouseClick(int button, int state, int x, int y) {
	float a, b;
	a = (float)(33.33*(x - 350) / 350);
	b = (float)(33.33*(-y + 350) / 350);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {		
		scene.PlayerShoot(glm::vec2(a, b));
	}
}

void MouseClicked(int x, int y) {
		float a, b;
		a = (float)(33.33*(x - 350) / 350);
		b = (float)(33.33*(-y + 350) / 350);
		scene.PlayerRotate(glm::vec2(a, b));
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(700, 700);
	glMatrixMode(GL_MODELVIEW);
	glutCreateWindow("Basic AI");
	
	glEnable(GL_TEXTURE_2D);

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutPassiveMotionFunc(MouseCoords);
	glutMotionFunc(MouseClicked);
	glutMouseFunc(MouseClick);
	glutKeyboardFunc(KeyPressed);
	glutKeyboardUpFunc(KeyUp);
	glutMainLoop();

	return 0;
}