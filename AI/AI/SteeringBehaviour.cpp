#include "SteeringBehaviour.h"

SteeringBehaviour::SteeringBehaviour(Zombie *z, Scene *s): owner(z), seek_target(0),
wander_on(0), seek_on(0), flee_on(0), arrive_on(0), obstacle_avoidance_on(0), scene(s),
hide_on(0), pursuit_on(0)
{
	steering_force = glm::vec2(0.0f, 0.0f);

	// ---------- WANDER INIT ---------- //
	wander_radius	= 3.0f;
	wander_distance = 5.0f;
	wander_jitter	= 1.f;
	float alpha = (float)(rand()%360) * (float)(PI/180.0f);
	wander_target = glm::vec2(wander_radius*cos(alpha), wander_radius*sin(alpha));
	wander_target_point.InitPoint(wander_target, 0.2, glm::vec3(0, 0, 1));

	obstacle_number = 0;
	intersection_number = 0;
	for(int i=0; i<20; ++i)
	{
		obstacle_position[i].InitPoint(glm::vec2(0, 0), 0.2, glm::vec3(0, 1, 0));
		obstacle_position[i].Hide();
	}
	for(int i=0; i<10; ++i)
	{
		intersection[i].InitPoint(glm::vec2(0, 0), 0.2, glm::vec3(1, 0, 0));
		intersection[i].Hide();
	}
	for(int i=0; i<4; ++i)
	{
		hiding_spot[i].InitPoint(glm::vec2(0, 0), 0.2, glm::vec3(1, 0, 1));
	}
	obstacle_x_axis.InitLine(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 1, 0));
	obstacle_y_axis.InitLine(glm::vec2(0, 0), glm::vec2(0, 0), glm::vec3(0, 1, 0));
	obstacle_box.InitRectangle(glm::vec2(0, 0), glm::vec2(1, MIN_DETECTION_BOX_LENGTH), glm::vec2(0.03,0.03), glm::vec3(0, 1, 0));
}

SteeringBehaviour::~SteeringBehaviour(void)
{
	
}

glm::vec2 SteeringBehaviour::CalculateSteeringForce(void)
{
	glm::vec2 force_wander = CalculateWander();
	glm::vec2 force_seek = CalculateSeek(seek_target);
	glm::vec2 force_oa = CalculateObstacleAvoidance();
	glm::vec2 force_hide = CalculateHide();
	steering_force = glm::vec2(0.0f, 0.0f);

	steering_force += force_hide;
	return steering_force;
/*
	if(GetLength(force_oa) > 0)
	{
		steering_force += force_oa * 1.f;
		//steering_force += force_seek * 0.2f;
		return steering_force;
	}
	else
	{
		
	}*/

}

