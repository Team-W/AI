#include "Scene.h"

Scene::Scene(void)
{
	view_matrix = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	memset(&KeyStates, 0, sizeof(KeyStates));
	// Init Test Objects
	test_zombie = new Zombie(this, 0.0f, 0.0f);
	player = new Player(this, 20.0f, 20.0f);
	// Init Objects
	
	AddObject(new Obstacle(this, 15.0, 15.0, 7.00));
	AddObject(new Obstacle(this, -12.0, -17.0, 9.00));
	AddObject(new Obstacle(this, 12.0, -6.0, 5.00));
	AddObject(new Obstacle(this, -16.0, 15.0, 6.00));
	AddObject(test_zombie);
}

Scene::~Scene(void)
{

}

void Scene::Update(double delta_time)
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(delta_time);
	}
	if (KeyStates['w']) player->Move(glm::vec2(0, 0.3), delta_time);
	if (KeyStates['s']) player->Move(glm::vec2(0, -0.3), delta_time);
	if (KeyStates['a']) player->Move(glm::vec2(-0.3, 0), delta_time);
	if (KeyStates['d']) player->Move(glm::vec2(0.3, 0), delta_time);
	player->Update(delta_time);
}

void Scene::Draw(void)
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}
	player->Draw();
}

void Scene::PlayerRotate(glm::vec2 heading)
{
	player->Rotate(heading);
}

void Scene::PlayerShoot(glm::vec2 aim)
{
	player->Shoot(aim);
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

void Scene::RemoveObject(GameEntity *entity)
{
	
}

ostream& operator<<(ostream &o, const Scene &gw)
{
	o << "---------------- SCENE ---------------" << "\n";

	o << "--------------------------------------" << endl;
	return o;
}
