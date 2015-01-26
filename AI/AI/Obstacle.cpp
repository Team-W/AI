#include "Obstacle.h"

Obstacle::Obstacle(Scene *s, double x, double y, double r): object_radius(r)
{
	this->scene = s;
	this->object_position = glm::vec2(x, y);
	this->collision_radius = r;
	object_scale = glm::vec2(0.03f, 0.03f);
	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::scale(model_matrix, glm::vec3(object_scale, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(object_position, 0.0f));
}

Obstacle::~Obstacle(void)
{

}

void Obstacle::Update(double delta_time)
{

}

void Obstacle::Draw()
{
	float a, b;
	glPushMatrix();
	GLfloat Matrix[16];
	MatrixToArray(Matrix, model_matrix, scene->GetViewMatrix());
	glLoadMatrixf(Matrix);
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
}

ostream& operator<<(ostream &o, const Obstacle &ob)
{
    o << "-------------- OBSTACLE --------------" << "\n";
    o << "--------------------------------------" << endl;
    return o;
}
