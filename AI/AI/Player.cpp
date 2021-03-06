#include "Player.h"


Player::Player(Scene *s, GLuint texture, float x, float y)
{
	this->scene = s;
	this->object_position = glm::vec2(x, y);
	this->object_scale = glm::vec2(0.03f, 0.03f);
	this->collision_radius = 1.0f;
	this->mouse = new GraphicDebug();
	this->mouse->InitPoint(glm::vec2(0, 0), 0.8, glm::vec3(0.9, 0.9, 0.9));
	this->color = glm::vec3(0, 0, 0);
	this->CDrail = false;
	this->CDmachine = false;
	this->rail = new GraphicDebug();
	this->rail->InitLine(glm::vec2(0, 0), glm::vec2(1,1), color);
	this->shooting_pos = object_position;
	this->shooting_target = object_position;
	this->texture = texture;
	current_weapon = MACHINE;

	Reset();
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

		if (radius > GetDistance(object_position + move, object->GetObjectPosition()))
		{
			float x = move.x, y = move.y;
			glm::vec2 objpos = object->GetObjectPosition();
			if (x > 0 && objpos.y >= object_position.y){
				move.y = -x; move.x = 0;
				break;
			}
			if (x > 0 && objpos.y < object_position.y){
				move.y = x; move.x = 0;
				break;
			}
			if (x < 0 && objpos.y >= object_position.y){
				move.y = x; move.x = 0;
				break;
			}
			if (x < 0 && objpos.y < object_position.y){
				move.y = -x; move.x = 0;
				break;
			}

			if (y > 0 && objpos.x >= object_position.x){
				move.y = 0; move.x = -y;
				break;
			}
			if (y > 0 && objpos.x < object_position.x){
				move.y = 0; move.x = y;
				break;
			}
			if (y < 0 && objpos.x >= object_position.x){
				move.y = 0; move.x = y;
				break;
			}
			if (y < 0 && objpos.x < object_position.x){
				move.y = 0; move.x = -y;
				break;
			}
			break;
		}
	}

	object_position += move;

	if (object_position.x < -32 || object_position.x > 32 || object_position.y < -32 || object_position.y > 32)
	{
		object_position -= move;
	}

	object_heading = mouse->getPosition() - object_position;
	Normalize(object_heading);

	PowerUp *powerup = 0;
	for(unsigned int i = 0; i<scene->powerups.size(); ++i)
	{
		powerup = scene->powerups[i];

		if(!powerup->spawned)
			continue;

		if(GetDistance(object_position, powerup->GetObjectPosition()) < 1.5f)
		{
			//cout << "You got " << powerup->debug_string << "!" << endl;

			switch(powerup->type)
			{
				case PowerUp::LIFE:
				{
					lifes += powerup->value;
					break;
				}
				case PowerUp::IMMORTALITY:
				{
					immortality_duration += powerup->value;
					break;
				}
				case PowerUp::MACHINE_AMMO:
				{
					machine_ammo += powerup->value;
					break;
				}
				case PowerUp::RAIL_AMMO:
				{
					rail_ammo += powerup->value;
					break;
				}
				case PowerUp::CASH:
				{
					cash += powerup->value;
					break;
				}
			}

			powerup->Despawn();
			scene->PrintPlayerData();
		}
	}

}

void Player::Rotate(glm::vec2 heading)
{
	object_heading = heading-object_position;
	
	Normalize(object_heading);
	this->mouse->UpdatePoint(heading, 0.5, glm::vec3(0.9, 0.9, 0.9));
	
	
}

void Player::Shoot(glm::vec2 fire)
{
	if (current_weapon == RAIL && !CDrail && rail_ammo)
	{
		color = glm::vec3(1, 1, 1);
		CDrail = true;
		RailPhysics();
		--rail_ammo;
		scene->PrintPlayerData();
	}
	else if(current_weapon == MACHINE && !CDmachine && machine_ammo)
	{
		scene->bbo->AddBullet(object_position, object_heading);
		CDmachine = true;
		--machine_ammo;
		scene->PrintPlayerData();
	}
}

