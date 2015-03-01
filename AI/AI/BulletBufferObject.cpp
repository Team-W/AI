#include "BulletBufferObject.h"

BulletBufferObject::BulletBufferObject(Scene *s, unsigned int size): buffer_size(size), current_index(0), scene(s)
{
	bullet_position.reserve(size);
	bullet_velocity.reserve(size);
	bullet_angle.reserve(size);
	used_bullets.reserve(size);

	for(unsigned int i = 0; i < size; ++i)
	{
		used_bullets.push_back(false);
		bullet_angle.push_back(0.0f);
		bullet_position.push_back(glm::vec2(100, 100));
		bullet_velocity.push_back(glm::vec2(0, 0));
	}

	bullet_max_speed = 500.0f;
	bullet_scale = glm::vec2(0.03f, 0.03f);
	bullet_color = glm::vec3(0.4f, 0.4f, 0.7f);
}

BulletBufferObject::~BulletBufferObject()
{

}

void BulletBufferObject::AddBullet(const glm::vec2 &position, const glm::vec2 &heading)
{
	bullet_position[current_index] = position;
	bullet_velocity[current_index] = heading;
	bullet_angle[current_index] = GetAngle(glm::vec2(0, 1), heading);
	used_bullets[current_index] = true;
	SetLength(bullet_velocity[current_index], bullet_max_speed);
	++current_index;

	if(current_index >= buffer_size)
		current_index = 0;
}

void BulletBufferObject::Update(double delta_time)
{
	glm::vec2 vdt = glm::vec2(delta_time, delta_time);

	for(unsigned int i = 0; i < buffer_size; ++i)
	{
		if(used_bullets[i])
		{
			bullet_position[i] += bullet_velocity[i] * vdt;

			// Collision with a border
			if(bullet_position[i].x > 33 || bullet_position[i].x < -33 || bullet_position[i].y > 33 || bullet_position[i].y < -33)
			{
				bullet_position[i] = glm::vec2(100, 100);
				used_bullets[i] = false;
			}

			// Collision with an Obstacle
			Obstacle *obstacle = 0;
			for(unsigned int j = 0; j < scene->obstacles.size(); ++j)
			{
				obstacle = scene->obstacles[j];

				if(GetDistance(obstacle->GetObjectPosition(), bullet_position[i]) < 0.5 + obstacle->GetCollisionRadius())
				{
					bullet_position[i] = glm::vec2(100, 100);
					used_bullets[i] = false;
				}
			}

			// Collision with a Zombie
			Zombie *zombie = 0;
			for(unsigned int j = 0; j < scene->zombies.size(); ++j)
			{
				zombie = scene->zombies[j];

				if(GetDistance(zombie->GetObjectPosition(), bullet_position[i]) < 0.5 + zombie->GetCollisionRadius())
				{
					bullet_position[i] = glm::vec2(100, 100);
					used_bullets[i] = false;
					zombie->gotHit();
					scene->player->score += 1;
					scene->player->cash += CASH_PER_ZOMBIE;
				}
			}


		}
	}
}

void BulletBufferObject::Draw()
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	for(unsigned int i = 0; i < buffer_size; ++i)
	{
		model_matrix = glm::mat4(1.0f);
		model_matrix = glm::scale(model_matrix, glm::vec3(bullet_scale, 1.0f));
		model_matrix = glm::translate(model_matrix, glm::vec3(bullet_position[i], 0.0f));
		model_matrix = glm::rotate(model_matrix, bullet_angle[i], glm::vec3(0, 0, 1));

		glPushMatrix();
		GLfloat Matrix[16];
		MatrixToArray(Matrix, model_matrix, scene->GetViewMatrix());
		glLoadMatrixf(Matrix);

		glBegin(GL_LINES);
			glColor3f(bullet_color.x, bullet_color.y, bullet_color.z);
			glVertex3f(0.0f, 0.0f, 0.f);
			glVertex3f(0.0f, 0.5f, 0.f);
		glEnd();

		glPopMatrix();
	}
	
	glDisable(GL_LINE_SMOOTH);
}

