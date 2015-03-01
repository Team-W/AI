#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "Headers.h"

class Scene;
class SteeringBehaviours;

class GameEntity
{
    public:
        GameEntity(void);
        virtual ~GameEntity(void);

        virtual void Update(double delta_time) = 0;
		virtual void Draw(void) = 0;

		int GetObjectID(void) const;
		const glm::mat4& GetModelMatrix(void) const;
		const glm::vec2& GetObjectScale(void) const;
		const glm::vec2& GetObjectPosition(void) const;
		const glm::vec2& GetObjectHeading(void) const;
		const glm::vec2& GetObjectSide(void) const;
		float GetCollisionRadius(void) const;

    protected:
		static int next_id;
        const int object_id;
		Scene *scene;

		GLuint texture[1];
		glm::mat4 model_matrix;
		glm::vec2 object_scale;
		glm::vec2 object_position;
		glm::vec2 object_heading;
		glm::vec2 object_side;

        float collision_radius;
		friend class PowerUp;
};

inline int GameEntity::GetObjectID(void) const
{
	return object_id;
}

inline const glm::mat4& GameEntity::GetModelMatrix(void) const
{
	return model_matrix;
}

inline const glm::vec2& GameEntity::GetObjectScale(void) const
{
	return object_scale;
}

inline const glm::vec2& GameEntity::GetObjectPosition(void) const
{
	return object_position;
}

inline const glm::vec2& GameEntity::GetObjectHeading(void) const
{
	return object_heading;
}

inline const glm::vec2& GameEntity::GetObjectSide(void) const
{
	return object_side;
}

inline float GameEntity::GetCollisionRadius(void) const
{
	return collision_radius;
}

#endif
