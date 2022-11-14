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

class Trough : public Prop {
public:
	Trough(PropType type) : Prop(type) {

		loseSfx = App->audio->LoadFx("pinball/Sounds/lose.wav");
		x = 120;
		y = SCREEN_HEIGHT + 20;

		pBody = App->physics->CreateRectangleSensor(x, y, 45, 5);
		pBody->listener = (Module*)App->pManager;

		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
		pBody->body->SetType(b2BodyType::b2_staticBody);
		pBody->prop = this;
	}

	void PlaySFX() {

		App->audio->PlayFx(loseSfx);
	}

	bool Update() {
		if (switchLayer) {
			App->scene->switchLayer(2);
			switchLayer = false;
		}
		
		return true;
	}

	void OnCollision(PhysBody* bodyB) {
		if (bodyB->prop != NULL) {
			switch (bodyB->prop->type) {
			case PropType::BALL:
				PlaySFX();
				App->scene->lifes--;
				switchLayer = true;
				break;
			}
		}
	}

private:
	int x;
	int y;

	// SFX
	int loseSfx;
	bool switchLayer = false;
	PhysBody* pBody;
};