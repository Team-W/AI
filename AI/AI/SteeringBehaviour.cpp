#include "SteeringBehaviour.h"

SteeringBehaviour::SteeringBehaviour(Zombie *z): owner(z), seek_target(0),
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
	obstacle_x_axis.InitLine(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 1, 0));
	obstacle_y_axis.InitLine(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 1, 0));
	//obstacle_box.InitLine(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 1, 0));
}

SteeringBehaviour::~SteeringBehaviour(void)
{
	
}

glm::vec2 SteeringBehaviour::CalculateSteeringForce(void)
{
	glm::vec2 force_wander = CalculateWander();
	glm::vec2 force_seek = CalculateSeek(seek_target);
	glm::vec2 force_oa = CalculateObstacleAvoidance();
	steering_force = glm::vec2(0.0f, 0.0f);

	if(GetLength(force_oa) > 0)
	{
		steering_force += force_oa * 0.8f;
		steering_force += force_seek * 0.2f;
		return steering_force;
	}
	else
	{
		return force_seek;
	}
}

void SteeringBehaviour::Draw(void)
{
	wander_target_point.DrawPoint();

	for(int i=0; i<obstacle_number; ++i)
		obstacle_position[i].DrawPoint();
	obstacle_x_axis.DrawLine();
	obstacle_y_axis.DrawLine();
	//obstacle_box.DrawLine();
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

	wander_target_point.UpdatePoint(target_world, 0.2, glm::vec3(0, 0, 1));

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

	float dist_to_closest_ip = 999999.0f;
	GameEntity *pointer_to_closest_ob = 0;
	glm::vec2 local_position_of_closest_ob = glm::vec2(0, 0);

	obstacle_number = 0;
	for(int i=0; i<obstacle_number; ++i)
		obstacle_position[i].Hide();

	obstacle_x_axis.UpdateLine(owner->GetObjectPosition() - GetPerpendicular(owner->object_heading) * glm::vec2(33, 33), owner->GetObjectPosition() + GetPerpendicular(owner->object_heading) * glm::vec2(33, 33), glm::vec3(0, 1, 0));
	obstacle_y_axis.UpdateLine(owner->GetObjectPosition() - owner->GetObjectHeading() * glm::vec2(33, 33), owner->GetObjectPosition() + owner->GetObjectHeading()  * glm::vec2(33, 33), glm::vec3(0, 1, 0));
	//obstacle_box.UpdateLine(owner->GetObjectPosition(), owner->GetObjectPosition() + owner->GetObjectHeading() * glm::vec2(detection_box_length, detection_box_length), glm::vec3(0, 0, 1));

	float angle = GetAngle(glm::vec2(0, 1), owner->object_heading);
	
	for(unsigned int i=0; i<owner->scene->objects.size(); ++i)
	{
		object = owner->scene->objects[i];

		if(object == owner || object == owner->scene->player)
			continue;

		if(GetDistance(object->GetObjectPosition(), owner->GetObjectPosition()) > detection_box_length)
			continue;

		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)(-angle), glm::vec3(0, 0, 1));
		glm::vec2 local_position(rot * glm::vec4(object->GetObjectPosition() - owner->GetObjectPosition(), 0.0f, 0.0f));

		if(local_position.y < 0)
			continue;
		
		obstacle_position[obstacle_number].Hide();
		obstacle_position[obstacle_number++].UpdatePoint(local_position, object->GetCollisionRadius(), glm::vec3(0, 1, 0));
		
		float expanded_radius = object->GetCollisionRadius() + owner->GetCollisionRadius() * 0.5f;

		if(fabs(local_position.x) > expanded_radius)
			continue;

		double cX = local_position.x;
		double cY = local_position.y;

		double sqrt_part = sqrt(expanded_radius*expanded_radius - cX*cX);

		double ip = cY - sqrt_part;
		if(ip <= 0) ip = cY + sqrt_part;

		if(ip < dist_to_closest_ip)
		{
			dist_to_closest_ip = ip;
			pointer_to_closest_ob = object;
			local_position_of_closest_ob = local_position;
		}
	}

	glm::vec2 force(0.0f, 0.0f);

	if(pointer_to_closest_ob == 0)
		return force;

	double multiplier = 1.0 + (detection_box_length - local_position_of_closest_ob.y) / detection_box_length;

	// lateral force
	force.x = (pointer_to_closest_ob->GetCollisionRadius() - local_position_of_closest_ob.x) * multiplier;

	const double braking_weight = 0.2;

	force.y = (pointer_to_closest_ob->GetCollisionRadius() - local_position_of_closest_ob.y) * braking_weight;

	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), GetAngle(glm::vec2(0, 1), owner->object_heading), glm::vec3(0, 0, 1));
	glm::vec2 force_world(rot * glm::vec4(force, 0.0f, 0.0f));

	return force_world;
}
