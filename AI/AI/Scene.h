#ifndef SCENE_H
#define SCENE_H

#include "Headers.h"
#include "GameEntity.h"
#include "Zombie.h"
#include "Obstacle.h"
#include "Player.h"
#include "PowerUp.h"
#include "BulletBufferObject.h"

class GameEntity;
class Zombie;
class Player;
class Obstacle;
class PowerUp;
class BulletBufferObject;

class Scene
{
	public:
		Scene(void);
		~Scene(void);

		void Init(GLuint *texture);

		void Update(double delta_time);
		void Draw(void);
		void Restart(void);
		void KeyState(unsigned char key, bool tf);
		void AddObject(GameEntity *entity);
		void AddZombie(Zombie *entity);
		void AddObstacle(Obstacle *entity);
		void AddPowerUp(PowerUp *powerup);
		
		void PlayerRotate(glm::vec2 heading);
		void PlayerMove(glm::vec2 move, double delta_time);
		void PlayerShoot(glm::vec2 aim);
			
		void PrintResult();
		void PrintPlayerData();
		void DisplayData();
		string GetPlayerData();

		bool CheckVictoryCondition(void);

		const glm::mat4& GetViewMatrix(void) const;

	private:
		glm::mat4 view_matrix;
		bool KeyStates[256];
		bool debug;
		bool game;

		int score;
		GLuint *texture;
		// TEST OBJECTS
		Zombie *test_zombie;
		// ~TEST OBJECTS
		
		GraphicDebug *mouse;
		Player *player;

		vector<Zombie*> zombies;
		vector<Obstacle*> obstacles;
		vector<PowerUp*> powerups;
		vector<GameEntity*> objects;

		BulletBufferObject *bbo;

		friend class Player;
		friend class Zombie;
		friend class SteeringBehaviour;
		friend class PowerUp;
		friend class BulletBufferObject;
		friend ostream& operator<<(ostream &o, const Scene &scene);
};

ostream& operator<<(ostream &o, const Scene &scene);

inline const glm::mat4& Scene::GetViewMatrix(void) const
{
	return view_matrix;
}



#endif
