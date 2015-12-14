#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Headers.h"
#include "Utilities.h"
#include "GameEntity.h"
#include "SteeringBehaviour.h"
#include "Scene.h"
#include "GraphicDebug.h"

class Scene;
class SteeringBehaviour;

class Zombie: public GameEntity
{
    public:
        Zombie(Scene *s, GLuint texture);
        ~Zombie(void);

        void Update(double delta_time);
		void RandomPoint();
		void RandomPosition();
		void MousePoint(glm::vec2 target);
		void gotHit();
		void Draw();
		void Group();
		void Respawn(double delta_time);	
		void Retaliate(double delta_time);
		int Attack();

    private:
        SteeringBehaviour *steering_behaviour;

		bool aggressive;
		bool attacked;
		bool dead;

		double respawn_timer;
		double attack_timer;

		int damage;

		glm::vec3 color;
		glm::vec2 target_position;
		glm::vec2 object_velocity;

		GraphicDebug heading_vector;
		GraphicDebug target_point;
		GraphicDebug obstacle_avoidance;

		friend class SteeringBehaviour;
        friend ostream& operator<<(ostream &o, const Zombie &z);
		friend class Scene;
};

ostream& operator<<(ostream &o, const Zombie &z);

#endif
