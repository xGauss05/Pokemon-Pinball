#pragma once
#include "Application.h"
#include "Module.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"

class ModulePhysics;
class SDL_Texture;

class Ball : Prop {
public:
	Ball() : Prop(type) {
		texture = App->textures->Load("pinball/Textures/temp_flipper.png");

		pBody = App->physics->CreateCircle(x, y, radius);
	}

	void Blit() {
		App->renderer->Blit(texture, x, y);
	}

private:
	int x;
	int y;
	int radius;

	PhysBody* pBody;
	
	SDL_Texture* texture;
};