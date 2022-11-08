#pragma once
#include "Application.h"
#include "Module.h"
#include "ModulePhysics.h"

class ModulePhysics;

enum FLIPPERSIDE {
	LEFT,
	RIGHT
};

class Flipper {
public:

	

	Flipper(FLIPPERSIDE side) {
		switch (side)
		{
		case LEFT:
			
			break;
		case RIGHT:

			break;
		default:
			break;
		}
	}

private:
	int x;
	int y;

	PhysBody* pBody;
	PhysBody* pin;
	b2RevoluteJoint* joint;
};