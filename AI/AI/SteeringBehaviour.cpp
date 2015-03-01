#include "SteeringBehaviour.h"

SteeringBehaviour::SteeringBehaviour(Zombie *z, Scene *s): owner(z), seek_target(0),
wander_on(0), seek_on(0), flee_on(0), arrive_on(0), obstacle_avoidance_on(1), scene(s),
hide_on(0), pursuit_on(0)
{
	steering_force = glm::vec2(0.0f, 0.0f);

	// ---------- WANDER INIT ---------- //
	wander_radius	= 3.0f;
	wander_distance = 5.0f;
	wander_jitter	= 1.2f;
	float alpha = (float)(rand()%360) * (float)(PI/180.0f);
	wander_target = glm::vec2(wander_radius*cos(alpha), wander_radius*sin(alpha));
	wander_target_point.InitPoint(wander_target, 0.2, glm::vec3(0, 0, 1));

	for(int i=0; i<4; ++i)
	{
		//hiding_spot[i].InitPoint(glm::vec2(0, 0), 0.2, glm::vec3(1, 0, 1));
	}
}

SteeringBehaviour::~SteeringBehaviour(void)
{

}

glm::vec2 SteeringBehaviour::CalculateSteeringForce(void)
{
	ToggleState();

	const double mult_obstacle_avoidance	= 0.9;
	const double mult_hide					= 0.7;
	const double mult_seek					= 0.7;
	const double mult_wander				= 0.5;
	const double mult_pursuit				= 0.7;

	steering_force = glm::vec2(0.0f, 0.0f);
	glm::vec2 force;

	if(obstacle_avoidance_on)
	{
		force = CalculateObstacleAvoidance();
		SetLength(force, ZOMBIE_MAX_FORCE);
		force *= mult_obstacle_avoidance;
		if(!AccumulateForce(steering_force, force)) return steering_force;
	}

	if(hide_on)
	{
		force = CalculateHide();
		SetLength(force, ZOMBIE_MAX_FORCE);
		force *= mult_hide;
		if(!AccumulateForce(steering_force, force)) return steering_force;
	}

	if(seek_on)
	{
		force = CalculateSeek(seek_target);
		SetLength(force, ZOMBIE_MAX_FORCE);
		force *= mult_seek;
		if(!AccumulateForce(steering_force, force)) return steering_force;
	}

	/*if(pursuit_on)
	{
		force = CalculatePursuit();
		force *= mult_pursuit;
		if(!AccumulateForce(steering_force, force)) return steering_force;
	}*/

	if(wander_on)
	{
		force = CalculateWander();
		SetLength(force, ZOMBIE_MAX_FORCE);
		force *= mult_wander;
		if(!AccumulateForce(steering_force, force)) return steering_force;
	}

	SetLength(steering_force, ZOMBIE_MAX_FORCE);
	return steering_force;
}

void SteeringBehaviour::ToggleState()
{
	if(owner->aggressive)
	{
		hide_on = false;
		wander_on = false;
		pursuit_on = true;
		seek_on = true;
	}
	else
	{
		hide_on = true;
		wander_on = true;
		pursuit_on = false;
		seek_on = false;
	}
}

bool SteeringBehaviour::AccumulateForce(glm::vec2 &total, glm::vec2 &force)
{
	double total_length = GetLength(total);
	double remaining_force = ZOMBIE_MAX_FORCE - total_length;

	if(remaining_force <= 0)
		return false;

	double length = GetLength(force);

	if(length <= remaining_force)
		total += force;
	else
		total += force * (glm::vec2(remaining_force, remaining_force) / ZOMBIE_MAX_FORCE);

	return true;
}

 
void SteeringBehaviour::Draw(void)
{
	wander_target_point.DrawPoint();

	//for(int i=0; i<4; ++i)
		//hiding_spot[i].DrawPoint();
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

	return (target_world - owner->object_position);
}

