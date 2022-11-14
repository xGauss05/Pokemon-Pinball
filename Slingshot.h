#pragma once
#include "Application.h"
#include "Module.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "Animation.h"

class ModulePhysics;
class SDL_Texture;

enum SlingPlace {
	LEFT,
	RIGHT
};

class Slingshot : public Prop {
public:
	Slingshot(PropType type, SlingPlace sPlace) : Prop(type) {

		switch (sPlace)
		{
		case SlingPlace::LEFT:
			x = 98;
			y = 142;
			pBody = App->physics->CreateChain(x, y, pLeft, 3);
			break;
		case SlingPlace::RIGHT:
			x = 134;
			y = 142;
			pBody = App->physics->CreateChain(x, y, pRight, 3);
			break;
		}

		texture = App->textures->Load("pinball/Textures/Assets_Map.png");
		pBody->body->SetType(b2BodyType::b2_staticBody);
		pBody->prop = this;
		pBody->listener = (Module*)App->pManager;
		bumperSfx = App->audio->LoadFx("pinball/Sounds/bumpers.wav");


	}

	

	void Blit() {

	}

	void PlaySFX() {
		App->audio->PlayFx(bumperSfx);
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();
		Blit();

		if (bodyB->prop != NULL) {
			if (bodyB->prop->type == PropType::BALL) {

				bodyB->body->ApplyForceToCenter({  }, true);

			}
		}
	}

	bool Update() {
		
		return true;
	}

	bool PostUpdate() {
		
		return true;
	}

private:
	int x;
	int y;
	int pLeft[6] = {
		67, 343,
		86, 368,
		66, 360 };
	int pRight[6] = { 
		171, 340,
		153, 368,
		171, 359 };

	PhysBody* pBody;

	int bumperSfx;

	SDL_Texture* texture;
};