#include "Scene.h"

Scene::Scene(void)
{
	view_matrix = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
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
