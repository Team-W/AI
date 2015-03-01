#pragma once
#include "Headers.h"
#include "Utilities.h"
#include "GameEntity.h"
#include "Scene.h"
#include "GraphicDebug.h"

class Scene;

class Player :
	public GameEntity
{
public:
	Player(Scene *s, float x, float y);
	~Player();

	void Update(double delta_time);
	void Draw();
	void RailPhysics();
	void Shoot(glm::vec2 fire);
	void Move(glm::vec2 move, double delta_time);
	void Rotate(glm::vec2 heading); // heading = mouse pos
	void Reset();
	void Respawn();
	void RandomPosition();
	void RandomPoint();

	

private:
	GraphicDebug *mouse;
	GraphicDebug *rail;
	glm::vec3 color;
	glm::vec2 shooting_pos;
	glm::vec2 shooting_target;
	double speed;
	bool CDrail;

	float immortality_duration;
	int score;
	int rail_ammo;
	int machine_ammo;
	int lifes;
	int cash;

	friend class Scene;
	friend class PowerUp;
};

inline void Player::Reset()
{
	object_position = glm::vec2(0, 0);
	immortality_duration = 0.0f;
	score = 0;
	rail_ammo = 10;
	machine_ammo = 0;
	lifes = 3;
	cash = 0;
}