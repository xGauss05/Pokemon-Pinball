#ifndef __MODULE_DEBUG_H__
#define __MODULE_DEBUG_H__

#include "Module.h"
#include <chrono>
using namespace std::chrono;

enum class Screen
{
	HOME,
	TIME,
	GRAVITY,
	COLLIDERS,
	SPRITES,
	VARIABLES,
	NONE
};

class ModuleDebug : public Module {
public:
	// Constructor
	ModuleDebug(Application* app, bool start_enabled = true);

	// Destructor
	~ModuleDebug();

	bool Start() override;

	// Called at the middle of the application loop
	// Switches the debug mode on/off
	update_status Update();

	// Called at the end of the application loop
	// Draw all functionality (if debug mode is enabled)
	update_status PostUpdate();

	// Draws all existing colliders with some transparency
	void DebugDraw();

public:
	microseconds elapsedCycle;
	microseconds elapsedFrame;
	int targetFPS = 60;
	double FPS;

private:

	Screen currentScreen = Screen::HOME;

	// Simple debugging flag
	bool debug;

	bool time = false;
	bool gravity = false;
	bool colliders = false;
	bool sprites = false;
	bool variables = false;
};

#endif // __MODULE_DEBUG_H__
