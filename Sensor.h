#pragma once
#include "Application.h"
#include "Module.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

class ModulePhysics;
class SDL_Texture;

enum SensorSide {
	EVO,
	GET,
	HOLE,
	UPGRADE,
	COINS,
	MOUNTAIN, 
	SPRING
};

class Sensor : public Prop {
public:
	Sensor(PropType type, SensorSide sSide) : Prop(type) {

		this->side = sSide;
		switch (sSide)
		{
		case EVO:
			pBody1 = App->physics->CreateRectangleSensor(29, 247, 5, 5);
			pBody2 = App->physics->CreateRectangleSensor(43, 115, 5, 5);
			pBody1->prop = pBody2->prop = this;
			pBody1->listener = pBody2->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			pBody2->body->SetType(b2BodyType::b2_staticBody);
			break;
		case GET:
			pBody1 = App->physics->CreateRectangleSensor(210, 247, 5, 5);
			pBody2 = App->physics->CreateRectangleSensor(202, 131, 5, 5);
			pBody1->prop = pBody2->prop = this;
			pBody1->listener = pBody2->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			pBody2->body->SetType(b2BodyType::b2_staticBody);
			break;
		case HOLE:
			pBody1 = App->physics->CreateRectangleSensor(32, 359, 5, 5);
			pBody2 = App->physics->CreateRectangleSensor(56, 359, 5, 5);
			pBody3 = App->physics->CreateRectangleSensor(182, 359, 5, 5);
			pBody4 = App->physics->CreateRectangleSensor(206, 359, 5, 5);
			pBody1->prop = pBody2->prop = pBody3->prop = pBody4->prop = this;
			pBody1->listener = pBody2->listener = pBody3->listener = pBody4->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			pBody2->body->SetType(b2BodyType::b2_staticBody);
			pBody3->body->SetType(b2BodyType::b2_staticBody);
			pBody4->body->SetType(b2BodyType::b2_staticBody);
			break;
		case UPGRADE:
			pBody1 = App->physics->CreateRectangleSensor(87, 118, 5, 5);
			pBody2 = App->physics->CreateRectangleSensor(108, 118, 5, 5);
			pBody3 = App->physics->CreateRectangleSensor(129, 118, 5, 5);
			pBody1->prop = pBody2->prop = pBody3->prop = this;
			pBody1->listener = pBody2->listener = pBody3->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			pBody2->body->SetType(b2BodyType::b2_staticBody);
			pBody3->body->SetType(b2BodyType::b2_staticBody);
			break;
		case COINS:
			pBody1 = App->physics->CreateRectangleSensor(43, 227, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			break;
		case MOUNTAIN:
			pBody1 = App->physics->CreateRectangleSensor(155, 120, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			break;
		case SPRING:
			pBody1 = App->physics->CreateRectangleSensor(240, 82, 5, 5);
			pBody2 = App->physics->CreateRectangleSensor(180, 31, 5, 5);
			pBody1->prop = pBody2->prop = this;
			pBody1->listener = pBody2->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			pBody2->body->SetType(b2BodyType::b2_staticBody);
			break;
		default:
			break;
		}
	}

	bool Update() {

		if (switchLayer != -1) {
			App->scene->switchLayer(switchLayer);
			switchLayer = -1;
		}

		return true;
	}

	void PlaySFX() {

	}

	void OnCollision(PhysBody* otherBody) {
		
	}

	void EndCollision(PhysBody* otherBody) {
		if (otherBody->prop != NULL && otherBody->prop->type == PropType::BALL) {
			switch (side)
			{
			case COINS:
				if (METERS_TO_PIXELS(otherBody->body->GetPosition().y) > 227) {
					switchLayer = 0;
				}
				else {
					switchLayer = 1;
				}
				break;
			case MOUNTAIN:
				if (METERS_TO_PIXELS(otherBody->body->GetPosition().y) > 120) {
					switchLayer = 0;
				}
				break;
			default:
				break;
			}
		}
	}

private:

	SensorSide side;

	PhysBody* pBody1 = nullptr;
	PhysBody* pBody2 = nullptr;
	PhysBody* pBody3 = nullptr;
	PhysBody* pBody4 = nullptr;

	int switchLayer = -1;

};