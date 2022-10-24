#include "ModuleDebug.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include "ModulePlayer.h"
#include "ModuleFonts.h"
#include <string>
using namespace std;

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(app, start_enabled) {
	debug = false;
}
ModuleDebug::~ModuleDebug() {
}

bool ModuleDebug::Start() {
	debug = false;
	return true;
}

update_status ModuleDebug::Update() {

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		debug = !debug;

	if (debug) {
		if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
			variables = !variables;
	}

	App->fonts->BlitText(20, 30, 0, "UPDATE TEST");

	return UPDATE_CONTINUE;
}

update_status ModuleDebug::PostUpdate() {
	
	App->fonts->BlitText(20, 40, 0, "POSTUPDATE TEST");

	if (debug)
	{
		DebugDraw();
	}
	return UPDATE_CONTINUE;
}

void ModuleDebug::DebugDraw() {
	
	App->fonts->BlitText(10, 30, 0, "PRESS V FOR VARIABLES");

	//Variables debug
	if (variables) 
	{
		App->fonts->BlitText(10, 40, 0, "WORKING!");
	}
}