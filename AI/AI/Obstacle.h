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
        Obstacle(Scene *s, double x, double y, double rad);
        ~Obstacle(void);

        void Update(double delta_time);
		void Draw(void);

    private:
		double object_radius;

        friend ostream& operator<<(ostream &o, const Obstacle &ob);
};

ostream& operator<<(ostream &o, const Obstacle &ob);

#endif
