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

		x = 32;
		y = 395;
		w = h = 20;
		force = 43;

		pBody = App->physics->CreateRectangleSensor(x, y, w, h);
		pBody->body->SetType(b2BodyType::b2_kinematicBody);
		pBody->prop = this;
		pBody->listener = (Module*)App->pManager;
		initAnim();
		pikachuHitSfx = App->audio->LoadFx("pinball/Sounds/pikachu_hit.wav");
		pikachuThunderSfx = App->audio->LoadFx("pinball/Sounds/pikachu_thunder.wav");
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
		hitAnim.speed = 0.05f;
		hitAnim.loop = false;

		for (int i = 5; i < 9; i++) {
			chargedAnim.PushBack({ i * 38, 0,38,91 });
		}
		chargedAnim.speed = 0.1f;
		chargedAnim.loop = true;

		for (int i = 9; i < 13; i++) {
			thunderAnim.PushBack({ i * 38, 0,38,91 });
		}
		thunderAnim.speed = 0.05f;
		thunderAnim.loop = false;



		currentAnim = &idleAnim;
	}

	void Blit() {

		if (currentAnim == &idleAnim)
			idleAnim.Update();

		if (currentAnim == &hitAnim)
			hitAnim.Update();

		if (currentAnim == &chargedAnim)
			chargedAnim.Update();

		if (currentAnim == &thunderAnim)
			thunderAnim.Update();
		
		if (pikachuSide) {
			App->renderer->Blit(pikachuTexture, 191, 318, &(currentAnim->GetCurrentFrame()));
			x = 210;
			pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
		}
		else {
			App->renderer->Blit(pikachuTexture, 14, 318, &(currentAnim->GetCurrentFrame()));
			x = 32;
			pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
		}

	}

	void PlaySFX() {
		if (charged) App->audio->PlayFx(pikachuThunderSfx);
		else App->audio->PlayFx(pikachuHitSfx);
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();

		if (bodyB->prop != NULL) {
			if (bodyB->prop->type == PropType::BALL) {
				if (!charged) {
					currentAnim = &hitAnim;
					currentAnim->Reset();
				}
				else {
					
					currentAnim = &thunderAnim;
					currentAnim->Reset();
					App->scene->pikachuTrigger = false;
					charged = false;
					bodyB->body->ApplyForceToCenter({ 0, -force }, true);
				}

			}
		}
	}

	bool Update() {

		charged = App->scene->pikachuTrigger;

		if (currentAnim == &hitAnim && currentAnim->HasFinished()) currentAnim = &idleAnim;

		if (currentAnim == &thunderAnim && currentAnim->HasFinished()) currentAnim = &idleAnim;

		if (currentAnim == &chargedAnim && !charged) currentAnim = &idleAnim;

		if (charged) currentAnim = &chargedAnim;

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) pikachuSide = false;

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) pikachuSide = true;


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

	void Charge(){
		charged = true;
	}

private:
	float32 x;
	float32 y;
	float32 w;
	float32 h;
	float32 force;

	PhysBody* pBody;

	int pikachuHitSfx;
	int pikachuThunderSfx;
	bool pikachuSide = false;
	bool charged = false;

	SDL_Texture* pikachuTexture;

	Animation* currentAnim;
	Animation idleAnim;
	Animation hitAnim;
	Animation chargedAnim;
	Animation thunderAnim;
};