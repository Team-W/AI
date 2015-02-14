#ifndef HEADERS_H
#define HEADERS_H

// Definitions
#define GLM_FORCE_RADIANS
#define PI 3.14159265358979323846

#define ZOMBIE_MAX_SPEED 50.0f
#define ZOMBIE_MAX_FORCE 33.3f;
#define ZOMBIE_TURNING_MAX_SPEED 3.0f
#define MIN_DETECTION_BOX_LENGTH 3.6f
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