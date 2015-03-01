#include "PowerUp.h"

PowerUp::PowerUp(Scene *s)
{
	scene = s;

	this->model_matrix = glm::mat4(1.0f);
	this->object_scale = glm::vec2(0.03f, 0.03f);
	this->object_position = glm::vec2(0, 0);
	this->object_heading = glm::vec2(0, 0);
	this->object_side = glm::vec2(0, 0);

	this->collision_radius = 0.0f;

	Respawn();
}

void PowerUp::RandomPoint()
{
	int sign1 = (((float)rand()/RAND_MAX - 0.5)>0) ? 1:-1;
	int sign2 = (((float)rand()/RAND_MAX - 0.5)>0) ? 1:-1;
	this->object_position = glm::vec2(sign1 * (float)(rand() % 320) / 10, sign2 * (float)(rand() % 320) / 10);
}

void PowerUp::RandomPosition()
{
	RandomPoint();
	unsigned int count = 0;
	GameEntity *object = 0;

	while(count < scene->objects.size())
	{
		object = scene->objects[count];

		if(this->object_id == object->GetObjectID()){ ++count; continue; }

		if(object->GetCollisionRadius() + 2 > GetDistance(object->GetObjectPosition(), this->object_position))
		{
			count = 0;
			RandomPoint();
			continue;
		}

		++count;
	}
}

void PowerUp::Despawn()
{
	spawned = false;
	object_position = glm::vec2(100, 100);

	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::scale(model_matrix, glm::vec3(object_scale, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(object_position, 0.0f));
	model_matrix = glm::rotate(model_matrix, GetAngle(glm::vec2(0, 1), object_heading), glm::vec3(0, 0, 1));
}

void PowerUp::Respawn()
{
	RandomPosition();
	spawn_timer = POWERUP_RESPAWN_TIME;
	spawned = true;

	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::scale(model_matrix, glm::vec3(object_scale, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(object_position, 0.0f)); 
	model_matrix = glm::rotate(model_matrix, GetAngle(glm::vec2(0, 1), object_heading), glm::vec3(0, 0, 1));

	int random = (rand() % 10);

	switch(random)
	{
		case 0:
			type = LIFE;
			break;
		case 1:
			type = IMMORTALITY;
			break;
		case 2:
		case 3:
		case 4:
			type = MACHINE_AMMO;
			break;
		case 5:
		case 6:
			type = RAIL_AMMO;
			break;
		case 7:
		case 8:
		case 9:
			type = CASH;
			break;
	}
	
	switch(type)
	{
		case LIFE:
		{
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			value = POWERUP_LIFE_VALUE;
			debug_string = "LIFE";
			break;
		}
		case IMMORTALITY:
		{
			color = glm::vec3(1.0f, 0.0f, 7.0f);
			value = POWERUP_IMMORTALITY_VALUE;
			debug_string = "IMMORTALITY";
			break;
		}
		case MACHINE_AMMO:
		{
			color = glm::vec3(0.4f, 0.4f, 0.4f);
			value = POWERUP_MACHINE_AMMO_VALUE;
			debug_string = "MACHINE_AMMO";
			break;
		}
		case RAIL_AMMO:
		{
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			value = POWERUP_RAIL_AMMO_VALUE;
			debug_string = "RAIL_AMMO";
			break;
		}
		case CASH:
		{
			color = glm::vec3(0.2f, 1.0f, 0.2f);
			value = POWERUP_CASH_VALUE;
			debug_string = "CASH";
			break;
		}
	}
}

PowerUp::~PowerUp()
{

}

void PowerUp::Update(double delta_time)
{
	if(spawned) return;

	if(spawn_timer > 0)
		spawn_timer -= delta_time;
	else
		Respawn();
}

void PowerUp::Draw(void)
{
	float a, b;
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glPushMatrix();
	GLfloat Matrix[16];
	MatrixToArray(Matrix, model_matrix, scene->GetViewMatrix());
	glLoadMatrixf(Matrix);

	glBegin(GL_TRIANGLE_FAN);
		glColor3f(color.x, color.y, color.z);

		a = 0.5f * (float)cos(359 * PI / 180.0f);
		b = 0.5f * (float)sin(359 * PI / 180.0f);
		for (int j = 0; j < 360; j++)
		{
			glVertex2f(a, b);
			a = 0.5f * (float)cos(j * PI / 180.0f);
			b = 0.5f * (float)sin(j * PI / 180.0f);
			glVertex2f(a, b);
		}
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-0.30f, 0.00f, 0.f);
		glVertex3f(0.30f, 0.00f, 0.f);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.00f, -0.30f, 0.f);
		glVertex3f(0.00f, 0.30f, 0.f);
	glEnd();

	glDisable(GL_LINE_SMOOTH);

	glBegin(GL_LINES);
		glColor3f(0.2f, 0.2f, 1.0f);

		a = 0.7f * (float)cos(359 * PI / 180.0f);
		b = 0.7f * (float)sin(359 * PI / 180.0f);
		for(int j = 0; j < 360; j++)
		{
			glVertex2f(a, b);
			a = 0.7f * (float)cos(j * PI / 180.0f);
			b = 0.7f * (float)sin(j * PI / 180.0f);
			glVertex2f(a, b);
		}
	glEnd();

	glPopMatrix();
	
}
