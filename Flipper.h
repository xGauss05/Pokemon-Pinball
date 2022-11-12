#pragma once
#include "Application.h"
#include "Module.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"

class ModulePhysics;
class SDL_Texture;

enum FLIPPERSIDE {
	LEFT,
	RIGHT
};

class Flipper : Prop {
public:
	Flipper(PropType type, FLIPPERSIDE side) : Prop(type) {

		switch (side)
		{
		case LEFT:
			texture = App->textures->Load("pinball/Textures/flipper_left.png");

			x = 92;
			y = 300;
			xPin = x - 11;
			yPin = y - 1;
			break;
		case RIGHT:
			texture = App->textures->Load("pinball/Textures/flipper_right.png");

			x = 148;
			y = 300;
			xPin = x + 11;
			yPin = y - 1;
			break;
		default:
			break;
		}


		pBody = App->physics->CreateRectangle(x, y, 32, 12);
		pin = App->physics->CreateRectangle(xPin, yPin, 2, 2);
		pBody->body->SetType(b2BodyType::b2_dynamicBody);
		pin->body->SetType(b2BodyType::b2_staticBody);

		joint = (b2RevoluteJoint*)App->physics->CreateRevoluteJoint(pBody, pin, xPin, yPin);

		switch (side)
		{
		case LEFT:
			joint->SetLimits(-30 * DEGTORAD, 30 * DEGTORAD);
			joint->SetMotorSpeed(20);
			joint->SetMaxMotorTorque(20);
			break;
		case RIGHT:
			joint->SetLimits(-30 * DEGTORAD, 30 * DEGTORAD);
			joint->SetMotorSpeed(-20);
			joint->SetMaxMotorTorque(20);
			break;
		default:
			break;
		}

		joint->EnableLimit(true);
	}

	void Blit() {
		App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x - 16), METERS_TO_PIXELS(pBody->body->GetPosition().y - 6), NULL, 1.0f, pBody->GetRotation());
	}

	void Kick() {
		joint->EnableMotor(true);
	}

	void StopKick() {
		joint->EnableMotor(false);
	}

private:
	int x;
	int y;

	int xPin;
	int yPin;

	PhysBody* pBody;
	PhysBody* pin;
	b2RevoluteJoint* joint;

	SDL_Texture* texture;
};