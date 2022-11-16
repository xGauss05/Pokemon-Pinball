#pragma once
#include "Application.h"
#include "Module.h"
#include "Animation.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

class ModulePhysics;
class SDL_Texture;

class Spring : Prop {
public:
	Spring(PropType type) : Prop(type) {

		pBody = App->physics->CreateRectangle(x, y, 13, 20);
		pBody->body->SetType(b2BodyType::b2_dynamicBody);
		path = App->physics->CreateRectangle(x, y + 12, 1, 10);
		path->body->SetType(b2BodyType::b2_staticBody);

		joint = (b2PrismaticJoint*)App->physics->CreatePrismaticJoint(pBody, path, x, y, b2Vec2(0.0f, -1.0f));

		joint->SetLimits(PIXEL_TO_METERS(-pathLength), PIXEL_TO_METERS(pathLength));
		joint->EnableLimit(true);

		pBody->body->ResetMassData();
		b2MassData* data = new b2MassData;
		data->center = b2Vec2(0.0f, 0);
		data->mass = 3.0f;
		pBody->body->SetMassData(data);

		joint->EnableMotor(true);
		joint->SetMotorSpeed(-releaseSpeed);
		joint->SetMaxMotorForce(700);

		initAnim();
		releaseSFX = App->audio->LoadFx("pinball/Sounds/spoink_release.wav");
		chargeSFX = App->audio->LoadFx("pinball/Sounds/spoink_charge.wav");
	}

	void initAnim()
	{
		spoinkTexture = App->textures->Load("pinball/Textures/spoink_sprite.png");

		idleAnim.PushBack({ 0, 0, 20, 40 });
		idleAnim.PushBack({ 20, 0, 20, 40 });
		idleAnim.speed = 0.05;

		for (int i = 2; i < 7; i++)
		{
			compressionAnim.PushBack({ i * 20, 0, 20, 40 });
		}
		compressionAnim.speed = 0.1f;
		compressionAnim.loop = false;

		currentAnim = &idleAnim;
	}

	void StartLoading()
	{
		joint->SetMotorSpeed(contractionSpeed);
		compressionAnim.Reset();
		currentAnim = &compressionAnim;

		App->audio->PlayFx(chargeSFX);
	}

	void Release()
	{
		joint->SetMotorSpeed(-releaseSpeed);
		currentAnim = &idleAnim;

		App->audio->PlayFx(releaseSFX);
	}

	void BlitByLayer(int layer) {
		if (layer == 0) {
			idleAnim.Update();
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) { compressionAnim.Update(); }

			if (currentAnim == &idleAnim)
			{
				App->renderer->Blit(spoinkTexture, METERS_TO_PIXELS(pBody->body->GetPosition().x) - 9, METERS_TO_PIXELS(pBody->body->GetPosition().y) - 11, &currentAnim->GetCurrentFrame());
			}
			if (currentAnim == &compressionAnim)
			{
				App->renderer->Blit(spoinkTexture,
					METERS_TO_PIXELS(pBody->body->GetPosition().x) - 9,
					METERS_TO_PIXELS(pBody->body->GetPosition().y) - 11 - 4,
					&currentAnim->GetCurrentFrame());
			}
		}
	}

	bool Update() {
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
			StartLoading();
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
			Release();
		}
		return true;
	}

	bool CleanUp() {
		App->textures->Unload(spoinkTexture);

		delete pBody;
		pBody = nullptr;

		delete path;
		path = nullptr;

		joint = nullptr;

		currentAnim = nullptr;

		return true;
	}

private:
	int x = 243;
	int y = 395;

	PhysBody* pBody;
	PhysBody* path;
	int pathLength = 7;
	float contractionSpeed = 0.3f;
	float releaseSpeed = 200.0f;

	b2PrismaticJoint* joint;

	SDL_Texture* spoinkTexture;
	
	Animation* currentAnim;
	Animation idleAnim;
	Animation compressionAnim;

	int releaseSFX;
	int chargeSFX;
};