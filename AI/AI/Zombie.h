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
        Zombie(Scene *s, float x, float y);
        ~Zombie(void);

        void Update(double delta_time);
		void RandomPoint();
		void MousePoint(glm::vec2 target);
		void gotHit();
		void Draw();
		void Group();

    private:
        SteeringBehaviour *steering_behaviour;
		bool aggressive;
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
