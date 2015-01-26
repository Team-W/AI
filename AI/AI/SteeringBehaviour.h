#ifndef STEERING_BEHAVIOUR_H
#define STEERING_BEHAVIOUR_H

#include "Headers.h"
#include "Utilities.h"
#include "GameEntity.h"
#include "Zombie.h"
#include "Obstacle.h"
#include "GraphicDebug.h"

class Zombie;

class SteeringBehaviour
{
	public:
		SteeringBehaviour(Zombie *z);
		~SteeringBehaviour(void);

		const glm::vec2& GetSteeringForce(void) const;

		void Draw(void);

		void ToggleWander(void);
		void ToggleSeek(void);
		void ToggleFlee(void);
		void ToggleArrive(void);
		void ToggleObstacleAvoidance(void);

	private:
		Zombie *owner;

		// ---------- PARAMS ---------- //
		float wander_radius;
		float wander_distance;
		float wander_jitter;
		glm::vec2 wander_target;
		GraphicDebug wander_target_point;
		GraphicDebug obstacle_position[20];
		GraphicDebug obstacle_x_axis;
		GraphicDebug obstacle_y_axis;
		GraphicDebug obstacle_box;
		int obstacle_number;
		// ----------------------------- //

		glm::vec2 steering_force;

		bool wander_on;
		bool seek_on;
		bool flee_on;
		bool arrive_on;
		bool obstacle_avoidance_on;

		glm::vec2 CalculateSteeringForce(void);
		glm::vec2 CalculateWander(void);
		glm::vec2 CalculateSeek(const glm::vec2 &target);
		glm::vec2 CalculateFlee(void);
		glm::vec2 CalculateArrive(void);
		glm::vec2 CalculateObstacleAvoidance(void);

		friend class Zombie;
		friend ostream& operator<<(ostream &o, const SteeringBehaviour &sb);
};

ostream& operator<<(ostream &o, const SteeringBehaviour &sb);

inline const glm::vec2& SteeringBehaviour::GetSteeringForce(void) const
{
	return steering_force;
}

inline void SteeringBehaviour::ToggleWander(void)
{
	wander_on = !wander_on;
}

inline void SteeringBehaviour::ToggleSeek(void)
{
	seek_on = !seek_on;
}

inline void SteeringBehaviour::ToggleFlee(void)
{
	flee_on = !flee_on;
}

inline void SteeringBehaviour::ToggleArrive(void)
{
	arrive_on = !arrive_on;
}

inline void SteeringBehaviour::ToggleObstacleAvoidance(void)
{
	obstacle_avoidance_on = !obstacle_avoidance_on;
}

#endif
