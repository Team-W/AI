#include "Player.h"


Player::Player(Scene *s, float x, float y)
{
	this->scene = s;
	this->object_position = glm::vec2(x, y);
	this->object_scale = glm::vec2(0.03f, 0.03f);
	this->mouse = new GraphicDebug();
	this->mouse->InitPoint(glm::vec2(0, 0), 0.8, glm::vec3(0.9, 0.9, 0.9));
	this->CDrail = false;
	this->rail = new GraphicDebug();
	this->mouse->InitLine(glm::vec2(0, 0), glm::vec2(1,1), glm::vec3(1, 1, 1));
}


Player::~Player()
{
	
}

void Player::Move(glm::vec2 move, double delta_time)
{
	SetLength(move, PLAYER_SPEED);
	move *= delta_time;
	object_position += move;
}

void Player::Rotate(glm::vec2 heading)
{
	object_heading = heading-object_position;
	SetLength(object_heading, 1000.0);
	this->rail->UpdateLine(object_position, object_heading, glm::vec3(1, 1, 1));

	Normalize(object_heading);
	this->mouse->UpdatePoint(heading, glm::vec3(0.9, 0.9, 0.9));
	
	
}

void Player::Shoot(glm::vec2 fire)
{
}

void Player::Update(double delta_time)
{
	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::scale(model_matrix, glm::vec3(object_scale, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(object_position, 0.0f));
	model_matrix = glm::rotate(model_matrix, GetAngle(glm::vec2(0, 1), object_heading), glm::vec3(0, 0, 1));

}

void Player::Draw()
{
	mouse->DrawPoint();
	rail->DrawLine();
	float a, b;
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glPushMatrix();
	GLfloat Matrix[16];
	MatrixToArray(Matrix, model_matrix, scene->GetViewMatrix());
	glLoadMatrixf(Matrix);

	glBegin(GL_TRIANGLES);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.50f, -0.60f, 0.f);
		glVertex3f(0.50f, -0.60f, 0.f);
		glVertex3f(0.00f, 0.75f, 0.f);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.5f, 0.0f, 0.0f);

		a = 1.0f * (float)cos(359 * PI / 180.0f);
		b = 1.0f * (float)sin(359 * PI / 180.0f);
		for (int j = 0; j < 360; j++)
		{
			glVertex2f(a, b);
			a = 1.0f * (float)cos(j * PI / 180.0f);
			b = 1.0f * (float)sin(j * PI / 180.0f);
			glVertex2f(a, b);
		}
	glEnd();
	glPopMatrix();
	glDisable(GL_LINE_SMOOTH);
}