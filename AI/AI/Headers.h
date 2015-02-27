#ifndef HEADERS_H
#define HEADERS_H

// Definitions
#define GLM_FORCE_RADIANS
#define PI 3.14159265358979323846

#define ZOMBIE_MAX_SPEED 120.0f
#define ZOMBIE_MAX_FORCE 15000.0f
#define ZOMBIE_RESPAWN_TIMER 3.0f
#define ZOMBIE_GROUP 4

#define MIN_DETECTION_BOX_LENGTH 3.0f
#define PLAYER_SPEED 100.0f


// Standard Libraries
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

// Graphic Libraries
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

// Misc
using namespace std;

#endif