#ifndef POWERUP_H
#define POWERUP_H

#include "GameEntity.h"
#include "Scene.h"

class Scene;

class PowerUp: public GameEntity
{
	public:

		enum POWERUP_TYPE
		{
			RAIL_AMMO,
			MACHINE_AMMO,
			IMMORTALITY,
			LIFE,
			CASH
		};

		PowerUp(Scene *s);
		~PowerUp();

		void Update(double delta_time);
		void Draw(void);
		void RandomPoint();
		void RandomPosition();
		void Respawn();
		void Despawn();

	private:
		POWERUP_TYPE type;
		int value;
		float spawn_timer;
		string debug_string;
		bool spawned;

		glm::vec3 color;

		friend class Scene;
		friend class Player;
};

#endif

