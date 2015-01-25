#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "Headers.h"

class Scene;

class GameEntity
{
    public:
        GameEntity(void);
        virtual ~GameEntity(void);

        virtual void Update(double delta_time) = 0;
		virtual void Draw(void) = 0;

    protected:
		static int next_id;
        const int object_id;
		Scene *scene;

		glm::mat4 model_matrix;
		glm::vec2 object_scale;
		glm::vec2 object_position;
		glm::vec2 object_heading;
		glm::vec2 object_side;

        float collision_radius;

		friend class SteeringBehaviours;
};

#endif
