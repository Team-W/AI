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

	enum WEAPON_TYPE
	{
		RAIL,
		MACHINE
	} current_weapon;

	Player(Scene *s, GLuint texture, float x, float y);
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
	string GetPlayerData();
	void UpgradeWeapon(Player::WEAPON_TYPE type);
	bool CheckValidPosition(const glm::vec2 &position);

private:
	GraphicDebug *mouse;
	GraphicDebug *rail;
	glm::vec3 color;
	glm::vec2 shooting_pos;
	glm::vec2 shooting_target;
	float machine_timer;
	double speed;
	bool CDrail;
	bool CDmachine;

	float immortality_duration;
	int score;
	int rail_ammo;
	int machine_ammo;
	int lifes;
	int cash;
	int rail_lvl;
	int machine_lvl;

	friend class Scene;
	friend class PowerUp;
	friend class BulletBufferObject;
};

inline void Player::Reset()
{
	object_position = glm::vec2(0, 0);
	immortality_duration = 0.0f;
	score = 0;
	rail_ammo = PLAYER_STARTING_RAIL_AMMO;
	machine_ammo = PLAYER_STARTING_MACHINE_AMMO;
	lifes = PLAYER_STARTING_LIFES; //ADD Player HP
	cash = 0;
	rail_lvl = 1;
	machine_lvl = 1;
}

inline string Player::GetPlayerData()
{
	stringstream ss;

	ss << "Lifes: " << lifes << "    Curent weapon: " << (current_weapon == RAIL ? "railgun" : "machine gun") << "   Railgun[" << rail_ammo << "] Machine gun[" << machine_ammo << "]";

	return ss.str();
}
