#include "Scene.h"

Scene::Scene(void)
{
	view_matrix = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	memset(&KeyStates, 0, sizeof(KeyStates));
	// Init Player
	player = new Player(this, 0.0f, 0.0f);

	bool game = true;
	int score = 0;

	// Init Objects	
	AddObstacle(new Obstacle(this, 15.0, 15.0, 7.00));
	AddObstacle(new Obstacle(this, -12.0, -17.0, 9.00));
	AddObstacle(new Obstacle(this, 12.0, -6.0, 5.00));
	AddObstacle(new Obstacle(this, -16.0, 15.0, 6.00));
	for (unsigned int i = 0; i < 15; ++i) AddZombie(new Zombie(this));
}

Scene::~Scene(void)
{
	if (player != 0) delete player;
	//if (test_zombie != 0) delete test_zombie;
	for (int i = 0; i < objects.size(); i++){
		if (objects[i] != 0) delete objects[i];
	}

}

bool Scene::CheckVictoryCondition(void)
{
	int z = zombies.size();
	game = true;
	for(unsigned int i=0; i<zombies.size(); ++i)
	{
		if(GetDistance(player->GetObjectPosition(), zombies[i]->GetObjectPosition()) < 2)
		{
			game = false;
			cout << "PRZEGRALES!" << endl;
		}
		if (GetDistance(player->GetObjectPosition(), zombies[i]->GetObjectPosition()) > 200) --z;
	}
	
	if(z == 0)
	{
		game = false;
		cout << "WYGRALES!" << endl;
	}

	return game;
}

void Scene::Update(double delta_time)
{
	if (CheckVictoryCondition()){

		GroupZombies();
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

	//if (KeyStates['r'] || KeyStates['R']) Restart();
}

void Scene::Draw(void)
{
	player->Draw();
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}
	
}

void Scene::PlayerRotate(glm::vec2 heading)
{
	player->Rotate(heading);
	
}

void Scene::PlayerShoot(glm::vec2 aim)
{
	player->Shoot(aim);
}

void Scene::ZombieTarget(glm::vec2 target)
{
	test_zombie->MousePoint(target);
}

void Scene::Key(unsigned char key)
{
	if (KeyStates['r']) test_zombie->RandomPoint();
}

void Scene::KeyState(unsigned char key, bool tf)
{
	KeyStates[key] = tf;
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

void Scene::GroupZombies(void)
{

}

ostream& operator<<(ostream &o, const Scene &gw)
{
	o << "---------------- SCENE ---------------" << "\n";

	o << "--------------------------------------" << endl;
	return o;
}
