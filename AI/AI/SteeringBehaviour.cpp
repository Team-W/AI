#include "SteeringBehaviour.h"

SteeringBehaviour::SteeringBehaviour(Zombie *z): owner(z),
	wander_on(0), seek_on(0), flee_on(0), arrive_on(0)
{
	steering_force = glm::vec2(0.0f, 0.0f);

	// ---------- WANDER INIT ---------- //
	wander_radius	= 2.0f;
	wander_distance = 5.0f;
	wander_jitter	= 0.8f;
	float alpha = (float)(rand()%360) * PI/180.0f;
	wander_target = glm::vec2(wander_radius*cos(alpha), wander_radius*sin(alpha));
	wander_target_point.InitPoint(wander_target, 0.2, glm::vec3(0, 0, 1));
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

