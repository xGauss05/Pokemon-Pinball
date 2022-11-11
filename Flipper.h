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
		texture = App->textures->Load("pinball/Textures/temp_flipper.png");

		switch (side)
		{
		case LEFT:
			x = 100;
			y = 100;
			xPin = x - 10;
			yPin = y;
			break;
		case RIGHT:
			x = 200;
			y = 100;
			xPin = x + 10;
			yPin = y;
			break;
		default:
			break;
		}


		pBody = App->physics->CreateRectangle(x, y, 30, 10);
		pin = App->physics->CreateRectangle(xPin, yPin, 1, 1);
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
		App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x - 15), METERS_TO_PIXELS(pBody->body->GetPosition().y - 5), NULL, 1.0f, pBody->GetRotation());
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