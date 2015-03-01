#include "Obstacle.h"

Obstacle::Obstacle(Scene *s, GLuint texture, double x, double y, double r): object_radius(r)
{
	this->scene = s;
	this->object_position = glm::vec2(x, y);
	this->collision_radius = (float)r;
	this->color = glm::vec3(0.3, 0.1, 0.0);
	object_scale = glm::vec2(0.03f, 0.03f);
	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::scale(model_matrix, glm::vec3(object_scale, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(object_position, 0.0f));
	this->texture = texture;
}

Obstacle::~Obstacle(void)
{

}

void Obstacle::Update(double delta_time)
{

}

void Obstacle::setColor(glm::vec3 color){
	this->color = color;
}

void Obstacle::Draw()
{
	float a, b;
	glPushMatrix();
	GLfloat Matrix[16];
	MatrixToArray(Matrix, model_matrix, scene->GetViewMatrix());
	glLoadMatrixf(Matrix);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-object_radius, -object_radius, 0.f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(object_radius, -object_radius, 0.f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(object_radius, object_radius, 0.f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-object_radius, object_radius, 0.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

ostream& operator<<(ostream &o, const Obstacle &ob)
{
    o << "-------------- OBSTACLE --------------" << "\n";
    o << "--------------------------------------" << endl;
    return o;
}
