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
			pBody = App->physics->CreateRectangleSensor(78, 356, 1, 34);
			pBody->body->SetTransform(pBody->body->GetPosition(), -0.6);
			break;
		case SlingPlace::SRIGHT:
			pBody2 = App->physics->CreateChain(0, 0, pRight, 6);
			pBody = App->physics->CreateRectangleSensor(161, 356, 1, 34);
			pBody->body->SetTransform({ pBody->body->GetPosition() }, 0.6);
			break;
		}
		force = 0.5;

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
					bodyB->body->SetLinearVelocity(b2Vec2{ 0,0 });
					bodyB->body->ApplyLinearImpulse({ (float32)(0.5 * 0.866), (float32)(-0.5 * 0.64) }, bodyB->body->GetPosition(), true);
					break;
				case SlingPlace::SRIGHT:
					bodyB->body->SetLinearVelocity(b2Vec2{ 0,0 });
					bodyB->body->ApplyLinearImpulse({ (float32)(0.5 * (-0.866)), (float32)(-0.5 * 0.64) }, bodyB->body->GetPosition(), true);
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
		86, 370 };
	int pRight[6] = {
		152, 370,
		172, 340,
		172, 360 };

	float32 force;

	PhysBody* pBody;
	PhysBody* pBody2;

	int bumperSfx;

	SlingPlace place;

	SDL_Texture* texture;
};