#include "Player.h"


Player::Player(Scene *s, float x, float y)
{
	this->scene = s;
	this->object_position = glm::vec2(x, y);
	this->object_scale = glm::vec2(0.03f, 0.03f);
	this->collision_radius = 1.0f;
	this->mouse = new GraphicDebug();
	this->mouse->InitPoint(glm::vec2(0, 0), 0.8, glm::vec3(0.9, 0.9, 0.9));
	this->color = glm::vec3(0, 0, 0);
	this->CDrail = false;
	this->rail = new GraphicDebug();
	this->mouse->InitLine(glm::vec2(0, 0), glm::vec2(1,1), color);
	this->shooting_pos = object_position;
	this->shooting_target = object_position;
}


Player::~Player()
{
	
}

void Player::Move(glm::vec2 move, double delta_time)
{
	SetLength(move, PLAYER_SPEED);
	GameEntity *object = 0;
	float radius = 0;
	bool collision = false;
	move *= delta_time;

	

	for(unsigned int i=0; i<scene->obstacles.size(); ++i)
	{
		object = scene->obstacles[i];

		if(object == this) continue;

		radius = object->GetCollisionRadius() + collision_radius;

		if(radius > GetDistance(object_position + move, object->GetObjectPosition()))
		{
			collision = true;
			break;
		}
	}

	if(!collision)
		object_position += move;

	if (object_position.x < -32 || object_position.x > 32 || object_position.y < -32 || object_position.y > 32)
	{
		object_position -= move;
	}

	object_heading = mouse->getPosition() -object_position;
	Normalize(object_heading);

}

void Player::Rotate(glm::vec2 heading)
{
	object_heading = heading-object_position;
	
	Normalize(object_heading);
	this->mouse->UpdatePoint(heading, 0.5, glm::vec3(0.9, 0.9, 0.9));
	
	
}

void Player::Shoot(glm::vec2 fire)
{
	if (!CDrail){
		color = glm::vec3(1, 1, 1);
		CDrail = true;
		RailPhysics();
	}
	
}

void Player::Update(double delta_time)
{
	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::scale(model_matrix, glm::vec3(object_scale, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(object_position, 0.0f));
	model_matrix = glm::rotate(model_matrix, GetAngle(glm::vec2(0, 1), object_heading), glm::vec3(0, 0, 1));
	
	if (color.x - 0.0033f > 0.0){
		color -= 0.0033f;
	}else{
		color *= 0;
		CDrail = false;
		shooting_pos = object_position;
		shooting_target = object_heading;
		shooting_target *= 1000;
		shooting_target += this->GetObjectPosition();	
	}

	this->rail->UpdateLine(shooting_pos, shooting_target, color);
}

void Player::RailPhysics(){

	float distance=1000,tmpDist=0;

	for (unsigned int i = 0; i < scene->obstacles.size(); ++i){
		glm::vec2 target = object_heading;
		Obstacle *obstacle = scene->obstacles[i];
		tmpDist = GetDistance(obstacle->GetObjectPosition(), this->GetObjectPosition());
		SetLength(target, tmpDist);
		target += this->GetObjectPosition();
		if (GetDistance(obstacle->GetObjectPosition(), target) < obstacle->GetCollisionRadius()){		
			if (tmpDist < distance){
				distance = tmpDist;
				shooting_target = target;
			}
		}
	}

	for (unsigned int i = 0; i < scene->zombies.size(); ++i)
	{
		glm::vec2 target = object_heading;
		Zombie *zombie = scene->zombies[i];
		SetLength(target, GetDistance(zombie->GetObjectPosition(), this->GetObjectPosition()));
		target += this->GetObjectPosition();
		if (GetDistance(zombie->GetObjectPosition(), target) < zombie->GetCollisionRadius() && GetDistance(zombie->GetObjectPosition(), this->GetObjectPosition())<distance){
			zombie->gotHit();
		}
	}
	
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
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(-0.50f, -0.60f, 0.f);
		glVertex3f(0.50f, -0.60f, 0.f);
		glVertex3f(0.00f, 0.75f, 0.f);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.5f, 0.5f, 0.0f);

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