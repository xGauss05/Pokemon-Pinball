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
	SLEFT,
	SRIGHT
};

class Slingshot : public Prop {
public:
	Slingshot(PropType type, SlingPlace sPlace) : Prop(type) {

		place = sPlace;

		switch (sPlace)
		{
		case SlingPlace::SLEFT:
			pBody2 = App->physics->CreateChain(0, 0, pLeft, 6);
			pBody = App->physics->CreateRectangle(78, 357, 1, 36);
			pBody->body->SetTransform(pBody->body->GetPosition(),-0.6);
			break;
		case SlingPlace::SRIGHT:
			pBody2 = App->physics->CreateChain(0, 0, pRight, 6);
			pBody = App->physics->CreateRectangle(160, 354, 1, 36);
			pBody->body->SetTransform({ pBody->body->GetPosition() }, 0.6);
			break;
		}
		force = 30;

		pBody->body->SetType(b2BodyType::b2_staticBody);
		pBody2->body->SetType(b2BodyType::b2_staticBody);
		pBody->prop = this;
		pBody->listener = (Module*)App->pManager;
		texture = App->textures->Load("pinball/Textures/Assets_Map.png");
		bumperSfx = App->audio->LoadFx("pinball/Sounds/bumpers.wav");


	}

	

	void Blit() {
		SDL_Rect left;
		SDL_Rect right;
		switch (place)
		{
		case SlingPlace::SLEFT:
			left = { 0, 59, 23, 34 };
			App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x), METERS_TO_PIXELS(pBody->body->GetPosition().y), &left);
			break;
		case SlingPlace::SRIGHT:
			right = { 25, 59, 23, 34 };
			App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x), METERS_TO_PIXELS(pBody->body->GetPosition().y), &right);
			break;
		}
	}

	void PlaySFX() {
		App->audio->PlayFx(bumperSfx);
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();
		Blit();

		if (bodyB->prop != NULL) {
			if (bodyB->prop->type == PropType::BALL) {

				switch (place)
				{
					case SlingPlace::SLEFT:
						bodyB->body->ApplyForceToCenter({ (float32)(force* 0.866), (float32)(-force*0.64) }, true);
						break;
					case SlingPlace::SRIGHT:
						bodyB->body->ApplyForceToCenter({ (float32)(force*(-0.866)), (float32)(-force * 0.64) }, true);
						break;
				}
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
	int pLeft[6] = {
		66, 360,
		66, 340,
		86, 370};
	int pRight[6] = { 
		151, 370,
		171, 340,
		171, 360 };

	int force;

	PhysBody* pBody;
	PhysBody* pBody2;

	int bumperSfx;

	SlingPlace place;

	SDL_Texture* texture;
};