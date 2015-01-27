#include "GraphicDebug.h"

GraphicDebug::GraphicDebug(): radius(0)
{
	begin = glm::vec2(0, 0);
	end = glm::vec2(0, 0);
	color = glm::vec3(0, 0, 0);
	scale = glm::vec2(0.03f, 0.03f);
	hidden = false;
};

GraphicDebug::~GraphicDebug(void)
{

}

void GraphicDebug::InitPoint(glm::vec2 begin, double r, glm::vec3 color)
{
	this->begin = begin;
	this->radius = r;
	this->color = color;
}

void GraphicDebug::InitLine(glm::vec2 begin, glm::vec2 end, glm::vec3 color)
{
	this->begin = begin;
	this->end = end;
	this->color = color;
}

void GraphicDebug::InitRectangle(glm::vec2 begin, glm::vec2 end, glm::vec3 color)
{
	this->begin = begin;
	this->end = end;
	this->color = color;
	scale = glm::vec2(1.0f, 1.0f);
}

void GraphicDebug::UpdatePoint(glm::vec2 begin, double r, glm::vec3 color)
{	
	if(!this->hidden)
	{
		this->begin = begin;
		this->color = color;
		this->radius = r;
	}else{
		this->begin = glm::vec2(100, 100);
	}
}

void GraphicDebug::UpdateLine(glm::vec2 begin, glm::vec2 end, glm::vec3 color)
{
	if(!this->hidden)
	{
		this->begin = begin;
		this->end = end;
		this->color = color;
	}else{
		this->begin = glm::vec2(100, 100);
		this->end = glm::vec2(100, 100);
	}
};

void GraphicDebug::UpdateRectangle(glm::vec2 begin, glm::vec2 end, glm::vec3 color)
{
	if(!this->hidden)
	{
		this->begin = glm::vec2(0,0);
		this->end = end;
		this->color = color;
		this->heading = heading;
	}else{
		this->begin = glm::vec2(100, 100);
	}
};

void GraphicDebug::DrawPoint()
{
	float a, b;
	glPushMatrix();
	glScalef(scale.x, scale.y, 1.0f);
	glTranslatef(begin.x, begin.y, 0.0);
	glBegin(GL_LINES);

	glColor3f(color.x, color.y, color.z);

	a = (float)radius * (float)cos(359 * PI / 180.0f);
	b = (float)radius * (float)sin(359 * PI / 180.0f);
	for(int j = 0; j < 360; j++)
	{
		glVertex2f(a, b);
		a = (float)radius * (float)cos(j * PI / 180.0f);
		b = (float)radius * (float)sin(j * PI / 180.0f);
		glVertex2f(a, b);
	}

	glEnd();
	glPopMatrix();
}

void GraphicDebug::DrawLine()
{
	glPushMatrix();
		glScalef(scale.x, scale.y, 1.0f);
		glBegin(GL_LINES);
			glColor3f(color.x, color.y, color.z);
			glVertex2f(begin.x, begin.y);
			glVertex2f(end.x, end.y);
		glEnd();
	glPopMatrix();
}

void GraphicDebug::DrawRectngle()
{
	glPushMatrix();
		glScalef(scale.x, scale.y, 1.0f);
		glBegin(GL_LINES);
			glColor3f(color.x, color.y, color.z);
			if (!hidden){
				glVertex2f(begin.x - end.x, begin.x - 1.20f);
				glVertex2f(begin.x + end.x, begin.x - 1.20f);
				glVertex2f(begin.x + end.x, begin.x - 1.20f);
				glVertex2f(begin.x + end.x, begin.x + end.y);
				glVertex2f(begin.x + end.x, begin.x + end.y);
				glVertex2f(begin.x - end.x, begin.x + end.y);
				glVertex2f(begin.x - end.x, begin.x + end.y);
				glVertex2f(begin.x - end.x, begin.x - 1.20f);
			}
		glEnd();
	glPopMatrix();
}

void GraphicDebug::Hide()
{
	this->hidden = !this->hidden;
}
glm::vec2 GraphicDebug::getPosition(){
	return this->begin;
}