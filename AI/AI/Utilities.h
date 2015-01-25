#ifndef UTILITIES_H
#define UTILITIES_H

#include "Headers.h"

// Display Functions
ostream& operator<<(ostream &o, const glm::vec2 &vector);
ostream& operator<<(ostream &o, const glm::mat4 &matrix);

// Matrix Operations
void MatrixToArray(GLfloat array[16], const glm::mat4 &model_matrix, const glm::mat4 &view_matrix);

// Vector Operations
inline int GetSign(glm::vec2 &a, glm::vec2 &b)
{
	return (a.y*b.x > a.x*b.y) ? -1 : 1;
}

inline float GetLength(glm::vec2 &vector)
{
	return sqrt(vector.x*vector.x + vector.y*vector.y);
}

inline float GetLengthSqrt(glm::vec2 &vector)
{
	return (vector.x*vector.x + vector.y*vector.y);
}

inline float GetDistance(glm::vec2 &a, glm::vec2 &b)
{
	double xd = b.x - a.x;
	double yd = b.y - a.y;

	return (float)sqrt(xd*xd + yd*yd);
}

inline float GetDistanceSqrt(glm::vec2 &a, glm::vec2 &b)
{
	double xd = b.x - a.x;
	double yd = b.y - a.y;

	return (float)(xd*xd + yd*yd);
}

inline float Dot(glm::vec2 &a, glm::vec2 &b)
{
	return a.x*b.x + a.y*b.y;
}

inline float GetAngle(glm::vec2 &a, glm::vec2 &b)
{
	return GetSign(a, b) * acos(Dot(a, b));
}

inline float DegreesToRadians(float degrees)
{
	return (float)(degrees * (180.0f / PI));
}

inline void Normalize(glm::vec2 &vector)
{
	float l = GetLength(vector);

	if(l > 0.00000001)
	{
		vector.x /= l;
		vector.y /= l;
	}
}

inline void SetLength(glm::vec2 &vector, float d)
{
	Normalize(vector);
	vector.x *= d;
	vector.y *= d;
}

inline void Truncate(glm::vec2 &vector, float f)
{
	if(GetLength(vector) > f)
		SetLength(vector, f);
}

inline glm::vec2 GetReversed(glm::vec2 &vector)
{
	return glm::vec2(-vector.x, -vector.y);
}

inline glm::vec2 GetPerpendicular(glm::vec2 &vector)
{
	return glm::vec2(-vector.y, vector.x);
}

#endif