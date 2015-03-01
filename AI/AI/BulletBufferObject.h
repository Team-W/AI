#ifndef BULLET_BUFFER_OBJECT
#define BULLET_BUFFER_OBJECT

#include "Headers.h"
#include "Scene.h"

class Scene;

class BulletBufferObject
{
	public:
		BulletBufferObject(Scene *s, unsigned int size);
		~BulletBufferObject();

		void AddBullet(const glm::vec2 &position, const glm::vec2 &heading);
		void Update(double delta_time);
		void Draw();

	private:
		unsigned int buffer_size;
		unsigned int current_index;

		Scene *scene;
		float bullet_max_speed;
		glm::vec2 bullet_scale;
		glm::vec3 bullet_color;
		glm::mat4 model_matrix;

		vector<glm::vec2> bullet_position;
		vector<glm::vec2> bullet_velocity;
		vector<float> bullet_angle;
		vector<bool> used_bullets;
};

#endif

