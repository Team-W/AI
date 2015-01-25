#include "GameEntity.h"

int GameEntity::next_id = 0;

GameEntity::GameEntity(void): object_id(next_id++), scene(0), collision_radius(0)
{
	model_matrix	= glm::mat4(1.0f);
	object_scale	= glm::vec2(1.0f);
	object_position = glm::vec2(0.0f);
	object_heading	= glm::vec2(0.0f);
	object_side		= glm::vec2(0.0f);
}

GameEntity::~GameEntity(void)
{

}

