#include "Scene.h"

Scene::Scene(void)
{
	view_matrix = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	memset(&KeyStates, 0, sizeof(KeyStates));
	bbo = new BulletBufferObject(this, 50);
	this->debug = false;
	this->game = true;
	this->score = 0;
	this->mouse = new GraphicDebug();
	this->mouse->InitPoint(glm::vec2(0, 0), 0.8, glm::vec3(0.9, 0.9, 0.9));
}

Scene::~Scene(void)
{
	if (player != 0) delete player;
	for (unsigned int i = 0; i < objects.size(); i++){
		if (objects[i] != 0) delete objects[i];
	}

}
void Scene::Init(GLuint *texture){
	this->texture = texture;

	// Init Player
	this->player = new Player(this, this->texture[0], 0.0f, 0.0f);

	// Init Objects	
	AddObstacle(new Obstacle(this, this->texture[2], 15.0, 15.0, 7.00));
	AddObstacle(new Obstacle(this, this->texture[2], -12.0, -17.0, 9.00));
	AddObstacle(new Obstacle(this, this->texture[2], 12.0, -6.0, 5.00));
	AddObstacle(new Obstacle(this, this->texture[2], -16.0, 15.0, 6.00));

	// Init Zombies
	for (unsigned int i = 0; i < ZOMBIE_AMOUNT; ++i) AddZombie(new Zombie(this, this->texture[1]));

	//Init PowerUps
	for (unsigned int i = 0; i < POWERUP_AMOUNT; ++i) AddPowerUp(new PowerUp(this));

	PrintPlayerData();

}

void Scene::PrintResult()
{

}

void Scene::PrintPlayerData()
{

}
string Scene::GetPlayerData()
{
	return player->GetPlayerData();
}

void Scene::Update(double delta_time)
{
	DisplayData();
	if (this->game){
		CheckVictoryCondition();
		bbo->Update(delta_time);
		for (unsigned int i = 0; i < objects.size(); i++)
		{
			objects[i]->Update(delta_time);
		}

		if (KeyStates['w'] || KeyStates['W']) player->Move(glm::vec2(0, 0.3), delta_time);
		if (KeyStates['s'] || KeyStates['S']) player->Move(glm::vec2(0, -0.3), delta_time);
		if (KeyStates['a'] || KeyStates['A']) player->Move(glm::vec2(-0.3, 0), delta_time);
		if (KeyStates['d'] || KeyStates['D']) player->Move(glm::vec2(0.3, 0), delta_time);

		player->Update(delta_time);
	}
	if ((KeyStates['r'] || KeyStates['R']) && !game) Restart();
	if (KeyStates[27]) exit(0); //ESC key quits the game
}

bool Scene::CheckVictoryCondition(void)
{
	if(player->immortality_duration > 0)
		return game;

	for (unsigned int i = 0; i<zombies.size(); ++i)
	{
		if (GetDistance(player->GetObjectPosition(), zombies[i]->GetObjectPosition()) < 2)
		{
			
			//TO DO
			//ADD Zombie (multiple zombie) damage and timer (so they do not insta kill)
			if (player->health_points - zombies[i]->damage <= 0){
				if (player->lifes > 0)
				{
					player->Respawn();
				}
				else
				{
					this->game = false;
					PrintResult();
					break;
				}
			}

			player->TakeDamage(zombies[i]->Attack());

		}
	}

	return game;
}

void Scene::Restart(void){
	this->game = true;
	this->debug = true;
	for (unsigned int i = 0; i < zombies.size(); i++)
	{
		zombies[i]->RandomPosition();	
		zombies[i]->aggressive = false;
	}
	this->debug = false;
	player->Reset();
	for(unsigned int i = 0; i < powerups.size(); i++)
	{
		powerups[i]->Respawn();
	}
	PrintPlayerData();
}

string NumberWithSpaces(int n, int m)
{
	stringstream ss;
	ss << n;

	string s = ss.str();

	while(s.size() < m)
		s = " " + s;

	return s;
}

