#ifndef GRAPHICDEBUG_H
#define GRAPHICDEBUG_H

#include "Headers.h"

class GraphicDebug
{
	public:
		GraphicDebug(void);
		~GraphicDebug(void);

		void InitPoint(glm::vec2 begin, double r, glm::vec3 color);
		void InitLine(glm::vec2 begin, glm::vec2 end, glm::vec3 color);
		void InitRectangle(glm::vec2 begin, glm::vec2 end, glm::vec2 scale, glm::vec3 color);
		void UpdatePoint(glm::vec2 begin, double r, glm::vec3 color);
		void UpdateLine(glm::vec2 begin, glm::vec2 end, glm::vec3 color);
		void UpdateRectangle(glm::vec2 begin, glm::vec2 end, glm::vec3 color);
		void DrawPoint();
		void DrawLine();
		void DrawRectngle();
		void Hide();
		
		glm::vec2 getPosition();

	private:
		bool hidden;
		double radius;
		glm::vec3 color;
		glm::vec2 scale;
		glm::vec2 begin;
		glm::vec2 end;
};

#endif
