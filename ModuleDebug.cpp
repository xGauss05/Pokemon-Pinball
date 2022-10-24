#include "Debug.h"

#include "App.h"
#include "Input.h"
#include "Render.h"

#include "Player.h"
#include "Render.h"
#include "Fonts.h"
#include "EntityManager.h"
#include <string>
using namespace std;

Debug::Debug() : Module() {
	debug = false;
}
Debug::~Debug() {
}

bool Debug::Start() {
	debug = false;
	return true;
}

bool Debug::Update(float dt) {

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		debug = !debug;

	if (debug) {
		if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
			variables = !variables;
	}

	return true;
}

bool Debug::PostUpdate() {

	if (debug)
	{
		DebugDraw();
	}


	return true;
}

void Debug::DebugDraw() {
	
	app->font->BlitText(10, + 20, 0, "PRESS V FOR VARIABLES");

	//Variables debug
	if (variables) {

		/*app->font->BlitText(10, debugBox + 10, 0, "-FREE CAM");
		if (!app->render->debugCamera)
			app->font->BlitText(90, debugBox + 10, 0, "OFF");
		else
			app->font->BlitText(90, debugBox + 10, 0, "ON");*/

		//Position debug
		app->font->BlitText(10, varBox + 30, 0, "PLAYER");

		app->font->BlitText(10, varBox + 40, 0, "X.");
		app->font->BlitText(25, varBox + 40, 0, std::to_string(app->entityManager->entities.At(0)->data->position.x).c_str());

		app->font->BlitText(10, varBox + 50, 0, "Y.");
		app->font->BlitText(25, varBox + 50, 0, std::to_string(app->entityManager->entities.At(0)->data->position.y).c_str());

		app->font->BlitText(10, varBox + 60, 0, "CAMERA X.");
		app->font->BlitText(80, varBox + 60, 0, std::to_string(app->render->camera.x).c_str());

		app->font->BlitText(10, varBox + 70, 0, "CAMERA Y.");
		app->font->BlitText(80, varBox + 70, 0, std::to_string(app->render->camera.y).c_str());
	}
}