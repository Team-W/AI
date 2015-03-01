#include "Zombie.h"

Zombie::Zombie(Scene *s)
{
	this->scene = s;
	this->steering_behaviour = new SteeringBehaviour(this, scene);
	this->collision_radius = 1.0f;
	this->object_velocity = glm::vec2(0.0f, 0.0f);
	this->target_position = glm::vec2(0.0f, 0.0f);
	this->object_scale = glm::vec2(0.03f, 0.03f);
	this->aggressive = false;
	this->dead = false;
	this->respawn_timer = ZOMBIE_RESPAWN_TIMER;
	this->texture[0] = SOIL_load_OGL_texture("images/zombie.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y); // ../images/zombie.png - error, but this is correct address of img

	srand((int)time(NULL));
	RandomPosition();
}

Zombie::~Zombie(void)
{
	if(steering_behaviour != 0)
		delete steering_behaviour;
}

void Zombie::Update(double delta_time)
{
	Respawn(delta_time);
	Group();

	if(aggressive)
		color = glm::vec3(0.5f, 0.f, 0.f);
	else
		color = glm::vec3(0.f, 0.5f, 0.f);

	steering_behaviour->SetSeekTarget(scene->player->GetObjectPosition());

	float detection_box_length = (GetLength(this->object_velocity) / ZOMBIE_MAX_SPEED);
	detection_box_length *= MIN_DETECTION_BOX_LENGTH;
	detection_box_length += MIN_DETECTION_BOX_LENGTH;

	object_velocity += steering_behaviour->CalculateSteeringForce() * glm::vec2(delta_time, delta_time);
	Truncate(object_velocity, ZOMBIE_MAX_SPEED);

	GameEntity *object = 0;
	float radius = 0;
	glm::vec2 move = object_velocity * glm::vec2(delta_time, delta_time);
	bool collision = false;
	for(unsigned int i=0; i<scene->objects.size(); ++i)
	{
		object = scene->objects[i];

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
	
	if (object_velocity.length() > 0.000001){
		object_heading = object_velocity;
		Normalize(object_heading);
		object_side = GetPerpendicular(object_heading);
	}

	// tmp to be implemented in collision
	if (object_position.x < -32 || object_position.x > 32 || object_position.y < -32 || object_position.y > 32)
	{
		object_position -= object_velocity * glm::vec2(delta_time, delta_time);
	}

	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::scale(model_matrix, glm::vec3(object_scale, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(object_position, 0.0f));
	model_matrix = glm::rotate(model_matrix, GetAngle(glm::vec2(0, 1), object_heading), glm::vec3(0, 0, 1));
}

void Zombie::Respawn(double delta_time){
	if (!dead)return;

	if (respawn_timer > 0){
		respawn_timer -= delta_time;
		return;
	}

	respawn_timer = ZOMBIE_RESPAWN_TIMER;
	RandomPosition();
}

void Zombie::RandomPosition(){
	this->dead = false;
	this->aggressive = false;
	RandomPoint();
	Player *player = scene->player;
	unsigned int count = 0;
	while (count < scene->objects.size()){
		GameEntity *object = scene->objects[count];
		
		if (this->object_id == object->GetObjectID()){ count++; continue; }

		if (player->GetCollisionRadius() + 7 > GetDistance(player->GetObjectPosition(), this->object_position)){
			count = 0; RandomPoint();
			continue;
		}

		if (object->GetCollisionRadius() + 1 > GetDistance(object->GetObjectPosition(), this->object_position)){
			count = 0; RandomPoint();
			continue;
		}
		count++;
	}
}
void Zombie::RandomPoint()
{
	int sign1 = (((float)rand()/RAND_MAX - 0.5)>0) ? 1:-1;
	int sign2 = (((float)rand()/RAND_MAX - 0.5)>0) ? 1:-1;
	this->object_position = glm::vec2(sign1 * (float)(rand() % 320) / 10, sign2 * (float)(rand() % 320) / 10);
	
}
void Zombie::MousePoint(glm::vec2 target){
	target_position = target;
}

void Zombie::Draw()
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glPushMatrix();
		GLfloat Matrix[16];
		MatrixToArray(Matrix, model_matrix, scene->GetViewMatrix());
		glLoadMatrixf(Matrix);

		glBindTexture(GL_TEXTURE_2D, this->texture[0]);
		glBegin(GL_QUADS);	
			glColor3f(color.x, color.y, color.z);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.50f, -0.60f, 0.f); 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.50f, -0.60f, 0.f); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.50f, 0.75f, 0.f); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.50f, 0.75f, 0.f); 
		glEnd();
	glPopMatrix();
	glDisable(GL_LINE_SMOOTH);
}

void Zombie::gotHit()
{
	this->dead = true;
	object_position = glm::vec2(2000, 2000);
	scene->PrintPlayerData();
}

void Zombie::Group()
{
	const double radius = 6.0f;

	int group = 0;

	for(unsigned int j=0; j<scene->zombies.size(); ++j)
	{
		if(GetDistance(object_position, scene->zombies[j]->GetObjectPosition()) < radius)
		{
			++group;
			if(scene->zombies[j]->aggressive)
			{
				this->aggressive = true;
				return;
			}
		}
	}

	if(group >= ZOMBIE_GROUP)
	{
		this->aggressive = true;
	}
}

ostream& operator<<(ostream &o, const Zombie &z)
{
	o.setf(ios::fixed, ios::floatfield);
	o.precision(4);
    o << "--------------- ZOMBIE ---------------" << "\n";
	o << "Position:\t["  << z.object_position.x << ", " << z.object_position.y << "]\n";
	o << "Target:\t\t["  << z.target_position.x << ", " << z.target_position.y << "]\n";
	o << "Velocity:\t["  << z.object_velocity.x << ", " << z.object_velocity.y << "]\n";
	o << "Heading:\t["   << z.object_heading.x	<< ", " << z.object_heading.y	<< "]\n";
    o << "--------------------------------------" << endl;
	o.unsetf(ios::floatfield);
    return o;
}

