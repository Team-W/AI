#ifndef SCENE_H
#define SCENE_H

#include "Headers.h"
#include "GameEntity.h"
#include "Zombie.h"
#include "Obstacle.h"

class GameEntity;
class Zombie;

class Scene
{
	public:
		Scene(void);
		~Scene(void);

		void Update(double delta_time);
		void Draw(void);
		void Key(unsigned char key);
		void AddObject(GameEntity *entity);
		void RemoveObject(GameEntity *entity);

		const glm::mat4& GetViewMatrix(void) const;

	private:
		glm::mat4 view_matrix;

		// TEST OBJECTS
		Zombie *test_zombie;
		// ~TEST OBJECTS

		vector<GameEntity*> objects;

		friend ostream& operator<<(ostream &o, const Scene &scene);
};

ostream& operator<<(ostream &o, const Scene &scene);

inline const glm::mat4& Scene::GetViewMatrix(void) const
{
	return view_matrix;
}

#endif
