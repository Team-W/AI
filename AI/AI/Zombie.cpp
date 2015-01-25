#include "Zombie.h"

Zombie::Zombie(Scene *s, float x, float y)
{
	this->steering_behaviour = new SteeringBehaviour(this);
	this->scene = s;
	this->object_position = glm::vec2(x, y);
	this->object_velocity = glm::vec2(0.0f, 0.0f);
	this->target_position = glm::vec2(0.0f, 0.0f);
	this->object_scale = glm::vec2(0.03f, 0.03f);
	RandomPoint();

	heading_vector.InitLine(object_position, object_heading + object_position, glm::vec3(0.7f, 0.7f, 0.7f));
	target_point.InitPoint(target_position, 0.2, glm::vec3(1, 0, 0));
}

Zombie::~Zombie(void)
{
	if(steering_behaviour != 0)
		delete steering_behaviour;
}

void Zombie::Update(double delta_time)
{
	heading_vector.UpdateLine(object_position, object_heading + object_position, glm::vec3(0.7f, 0.7f, 0.7f));
	target_point.UpdatePoint(target_position, glm::vec3(1, 0, 0));

	glm::vec2 steering_force = target_position - object_position;

	object_velocity = steering_force;
	SetLength(object_velocity, 100);

	object_position += object_velocity * glm::vec2(delta_time, delta_time);

	model_matrix = glm::mat4(1.0f);
	model_matrix = glm::scale(model_matrix, glm::vec3(object_scale, 1.0f));
	model_matrix = glm::translate(model_matrix, glm::vec3(object_position, 0.0f));

	if(GetLength(object_velocity) > 0.00000001)
	{
		object_heading = object_velocity;
		Normalize(object_heading);
		object_side = GetPerpendicular(object_heading);
	}

	model_matrix = glm::rotate(model_matrix, GetAngle(glm::vec2(0, 1), object_heading), glm::vec3(0, 0, 1));
}

void Zombie::RandomPoint()
{
	int sign1 = (((float)rand()/RAND_MAX - 0.5)>0) ? 1:-1;
	int sign2 = (((float)rand()/RAND_MAX - 0.5)>0) ? 1:-1;
	target_position = glm::vec2(sign1 * rand()%20, sign2 * rand()%20);
}

void Zombie::Draw()
{
	float a, b;
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glPushMatrix();
		GLfloat Matrix[16];
		MatrixToArray(Matrix, model_matrix, scene->GetViewMatrix());
		glLoadMatrixf(Matrix);

		glBegin(GL_TRIANGLES);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(-0.50f, -0.60f, 0.f);
			glVertex3f(0.50f, -0.60f, 0.f);
			glVertex3f(0.00f, 0.75f, 0.f);
		glEnd();

		glBegin(GL_LINES);
			glColor3f(0.5f, 0.0f, 0.0f);

			a = 1.0f * (float)cos(359 * PI / 180.0f);
			b = 1.0f * (float)sin(359 * PI / 180.0f);
			for (int j = 0; j < 360; j++)
			{
				glVertex2f(a, b);
				a = 1.0f * (float)cos(j * PI / 180.0f);
				b = 1.0f * (float)sin(j * PI / 180.0f);
				glVertex2f(a, b);
			}
		glEnd();
	glPopMatrix();

	heading_vector.DrawLine();
	target_point.DrawPoint();

	glDisable(GL_LINE_SMOOTH);
}

ostream& operator<<(ostream &o, const Zombie &z)
{
	o.setf(ios::fixed, ios::floatfield);
	o.precision(4);
    o << "--------------- ZOMBIE ---------------" << "\n";
	o << "Position:\t["  << z.object_position.x << ", " << z.object_position.y << "]\n";
	o << "Target:\t\t["  << z.target_position.x << ", " << z.target_position.y << "]\n";
	o << "Velocity:\t["  << z.object_velocity.x << ", " << z.object_velocity.y << "]\n";
	o << "Heading:\t["   << z.object_heading.x	<< ", " << z.object_heading.y	<< "]\n";
    o << "--------------------------------------" << endl;
	o.unsetf(ios::floatfield);
    return o;
}
