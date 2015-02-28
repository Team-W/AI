#ifndef POWERUP_H
#define POWERUP_H

#include "GameEntity.h"

class PowerUp: public GameEntity
{
	public:

		enum POWERUP_TYPE
		{
			RAIL_AMMO,
			IMMORTALITY,

		};

		PowerUp();
		~PowerUp();

	private:


};

#endif

