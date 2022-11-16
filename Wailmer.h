#pragma once
#pragma once
#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"

class ModulePhysics;
class SDL_Texture;

class Wailmer : public Prop {
public:
	Wailmer(PropType type) : Prop(type) {
		spitSfx = App->audio->LoadFx("pinball/Sounds/wailmer_spit.wav");
		swallowSfx = App->audio->LoadFx("pinball/Sounds/wailmer_swallow.wav");

		position = iPoint(180, 195);

		pBody = App->physics->CreateRectangleSensor(position.x, position.y, 5, 5);
		pBody->listener = (Module*)App->pManager;

		pBody->body->SetType(b2BodyType::b2_staticBody);
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0.0f);
		pBody->prop = this;
		hasSwallowed = false;
	}

	void PlaySFX() {
		if (hasSwallowed) {
			App->audio->PlayFx(spitSfx);
		}
		else {
			App->audio->PlayFx(swallowSfx);
		}
	}

	bool Update() {
		return true;
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();
		hasSwallowed = true;
	}

	void EndColission(PhysBody* bodyB) {
		PlaySFX();
		hasSwallowed = false;
	}

	void EndCollision(PhysBody* otherBody) {
		if (otherBody->prop != NULL) {
			switch (otherBody->prop->type) {
			case PropType::SENSOR_SPRING_IN:
				LOG("Ball END collided SPRING");
				release = true;
				break;
			}
		}
	}

	bool CleanUp() {
		delete pBody;
		pBody = nullptr;

		return true;
	}


private:

	// SFX
	int spitSfx;
	int swallowSfx;

	// Spawn position
	iPoint position;

	bool hasSwallowed, release;
	PhysBody* pBody;
	
};