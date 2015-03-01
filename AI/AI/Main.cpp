#include "Headers.h"
#include "Scene.h"

GLuint texture[3];
Scene scene;

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// building display

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

	texture[0] = SOIL_load_OGL_texture("../images/cat.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	texture[1] = SOIL_load_OGL_texture("../images/zombie.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	texture[2] = SOIL_load_OGL_texture("../images/obstacle.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	scene.Init(texture);

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