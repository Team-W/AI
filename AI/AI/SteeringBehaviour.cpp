#include "SteeringBehaviour.h"

SteeringBehaviour::SteeringBehaviour(Zombie *z): owner(z),
wander_on(0), seek_on(0), flee_on(0), arrive_on(0), obstacle_avoidance_on(0)
{
	steering_force = glm::vec2(0.0f, 0.0f);

	// ---------- WANDER INIT ---------- //
	wander_radius	= 2.0f;
	wander_distance = 5.0f;
	wander_jitter	= 0.8f;
	float alpha = (float)(rand()%360) * (float)(PI/180.0f);
	wander_target = glm::vec2(wander_radius*cos(alpha), wander_radius*sin(alpha));
	wander_target_point.InitPoint(wander_target, 0.2, glm::vec3(0, 0, 1));

	obstacle_number = 0;
	for(int i=0; i<20; ++i)
	{
		obstacle_position[i].InitPoint(glm::vec2(0, 0), 0.2, glm::vec3(0, 1, 0));
		obstacle_position[i].Hide();
	}	
}

SteeringBehaviour::~SteeringBehaviour(void)
{
	
}

glm::vec2 SteeringBehaviour::CalculateSteeringForce(void)
{
	return glm::vec2(0, 0);
}

void SteeringBehaviour::Draw(void)
{
	wander_target_point.DrawPoint();

	for(int i=0; i<obstacle_number; ++i)
		obstacle_position[i].DrawPoint();
}

glm::vec2 SteeringBehaviour::CalculateWander(void)
{
	int sign1 = (((float)rand()/RAND_MAX - 0.5)>0) ? 1:-1;
	int sign2 = (((float)rand()/RAND_MAX - 0.5)>0) ? 1:-1;

	wander_target += glm::vec2((double)sign1 * wander_jitter, (double)sign2 * wander_jitter);
	Normalize(wander_target);
	wander_target *= wander_radius;

	glm::vec2 target_local(wander_target + glm::vec2(0, wander_distance));

	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), GetAngle(glm::vec2(0, 1), owner->object_heading), glm::vec3(0, 0, 1));
	glm::vec2 target_world(rot * glm::vec4(target_local, 0.0f, 0.0f));
	target_world += owner->object_position;

	wander_target_point.UpdatePoint(target_world, glm::vec3(0, 0, 1));

	return target_world - owner->object_position;
}

glm::vec2 SteeringBehaviour::CalculateSeek(const glm::vec2 &target)
{
	glm::vec2 desired_velocity = target - owner->object_position;
	SetLength(desired_velocity, ZOMBIE_MAX_SPEED);

	return desired_velocity - owner->object_velocity;
}

glm::vec2 SteeringBehaviour::CalculateFlee(void)
{
	return glm::vec2(0, 0);
}

glm::vec2 SteeringBehaviour::CalculateArrive(void)
{
	return glm::vec2(0, 0);
}

glm::vec2 SteeringBehaviour::CalculateObstacleAvoidance(void)
{
	GameEntity *object = 0;
	float detection_box_length = (GetLength(owner->object_velocity)/ZOMBIE_MAX_SPEED);
	detection_box_length *=	MIN_DETECTION_BOX_LENGTH;
	detection_box_length += MIN_DETECTION_BOX_LENGTH;
	float distance = 0.0f;

	obstacle_number = 0;
	for(int i=0; i<obstacle_number; ++i)
		obstacle_position[i].Hide();
	
	for(unsigned int i=0; i<owner->scene->objects.size(); ++i)
	{
		object = owner->scene->objects[i];

		if(object == owner || object == owner->scene->player)
			continue;

		//if(GetLength(object->GetObjectPosition() - owner->GetObjectPosition()))
			//continue;

		glm::vec2 direction(object->GetObjectPosition() - owner->GetObjectPosition());
		Normalize(direction);
		float angle = GetAngle(owner->object_heading, direction);

		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
		glm::vec2 local_position(rot * glm::vec4(object->GetObjectPosition(), 0.0f, 0.0f));
		local_position -= owner->object_position;

		//glm::mat4 matrix(glm::mat4(1.0f) / object->GetModelMatrix());
		//glm::vec2 local_position(glm::vec4(object->GetObjectPosition(), 0.0f, 0.0f) * matrix);

		obstacle_position[obstacle_number].UpdatePoint(direction + owner->GetObjectPosition(), glm::vec3(0, 1, 0));
		obstacle_position[obstacle_number++].Hide();
	}

	return glm::vec2(0, 0);
}
