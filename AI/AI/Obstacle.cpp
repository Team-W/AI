#include "Obstacle.h"

Obstacle::Obstacle(Scene *s, double x, double y, double r): object_radius(r)
{
	this->scene = s;
	this->object_position = glm::vec2(x, y);
}

Obstacle::~Obstacle(void)
{

}

void Obstacle::Update(double delta_time)
{

}

void Obstacle::Draw()
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	float a, b;
	glPushMatrix();
	glTranslatef(object_position.x, object_position.x, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.3f, 0.1f, 0.0f);

	a = (float)object_radius * (float)cos(359 * PI / 180.0f);
	b = (float)object_radius * (float)sin(359 * PI / 180.0f);
	for (int j = 0; j < 360; j++)
	{
		glVertex2f(a, b);
		a = (float)object_radius * (float)cos(j * PI / 180.0f);
		b = (float)object_radius * (float)sin(j * PI / 180.0f);
		glVertex2f(a, b);
	}

	glEnd();
	glPopMatrix();

	glDisable(GL_LINE_SMOOTH);
}

ostream& operator<<(ostream &o, const Obstacle &ob)
{
    o << "-------------- OBSTACLE --------------" << "\n";
    o << "--------------------------------------" << endl;
    return o;
}