//arrive
glm::vec2 SteeringBehaviour::CalculateSeek(const glm::vec2 &target)
{
	glm::vec2 desired_velocity = target - owner->object_position;
	double distance = GetDistance(owner->GetObjectPosition(), target);
	SetLength(desired_velocity, ZOMBIE_MAX_SPEED);
	if (distance > 0){
		double Decelerate = 0.01;
		double speed = distance / Decelerate;	
		if (speed > ZOMBIE_MAX_SPEED){
			speed = ZOMBIE_MAX_SPEED;
		}
		desired_velocity *= speed / distance;

		return desired_velocity - owner->object_velocity;
	}

	return glm::vec2(0, 0);
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
	float dist_to_closest_ip = 9999.0f;

	GameEntity *pointer_to_closest_ob = 0;
	glm::vec2 local_position_of_closest_ob = glm::vec2(0, 0);

	float angle = GetAngle(glm::vec2(0, 1), owner->object_heading);
	
	for(unsigned int i=0; i<owner->scene->objects.size(); ++i)
	{
		object = owner->scene->objects[i];

		if(owner == object) continue;

		//if dist(obj.pos + zom.pos) > detectionBox + obj.rad -> skip
		if ((GetDistance(object->GetObjectPosition(), owner->GetObjectPosition())) > detection_box_length+ object->GetCollisionRadius()) continue;	

		//ToLocalSpace
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)(-angle), glm::vec3(0, 0, 1));
		glm::vec2 local_position(rot * glm::vec4(object->GetObjectPosition() - owner->GetObjectPosition(), 0.0f, 0.0f));

		//if behind -> skip
		if (local_position.y < 0) continue;		

		//obj.rad + zom.rad
		float expanded_radius = object->GetCollisionRadius() + owner->GetCollisionRadius();

		//if doesn't collide -> skip
		if (fabs(local_position.x) >= expanded_radius) continue;		

		double cX = local_position.x;
		double cY = local_position.y;

		double sqrt_part = sqrt(expanded_radius*expanded_radius - cX*cX);

		//intersection point
		double ip = cY - sqrt_part;
		if(ip <= 0) ip = cY + sqrt_part;

		if(ip < dist_to_closest_ip)
		{
			dist_to_closest_ip = (float)ip;
			pointer_to_closest_ob = object;					//rember closest obj
			local_position_of_closest_ob = local_position;	//rember closest obj position in LocalSpace
		}
	}

	glm::vec2 force(0.0f, 0.0f);

	//if doesnt have IP return 0 force
	if(pointer_to_closest_ob == 0) return force;

	//stronger force the closer we are
	double multiplier = 1.0 + (detection_box_length - local_position_of_closest_ob.y) / detection_box_length;
	
	//if negative -> collapses zombie behaviour
	if (multiplier <= 0) multiplier = 0.1;

	// lateral force with sides
	if (local_position_of_closest_ob.x<0)
		force.x = (float)((pointer_to_closest_ob->GetCollisionRadius() - local_position_of_closest_ob.x) * multiplier);
	else
		force.x = -(float)((pointer_to_closest_ob->GetCollisionRadius() + local_position_of_closest_ob.x) * multiplier);


	const double braking_weight = 0.2;

	force.y = (float)((pointer_to_closest_ob->GetCollisionRadius() - local_position_of_closest_ob.y) * braking_weight);

	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), GetAngle(glm::vec2(0, 1), owner->object_heading), glm::vec3(0, 0, 1));
	glm::vec2 force_world(rot * glm::vec4(force, 0.0f, 0.0f));

	return force_world;
}

glm::vec2 SteeringBehaviour::CalculateHidingSpot(const glm::vec2 &target, const glm::vec2 &obstacle, double radius)
{
	const double dist_from_boundry = 3.0;
	double dist = radius + dist_from_boundry;

	glm::vec2 to_obstacle = obstacle - target;
	Normalize(to_obstacle);
	to_obstacle *= dist;

	return to_obstacle + obstacle;
}

glm::vec2 SteeringBehaviour::CalculateHide(void)
{
	glm::vec2 direction;
	float distance;
	bool b = false;

	for(unsigned int i=0; i<scene->obstacles.size(); ++i)
	{
		direction = scene->player->GetObjectPosition() - owner->GetObjectPosition();
		Normalize(direction);

		distance = GetDistance(owner->GetObjectPosition(), scene->obstacles[i]->GetObjectPosition());
		SetLength(direction, distance);

		if(GetDistance(direction + owner->GetObjectPosition(), scene->obstacles[i]->GetObjectPosition()) < scene->obstacles[i]->GetCollisionRadius())
		{
			b = true;
			break;
		}
	}

	if(b == true)
	{
		return glm::vec2(0, 0);
	}

	double best_dist = 999999.0;
	glm::vec2 best_spot;
	int best_index = -1;

	for(unsigned int i=0; i<scene->obstacles.size(); ++i)
	{
		glm::vec2 hide_spot = CalculateHidingSpot(scene->player->GetObjectPosition(), scene->obstacles[i]->GetObjectPosition(), scene->obstacles[i]->GetCollisionRadius());

		double dist = GetDistanceSqrt(hide_spot, owner->GetObjectPosition());

		if(dist < best_dist)
		{
			best_dist = dist;
			best_spot = hide_spot;
			best_index = i;
		}

		//hiding_spot[i].UpdatePoint(hide_spot, 0.2f, glm::vec3(1, 0, 1));
	}

	//hiding_spot[best_index].UpdatePoint(best_spot, 0.2f, glm::vec3(1, 1, 0));

	return CalculateSeek(best_spot);  // !!!!!! ZMIENIC NA ARRIVE !!!!!!!
}