void Scene::Draw(void)
{
	player->Draw();
	bbo->Draw();
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}
	
	if(game)
	{
		this->mouse->DrawPoint();
	}

	if (!game)
	{
		string game_info = "GAME OVER";
		glColor3f(1.0f, 0.2f, 0.2f);
		glRasterPos2f(-0.25f, 0.05f);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)game_info.c_str());

		game_info = "Press [R] to restart";
		glColor3f(1.0f, 0.2f, 0.2f);
		glRasterPos2f(-0.30f, -0.05f);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)game_info.c_str());
	}


	string rail_info = (player->current_weapon == Player::WEAPON_TYPE::RAIL ? "-> " : "   ");
	rail_info += "Railgun     [" + NumberWithSpaces(player->rail_lvl, 1) +  "] [" + NumberWithSpaces(player->rail_ammo, 4) + "]";

	if(player->current_weapon == Player::WEAPON_TYPE::RAIL)
		glColor3f(1.0f, 1.0f, 1.0f);
	else
		glColor3f(0.2f, 0.2f, 1.0f);
	glRasterPos2f(0.35f, -0.94f);
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)rail_info.c_str());

	string machine_info = (player->current_weapon == Player::WEAPON_TYPE::MACHINE ? "-> " : "   ");
	machine_info += "Machine gun [" + NumberWithSpaces(player->machine_lvl, 1) +  "] [" + NumberWithSpaces(player->machine_ammo, 4) + "]";

	if(player->current_weapon == Player::WEAPON_TYPE::MACHINE)
		glColor3f(1.0f, 1.0f, 1.0f);
	else
		glColor3f(0.2f, 0.2f, 1.0f);
	glRasterPos2f(0.35f, -0.98f);
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)machine_info.c_str());

	string health_info = "Health: " + NumberWithSpaces(player->health_points, 2);
	glColor3f(0.7f, 0.1f, 0.1f);
	glRasterPos2f(-0.99f, -0.86f);
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)health_info.c_str());

	string lifes_info = "Lives: " + NumberWithSpaces(player->lifes + 1, 3);
	glColor3f(1.0f, 0.1f, 0.1f);
	glRasterPos2f(-0.99f, -0.90f);
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)lifes_info.c_str());

	string score_info = "Score: " + NumberWithSpaces(player->score, 3);
	glColor3f(0.3f, 0.8f, 0.8f);
	glRasterPos2f(-0.99f, -0.94f);
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)score_info.c_str());

	string cash_info = "Cash:  " + NumberWithSpaces(player->cash, 3);
	glColor3f(0.2f, 1.0f, 0.2f);
	glRasterPos2f(-0.99f, -0.98f);
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)cash_info.c_str());

	string info = "Press [ESC] to quit";
	glColor3f(0.5f, 0.5f, 0.5f);
	glRasterPos2f(-0.99f, 0.96f);
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)info.c_str());

	info = "Press [Q] to switch weapon";
	glColor3f(0.5f, 0.5f, 0.5f);
	glRasterPos2f(0.30f, 0.96f);
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)info.c_str());

	info = "Press [E] to upgrade weapon";
	glColor3f(0.5f, 0.5f, 0.5f);
	glRasterPos2f(0.30f, 0.92f);
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)info.c_str());
}

void Scene::PlayerRotate(glm::vec2 heading)
{
	player->Rotate(heading);
	this->mouse->UpdatePoint(heading, 0.5, glm::vec3(0.9, 0.9, 0.9));
	
}

void Scene::PlayerShoot(glm::vec2 aim)
{
	player->Shoot(aim);
}

void Scene::KeyState(unsigned char key, bool tf)
{
	KeyStates[key] = tf;

	if(key == 'q' && tf)
	{
		if(player->current_weapon == Player::WEAPON_TYPE::RAIL)
			player->current_weapon = Player::WEAPON_TYPE::MACHINE;
		else
			player->current_weapon = Player::WEAPON_TYPE::RAIL;

		PrintPlayerData();
	}
	else if(key == 'e' && tf)
	{
		if(player->current_weapon == Player::WEAPON_TYPE::MACHINE)
			player->UpgradeWeapon(Player::WEAPON_TYPE::MACHINE);
		else
			player->UpgradeWeapon(Player::WEAPON_TYPE::RAIL);
	}
}

void Scene::AddObject(GameEntity *entity)
{
	objects.push_back(entity);
}
void Scene::AddZombie(Zombie *entity)
{
	zombies.push_back(entity);
	objects.push_back(entity);
}
void Scene::AddObstacle(Obstacle *entity)
{
	obstacles.push_back(entity);
	objects.push_back(entity);
}

void Scene::AddPowerUp(PowerUp *entity)
{
	powerups.push_back(entity);
	objects.push_back(entity);
}

ostream& operator<<(ostream &o, const Scene &gw)
{
	o << "---------------- SCENE ---------------" << "\n";

	o << "--------------------------------------" << endl;
	return o;
}

void Scene::DisplayData()
{
	string str;
	stringstream ss;

	ss << "Lifes: " << player->lifes << endl;
	str = "";
	ss >> str;

	//glRasterPos2i(0, 0);
	//glColor3f(1, 0, 0);
	//glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"Dzialaj");




}
