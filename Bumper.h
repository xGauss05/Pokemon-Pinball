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

enum BumperPlace {
	BBOTTOM,
	BLEFT,
	BRIGHT
};

class Bumper : public Prop {
public:
	Bumper(PropType type, BumperPlace bPlace) : Prop(type) {

		switch (bPlace)
		{
		case BumperPlace::BBOTTOM:
			x = 115;
			y = 167;
			break;
		case BumperPlace::BLEFT:
			x = 98;
			y = 142;
			break;
		case BumperPlace::BRIGHT:
			x = 134;
			y = 142;
			break;
		}
		radius = 5;
		force = 50;

		pBody = App->physics->CreateCircle(x, y, radius);
		pBody->body->SetType(b2BodyType::b2_staticBody);
		pBody->prop = this;
		pBody->listener = (Module*)App->pManager;
		initAnim();
		bumperSfx = App->audio->LoadFx("pinball/Sounds/shroomish_hit.wav");
	}

	void initAnim()
	{
		shroomishTexture = App->textures->Load("pinball/Textures/shroomish_sprite.png");

		idleAnim.PushBack({ 0, 0, 27, 32 });
		idleAnim.PushBack({ 27, 0, 27, 32 });
		idleAnim.speed = 0.05;

		hitAnim.PushBack({ 54, 0, 27, 32 });
		hitAnim.PushBack({ 81, 0, 27, 32 });
		hitAnim.speed = 0.2f;
		hitAnim.loop = false;

		currentAnim = &idleAnim;
	}

	void Blit() {
		if (currentAnim == &idleAnim)
		{
			idleAnim.Update();
			App->renderer->Blit(shroomishTexture, METERS_TO_PIXELS(pBody->body->GetPosition().x) - radius*3, METERS_TO_PIXELS(pBody->body->GetPosition().y) - radius*4, &currentAnim->GetCurrentFrame());
		}

		if (currentAnim == &hitAnim)
		{
			hitAnim.Update();
			App->renderer->Blit(shroomishTexture, METERS_TO_PIXELS(pBody->body->GetPosition().x) - radius*3, METERS_TO_PIXELS(pBody->body->GetPosition().y) - radius*4, &currentAnim->GetCurrentFrame());
		}

	}

	void PlaySFX() {
		App->audio->PlayFx(bumperSfx);
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();
		currentAnim = &hitAnim;
		currentAnim->Reset();

		if (bodyB->prop != NULL) {
			if (bodyB->prop->type == PropType::BALL) {
				int angle = atan((pBody->body->GetPosition().y - bodyB->body->GetPosition().y) / (pBody->body->GetPosition().x - bodyB->body->GetPosition().x));
				float xForce = force * cos(angle);
				float yForce = force * sin(angle);
				
				bodyB->body->ApplyForceToCenter({ xForce, yForce }, true);
				
			}
		}
	}

	bool Update() {
		if (currentAnim == &hitAnim && currentAnim->HasFinished()) currentAnim = &idleAnim;
		
		return true;
	}

	bool PostUpdate() {
		Blit();
		return true;
	}

	bool CleanUp() {
		App->textures->Unload(shroomishTexture);

		currentAnim = nullptr;

		delete pBody;

		return true;
	}

public:

	int force;

private:

	int x;
	int y;
	int radius;

	PhysBody* pBody;
	
	// SFX
	int bumperSfx;

	// Bumper animations
	Animation idleAnim;
	Animation hitAnim;
	Animation* currentAnim = nullptr;

	// Shroomish texture
	SDL_Texture* shroomishTexture;

};
