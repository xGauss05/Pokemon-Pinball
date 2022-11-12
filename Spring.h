#pragma once
#include "Application.h"
#include "Module.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

class ModulePhysics;
class SDL_Texture;

class Spring : Prop {
public:
	Spring() : Prop(type) {

	}

	void Blit() {
		//App->renderer->Blit(NULL);
	}

	void PlaySFX() {
		//App->audio->PlayFx(NULL);
	}

private:
	int x;
	int y;

	PhysBody* pBody;

	SDL_Texture* texture;
};