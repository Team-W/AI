#include "Scene.h"

Scene::Scene(void)
{
	view_matrix = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));

	// Init Test Objects
	test_zombie = new Zombie(this, 0.0f, 0.0f);

	// Init Objects
	AddObject(test_zombie);
	AddObject(new Obstacle(this, 15.0, 15.0, 7.00));
	AddObject(new Obstacle(this, -12.0, -17.0, 9.00));
	AddObject(new Obstacle(this, 12.0, -6.0, 5.00));
	AddObject(new Obstacle(this, -16.0, 15.0, 6.00));
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
}

void Scene::Draw(void)
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}
}

void Scene::Key(unsigned char key)
{
	switch(key)
	{
		case 'r':
		{
			test_zombie->RandomPoint();
			break;
		}
		default:
		{

		}
	}
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
