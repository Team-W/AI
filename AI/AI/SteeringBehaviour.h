#ifndef STEERING_BEHAVIOUR_H
#define STEERING_BEHAVIOUR_H

#include "Headers.h"
#include "Utilities.h"
#include "Zombie.h"

class Zombie;

class SteeringBehaviour
{
	public:
		SteeringBehaviour(Zombie *z);
		~SteeringBehaviour(void);

	private:
		Zombie *owner;

		friend ostream& operator<<(ostream &o, const SteeringBehaviour &sb);
};

ostream& operator<<(ostream &o, const SteeringBehaviour &sb);

#endif
