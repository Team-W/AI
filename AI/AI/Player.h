#pragma once
#include "Headers.h"
#include "Utilities.h"
#include "GameEntity.h"
#include "Scene.h"
#include "GraphicDebug.h"

class Player :
	public GameEntity
{
public:
	Player(Scene *s, float x, float y);
	~Player();

	void Update(double delta_time);
	void Draw();
	void Shoot(glm::vec2 fire);
	void Move(glm::vec2 move, double delta_time);
	void Rotate(glm::vec2 heading); // heading = mouse pos

private:
	GraphicDebug *mouse;
	GraphicDebug *rail;
	double speed;
	bool CDrail;
};

