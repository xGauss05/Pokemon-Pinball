#pragma once
#include "Application.h"
#include "Module.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

class ModulePhysics;
class SDL_Texture;

enum FlipperSide {
	LEFT,
	RIGHT
};

class Flipper : public Prop {
public:
	Flipper(PropType type, FlipperSide fSide) : Prop(type) {
		side = fSide;
		
		switch (side)
		{
		case LEFT:
			flipperTexture = App->textures->Load("pinball/Textures/flipper_left.png");

			x = 97;
			y = 398;
			xPin = x - 11;
			yPin = y - 1;
			break;
		case RIGHT:
			flipperTexture = App->textures->Load("pinball/Textures/flipper_right.png");

			x = 143;
			y = 398;
			xPin = x + 11;
			yPin = y - 1;
			break;
		default:
			break;
		}

		pBody = App->physics->CreateRectangle(x, y, 30, 8);
		pin = App->physics->CreateRectangle(xPin, yPin, 2, 2);
		pBody->prop = this;

		pBody->body->ResetMassData();

		b2MassData* data = new b2MassData;

		data->center = b2Vec2(0.0f, 0.0f);
		data->mass = 10.0f;
		data->I = 1.0f;
		pBody->body->SetMassData(data);

		pBody->body->SetType(b2BodyType::b2_dynamicBody);
		pin->body->SetType(b2BodyType::b2_staticBody);

		joint = (b2RevoluteJoint*)App->physics->CreateRevoluteJoint(pBody, pin, xPin, yPin);

		switch (side)
		{
		case LEFT:
			joint->SetLimits(-21 * DEGTORAD, 30 * DEGTORAD);
			joint->SetMotorSpeed(-force);
			joint->SetMaxMotorTorque(force);
			break;
		case RIGHT:
			joint->SetLimits(-30 * DEGTORAD, 21 * DEGTORAD);
			joint->SetMotorSpeed(force);
			joint->SetMaxMotorTorque(force);
			break;
		default:
			break;
		}

		joint->EnableMotor(true);


		flipperSfx = App->audio->LoadFx("pinball/Sounds/flipper.wav");

		joint->EnableLimit(true);
	}

	void Blit() {
		App->renderer->Blit(flipperTexture, METERS_TO_PIXELS(pBody->body->GetPosition().x - 16), METERS_TO_PIXELS(pBody->body->GetPosition().y - 6), NULL, 1.0f, pBody->GetRotation());
	}

	bool Update() {
		switch (side) {
		case FlipperSide::LEFT:
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
				PlaySFX();
				Kick();
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP) {
				StopKick();
			}
			break;
		case FlipperSide::RIGHT:
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
				PlaySFX();
				Kick();
			}
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP) {
				StopKick();
			}
			break;
		default:
			break;
		}
		return true;
	}

	bool PostUpdate() { 
		Blit();
		return true;
	}

	void Kick() {
		switch (side)
		{
		case LEFT:
			joint->SetMotorSpeed(20);
			break;
		case RIGHT:
			joint->SetMotorSpeed(-20);
			break;
		default:
			break;
		}
	}

	void StopKick() {
		switch (side)
		{
		case LEFT:
			joint->SetMotorSpeed(-20);
			break;
		case RIGHT:
			joint->SetMotorSpeed(20);
			break;
		default:
			break;
		}
	}

	void PlaySFX() {
		App->audio->PlayFx(flipperSfx);
	}

	bool CleanUp() {
		App->textures->Unload(flipperTexture);

		delete pin;
		pin = nullptr;

		delete pBody;
		pBody = nullptr;

		joint = nullptr;

		return true;
	}

public:

	int force = 1000;

	b2RevoluteJoint* joint;

private:

	int x;
	int y;

	int xPin;
	int yPin;

	PhysBody* pBody;
	PhysBody* pin;

	// SFX
	int flipperSfx;

	FlipperSide side;

	SDL_Texture* flipperTexture;

};
