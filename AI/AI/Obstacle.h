#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Headers.h"
#include "Utilities.h"
#include "GameEntity.h"
#include "Scene.h"

class Scene;

class Obstacle: public GameEntity
{
    public:
		Obstacle(Scene *s, GLuint texture, double x, double y, double rad);
        ~Obstacle(void);

        void Update(double delta_time);
		void Draw(void);
		void setColor(glm::vec3 color);

    private:
		double object_radius;
		glm::vec3 color;
		friend class SteeringBehaviours;
        friend ostream& operator<<(ostream &o, const Obstacle &ob);
};

ostream& operator<<(ostream &o, const Obstacle &ob);

#endif
