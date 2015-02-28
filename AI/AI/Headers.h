#ifndef HEADERS_H
#define HEADERS_H

// Definitions
#define GLM_FORCE_RADIANS
#define PI 3.14159265358979323846

#define ZOMBIE_MAX_SPEED 120.0f
#define ZOMBIE_MAX_FORCE 15000.0f
#define ZOMBIE_RESPAWN_TIMER 1.0f
#define ZOMBIE_GROUP 4

#define MIN_DETECTION_BOX_LENGTH 3.0f
#define PLAYER_SPEED 100.0f
#define CASH_PER_ZOMBIE 2
#define RAIL_TIMER 0.033f

#define POWERUP_RESPAWN_TIME 3.0f
#define POWERUP_LIFE_VALUE 1
#define POWERUP_IMMORTALITY_VALUE 1
#define POWERUP_MACHINE_AMMO_VALUE 20
#define POWERUP_RAIL_AMMO_VALUE 5
#define POWERUP_CASH_VALUE 10

// Standard Libraries
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>

// Graphic Libraries
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

// Misc
using namespace std;

#endif