glm::vec2 SteeringBehaviour::CalculateSteeringForce_2(void)
{
	ToggleState();

	const double mult_obstacle_avoidance	= 0.8;
	const double mult_hide					= 0.7;
	const double mult_seek					= 0.7;
	const double mult_wander				= 0.5;
	const double mult_pursuit				= 0.7;

	glm::vec2 force_wander = CalculateWander();
	glm::vec2 force_seek = CalculateSeek(seek_target);
	glm::vec2 force_oa = CalculateObstacleAvoidance();
	glm::vec2 force_hide = CalculateHide();
	steering_force = glm::vec2(0.0f, 0.0f);
	glm::vec2 force;

	if(obstacle_avoidance_on)
	{
		force = CalculateObstacleAvoidance();
		force *= mult_obstacle_avoidance;
		if(!AccumulateForce(steering_force, force)) return steering_force;
	}

	if(hide_on)
	{
		force = CalculateHide();
		force *= mult_hide;
		if(!AccumulateForce(steering_force, force)) return steering_force;
	}

	if(seek_on)
	{
		force = CalculateSeek(seek_target);
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
		force *= mult_wander;
		if(!AccumulateForce(steering_force, force)) return steering_force;
	}

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
		total += force * glm::vec2(remaining_force, remaining_force);

	return true;
}

 
void SteeringBehaviour::Draw(void)
{
	wander_target_point.DrawPoint();

	for(int i=0; i<obstacle_number; ++i)
		obstacle_position[i].DrawPoint();
	for(int i=0; i<intersection_number; ++i)
		intersection[i].DrawPoint();
	for(int i=0; i<4; ++i)
		hiding_spot[i].DrawPoint();
	obstacle_x_axis.DrawLine();
	obstacle_y_axis.DrawLine();
	obstacle_box.DrawRectngle();
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

	float dist_to_closest_ip = 999999.0f;
	GameEntity *pointer_to_closest_ob = 0;
	glm::vec2 local_position_of_closest_ob = glm::vec2(0, 0);

	obstacle_number = 0;
	for(int i=0; i<obstacle_number; ++i)
		obstacle_position[i].Hide();
	intersection_number = 0;
	for(int i=0; i<intersection_number; ++i)
		intersection[i].Hide();

	obstacle_x_axis.UpdateLine(owner->GetObjectPosition() - GetPerpendicular(owner->object_heading) * glm::vec2(33, 33), owner->GetObjectPosition() + GetPerpendicular(owner->object_heading) * glm::vec2(33, 33), glm::vec3(0, 1, 0));
	obstacle_y_axis.UpdateLine(owner->GetObjectPosition() - owner->GetObjectHeading() * glm::vec2(33, 33), owner->GetObjectPosition() + owner->GetObjectHeading()  * glm::vec2(33, 33), glm::vec3(0, 1, 0));
	obstacle_box.UpdateRectangle(glm::vec2(0.f, 0.f), glm::vec2(1.f, detection_box_length), glm::vec3(0, 1, 0));

	float angle = GetAngle(glm::vec2(0, 1), owner->object_heading);
	
	for(unsigned int i=0; i<owner->scene->objects.size(); ++i)
	{
		object = owner->scene->objects[i];

		if(object == owner || object == owner->scene->player)
			continue;


		if ((GetDistance(object->GetObjectPosition(), owner->GetObjectPosition())) > detection_box_length+ object->GetCollisionRadius()){
			//cout << GetDistance(object->GetObjectPosition(), owner->GetObjectPosition()) + object->GetCollisionRadius() << ", " << detection_box_length << "\n";
			continue;
		}

		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)(-angle), glm::vec3(0, 0, 1));
		glm::vec2 local_position(rot * glm::vec4(object->GetObjectPosition() - owner->GetObjectPosition(), 0.0f, 0.0f));
		
		if (local_position.y < 0)
			continue;		

		obstacle_position[obstacle_number].Hide();
		obstacle_position[obstacle_number++].UpdatePoint(local_position, object->GetCollisionRadius(), glm::vec3(0, 1, 0));


		float expanded_radius = object->GetCollisionRadius() + owner->GetCollisionRadius();

		if (fabs(local_position.x) > expanded_radius)
			continue;

		cout << expanded_radius << endl;

		double cX = local_position.x;
		double cY = local_position.y;

		double sqrt_part = sqrt(expanded_radius*expanded_radius - cX*cX);

		double ip = cY - sqrt_part;
		if(ip <= 0) ip = cY + sqrt_part;

		intersection[intersection_number].Hide();
		intersection[intersection_number++].UpdatePoint(glm::vec2(0, ip) , 0.2f, glm::vec3(1, 0, 0));

		if(ip < dist_to_closest_ip)
		{
			dist_to_closest_ip = (float)ip;
			pointer_to_closest_ob = object;
			local_position_of_closest_ob = local_position;
		}
	}

	glm::vec2 force(0.0f, 0.0f);

	if(pointer_to_closest_ob == 0)
		return force;

	double multiplier = 1.0 + (detection_box_length - local_position_of_closest_ob.y) / detection_box_length;

	// lateral force
	force.x = (float)((pointer_to_closest_ob->GetCollisionRadius() - local_position_of_closest_ob.x) * multiplier);

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
	double best_dist = 999999.0;
	glm::vec2 best_spot;
	int best_index = -1;

	for(int i=0; i<scene->obstacles.size(); ++i)
	{
		glm::vec2 hide_spot = CalculateHidingSpot(scene->player->GetObjectPosition(), scene->obstacles[i]->GetObjectPosition(), scene->obstacles[i]->GetCollisionRadius());

		double dist = GetDistanceSqrt(hide_spot, owner->GetObjectPosition());

		if(dist < best_dist)
		{
			best_dist = dist;
			best_spot = hide_spot;
			best_index = i;
		}

		hiding_spot[i].UpdatePoint(hide_spot, 0.2f, glm::vec3(1, 0, 1));
	}

	hiding_spot[best_index].UpdatePoint(best_spot, 0.2f, glm::vec3(1, 1, 0));

	return CalculateSeek(best_spot);  // !!!!!! ZMIENIC NA ARRIVE !!!!!!!
}
