#ifndef __MODULE_DEBUG_H__
#define __MODULE_DEBUG_H__

#include "Module.h"

#define DEBUG_BOX 225

class Debug : public Module {
public:
	// Constructor
	Debug();

	// Destructor
	~Debug();

	bool Start() override;

	// Called at the middle of the application loop
	// Switches the debug mode on/off
	bool Update(float dt);

	// Called at the end of the application loop
	// Draw all functionality (if debug mode is enabled)
	bool PostUpdate();

	// Draws all existing colliders with some transparency
	void DebugDraw();

private:

	// Simple debugging flag
	bool debug;

	int varBox = 50;

	bool variables = false;
};

#endif // __MODULE_DEBUG_H__
