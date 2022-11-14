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
			break;
		}

		
		pBody->body->SetType(b2BodyType::b2_staticBody);
		pBody->prop = this;
		pBody->listener = (Module*)App->pManager;
		initAnim();
		bumperSfx = App->audio->LoadFx("pinball/Sounds/shroomish_hit.wav");


	}

	void initAnim()
	{
		texture = App->textures->Load("pinball/Textures/shroomish_sprite.png");

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


		/*/if (currentAnim == &idleAnim)
		{
			idleAnim.Update();
			App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x) - radius * 3, METERS_TO_PIXELS(pBody->body->GetPosition().y) - radius * 4, &currentAnim->GetCurrentFrame());
		}

		if (currentAnim == &hitAnim)
		{
			hitAnim.Update();
			App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x) - radius * 3, METERS_TO_PIXELS(pBody->body->GetPosition().y) - radius * 4, &currentAnim->GetCurrentFrame());
		}*/

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
				

				bodyB->body->ApplyForceToCenter({  }, true);

			}
		}
	}

	bool Update() {
		if (currentAnim == &hitAnim && currentAnim->HasFinished()) {
			currentAnim = &idleAnim;
		}
		return true;
	}

	bool PostUpdate() {
		Blit();
		return true;
	}

private:
	int x;
	int y;
	int* pLeft;
	int* pRight;

	PhysBody* pBody;

	int bumperSfx;

	SDL_Texture* texture;

	Animation* currentAnim;
	Animation idleAnim;
	Animation hitAnim;
};