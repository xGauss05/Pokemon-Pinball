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

class Bumper : Prop {
public:
	Bumper(PropType type, BumperPlace bPlace) : Prop(type) {

		switch (bPlace)
		{
		case BumperPlace::BBOTTOM:
			x = 115;
			y = 165;
			break;
		case BumperPlace::BLEFT:
			x = 95;
			y = 138;
			break;
		case BumperPlace::BRIGHT:
			x = 132;
			y = 138;
			break;
		}
		radius = 8;

		pBody = App->physics->CreateCircle(x, y, radius);
		pBody->body->SetType(b2BodyType::b2_staticBody);

		initAnim();
		bumperSfx = App->audio->LoadFx("pinball/Sounds/bumpers.wav");

	}

	void initAnim()
	{
		texture = App->textures->Load("pinball/Textures/shroomish_sprite.png");

		idleAnim.PushBack({ 0, 0, 27, 32 });
		idleAnim.PushBack({ 27, 0, 27, 32 });
		idleAnim.speed = 0.05;

		hitAnim.PushBack({ 54, 0, 27, 32 });
		hitAnim.PushBack({ 81, 0, 27, 32 });
		hitAnim.speed = 0.05f;
		hitAnim.loop = false;

		currentAnim = &idleAnim;
	}

	void Blit() {
		if (currentAnim->HasFinished()) {
			currentAnim = &idleAnim;
		}


		if (currentAnim == &idleAnim)
		{
			idleAnim.Update();
			App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x) - radius*2, METERS_TO_PIXELS(pBody->body->GetPosition().y) - radius*3, &currentAnim->GetCurrentFrame());
		}

		if (currentAnim == &hitAnim)
		{
			hitAnim.Update();
			App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x) - radius*2, METERS_TO_PIXELS(pBody->body->GetPosition().y) - radius*3, &currentAnim->GetCurrentFrame());
		}

	}

	void PlaySFX() {
		App->audio->PlayFx(bumperSfx);
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();
		currentAnim = &hitAnim;
	}

	bool Update() {
		return true;
	}

	bool PostUpdate() {
		Blit();
		return true;
	}

private:
	int x;
	int y;
	int radius;

	PhysBody* pBody;

	int bumperSfx;

	SDL_Texture* texture;

	Animation* currentAnim;
	Animation idleAnim;
	Animation hitAnim;
};
