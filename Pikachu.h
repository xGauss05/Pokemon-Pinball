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


class Pikachu : public Prop {
public:
	Pikachu(PropType type) : Prop(type) {


		force = 50;

		pBody = App->physics->CreateRectangle(x, y, w, h);
		pBody->body->SetType(b2BodyType::b2_staticBody);
		pBody->prop = this;
		pBody->listener = (Module*)App->pManager;
		initAnim();
		pikachuSfx = App->audio->LoadFx("pinball/Sounds/pikachu.wav");
	}

	void initAnim()
	{
		pikachuTexture = App->textures->Load("pinball/Textures/pikachu_sprite.png");

		for (int i = 0; i < 2; i++) {
			idleAnim.PushBack({ i * 38, 0,38,91 });
		}
		idleAnim.speed = 0.05f;
		idleAnim.loop = true;

		for (int i = 2; i < 5; i++) {
			hitAnim.PushBack({ i * 38, 0,38,91 });
		}
		hitAnim.speed = 0.2f;
		hitAnim.loop = false;

		for (int i = 5; i < 9; i++) {
			chargedAnim.PushBack({ i * 38, 0,38,91 });
		}
		chargedAnim.speed = 0.05f;
		chargedAnim.loop = true;

		for (int i = 9; i < 13; i++) {
			returnAnim.PushBack({ i * 38, 0,38,91 });
		}
		returnAnim.speed = 0.2f;
		returnAnim.loop = false;



		currentAnim = &idleAnim;
	}

	void Blit() {

		idleAnim.Update();
		if (pikachuTrigger) {
			App->renderer->Blit(pikachuTexture, 191, 318, &(currentAnim->GetCurrentFrame()));
		}
		else {
			App->renderer->Blit(pikachuTexture, 14, 318, &(currentAnim->GetCurrentFrame()));
		}

	}

	void PlaySFX() {
		App->audio->PlayFx(pikachuSfx);
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();

		if (bodyB->prop != NULL) {
			if (bodyB->prop->type == PropType::BALL) {
				if (!charged) {
					currentAnim = &hitAnim;
					currentAnim->Reset();
				}

			}
		}
	}

	bool Update() {
		//if (currentAnim == &hitAnim && currentAnim->HasFinished()) currentAnim = &idleAnim;



		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) pikachuTrigger = false;

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) pikachuTrigger = true;


		return true;
	}

	bool PostUpdate() {
		Blit();
		return true;
	}

	bool CleanUp() {
		App->textures->Unload(pikachuTexture);

		currentAnim = nullptr;

		delete pBody;

		return true;
	}

private:
	int x;
	int y;
	int w;
	int h;
	int force;

	PhysBody* pBody;

	int pikachuSfx;
	bool pikachuTrigger = false;
	bool charged = false;

	SDL_Texture* pikachuTexture;

	Animation* currentAnim;
	Animation idleAnim;
	Animation hitAnim;
	Animation chargedAnim;
	Animation returnAnim;
};