void Player::Update(double delta_time)
{
	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::scale(model_matrix, glm::vec3(object_scale, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(object_position, 0.0f));
	model_matrix = glm::rotate(model_matrix, GetAngle(glm::vec2(0, 1), object_heading), glm::vec3(0, 0, 1));
	
	if (color.x - (RAIL_TIMER * rail_lvl) > 0.0){
		color -= (RAIL_TIMER * rail_lvl);
	}else{
		color *= 0;
		CDrail = false;
		shooting_pos = object_position;
		shooting_target = object_heading;
		shooting_target *= 1000;
		shooting_target += this->GetObjectPosition();	
	}

	if(machine_timer - (MACHINE_TIMER * machine_lvl) > 0.0)
	{
		machine_timer -= (MACHINE_TIMER * machine_lvl);
	}
	else
	{
		CDmachine = false;
		machine_timer = 1.0f;
	}

	this->rail->UpdateLine(shooting_pos, shooting_target, color);

	if(immortality_duration > 0)
		(float)immortality_duration -= (float)delta_time;
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
			cash += CASH_PER_ZOMBIE;
			++score;
		}
	}
	
}
void Player::Draw()
{
	//mouse->DrawPoint();
	rail->DrawLine();
	float a, b;

	glPushMatrix();
	GLfloat Matrix[16];
	MatrixToArray(Matrix, model_matrix, scene->GetViewMatrix());
	glLoadMatrixf(Matrix);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	
	glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 0.f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 0.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	if(immortality_duration > 0)
	{
		glBegin(GL_LINES);
			glColor3f(0.4f, 0.4f, 1.0f);

			a = 1.3f * (float)cos(359 * PI / 180.0f);
			b = 1.3f * (float)sin(359 * PI / 180.0f);
			for(int j = 0; j < 360; j++)
			{
				glVertex2f(a, b);
				a = 1.3f * (float)cos(j * PI / 180.0f);
				b = 1.3f * (float)sin(j * PI / 180.0f);
				glVertex2f(a, b);
			}
		glEnd();
	}
	
	glPopMatrix();
	
}

void Player::TakeDamage(int damage){
	health_points -= damage;
	scene->PrintPlayerData();
}

void Player::Respawn()
{
	RandomPosition();
	health_points = PLAYER_HEALTH;
	machine_ammo += PLAYER_STARTING_MACHINE_AMMO / 3;
	rail_ammo += PLAYER_STARTING_RAIL_AMMO / 3;
	--lifes;
	scene->PrintPlayerData();
}

bool Player::CheckValidPosition(const glm::vec2 &position)
{
	GameEntity *object = 0;

	for(unsigned int i = 0; i < scene->objects.size(); ++i)
	{
		object = scene->objects[i];

		if(object->GetCollisionRadius() + 7 > GetDistance(object->GetObjectPosition(), this->object_position))
		{
			return false;
		}
	}
	return true;
}

void Player::RandomPosition()
{
	do { RandomPoint(); } while(!CheckValidPosition(object_position));
}

void Player::RandomPoint()
{
	int sign1 = (((float)rand()/RAND_MAX - 0.5)>0) ? 1:-1;
	int sign2 = (((float)rand()/RAND_MAX - 0.5)>0) ? 1:-1;
	this->object_position = glm::vec2(sign1 * (float)(rand() % 320) / 10, sign2 * (float)(rand() % 320) / 10);
}

void Player::UpgradeWeapon(Player::WEAPON_TYPE type)
{
	if(type == RAIL && rail_lvl != WEAPON_MAX_UPGRADE && cash >= rail_lvl * 100)
	{
		cash -= rail_lvl * 100;
		++rail_lvl;
	}
	else if(type == MACHINE && machine_lvl != WEAPON_MAX_UPGRADE && cash >= machine_lvl * 100)
	{
		cash -= machine_lvl * 100;
		++machine_lvl;
	}
}