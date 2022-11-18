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
			pBody = App->physics->CreateRectangleSensor(78, 356, 0, 34);
			pBody->body->SetTransform(pBody->body->GetPosition(), -0.6);
			break;
		case SlingPlace::SRIGHT:
			pBody2 = App->physics->CreateChain(0, 0, pRight, 6);
			pBody = App->physics->CreateRectangleSensor(160, 356, 0, 34);
			pBody->body->SetTransform({ pBody->body->GetPosition() }, 0.6);
			break;
		}
		force = 1.0f;

		pBody->body->SetType(b2BodyType::b2_staticBody);
		pBody2->body->SetType(b2BodyType::b2_staticBody);
		pBody->prop = this;
		pBody->listener = (Module*)App->pManager;
		slingshotTexture = App->textures->Load("pinball/Textures/Assets_Map.png");
		bumperSfx = App->audio->LoadFx("pinball/Sounds/bumpers.wav");
	}

	void Blit() {
		SDL_Rect left;
		SDL_Rect right;
		switch (place)
		{
		case SlingPlace::SLEFT:
			left = { 0, 59, 23, 34 };
			App->renderer->Blit(slingshotTexture, METERS_TO_PIXELS(pBody->body->GetPosition().x), METERS_TO_PIXELS(pBody->body->GetPosition().y), &left);
			break;
		case SlingPlace::SRIGHT:
			right = { 25, 59, 23, 34 };
			App->renderer->Blit(slingshotTexture, METERS_TO_PIXELS(pBody->body->GetPosition().x), METERS_TO_PIXELS(pBody->body->GetPosition().y), &right);
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
					//bodyB->body->SetLinearVelocity(b2Vec2{ 0,0 });
					bodyB->body->ApplyLinearImpulse({ (float32)(force * 0.866), (float32)(-force * 0.8) }, bodyB->body->GetPosition(), true);
					break;
				case SlingPlace::SRIGHT:
					//bodyB->body->SetLinearVelocity(b2Vec2{ 0,0 });
					bodyB->body->ApplyLinearImpulse({ (float32)(force * (-0.866)), (float32)(-force * 0.8) }, bodyB->body->GetPosition(), true);
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

	bool CleanUp() {
		App->textures->Unload(slingshotTexture);

		delete pBody;
		pBody = nullptr;

		delete pBody2;
		pBody2 = nullptr;

		return true;
	}

private:

	int pLeft[6] = {
		66, 360,
		66, 345,
		86, 370 };

	int pRight[6] = {
		152, 370,
		172, 345,
		172, 360 };

	float32 force;

	PhysBody* pBody;
	PhysBody* pBody2;

	int bumperSfx;

	SlingPlace place;

	SDL_Texture* slingshotTexture;
};