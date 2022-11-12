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

		joint->EnableMotor(true);
		joint->SetMotorSpeed(-releaseSpeed);
		joint->SetMaxMotorForce(20);
	}

	void StartLoading()
	{
		joint->SetMotorSpeed(contractionSpeed);
	}

	void Release()
	{
		joint->SetMotorSpeed(-releaseSpeed);
	}

	void Blit() {
		//App->renderer->Blit(NULL);
	}

	void PlaySFX() {
		//App->audio->PlayFx(NULL);
	}

	bool Update() {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			StartLoading();
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
			Release();
		}
		return true;
	}

	bool PostUpdate() {
		Blit();
		return true;
	}

private:
	int x = 243;
	int y = 390;

	PhysBody* pBody;
	PhysBody* path;
	int pathLength = 15;
	float contractionSpeed = 0.3f;
	float releaseSpeed = 20.0f;

	b2PrismaticJoint* joint;

	SDL_Texture* texture;
};