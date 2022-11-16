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
	EVO_TOP,
	EVO_BOT,
	GET_TOP,
	GET_BOT,
	HOLE,
	UP_LEFT,
	UP_MID,
	UP_RIGHT,
	COINS,
	MOUNTAIN,
	SPRING_IN,
	TOP_RAIL,
	LAKE_RAIL
};

class Sensor : public Prop {
public:
	Sensor(PropType type, SensorSide sSide) : Prop(type) {

		this->side = sSide;
		blingSfx = App->audio->LoadFx("pinball/Sounds/bling.wav");
		switch (sSide)
		{
		case EVO_TOP:
			pBody1 = App->physics->CreateRectangleSensor(43, 131, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			break;
		case EVO_BOT:
			pBody1 = App->physics->CreateRectangleSensor(26, 247, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			break;
		case GET_TOP:
			pBody1 = App->physics->CreateRectangleSensor(202, 131, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			break;
		case GET_BOT:
			pBody1 = App->physics->CreateRectangleSensor(210, 247, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
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
		case UP_LEFT:
			pBody1 = App->physics->CreateRectangleSensor(87, 118, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			break;
		case UP_MID:
			pBody1 = App->physics->CreateRectangleSensor(108, 118, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			break;
		case UP_RIGHT:
			pBody1 = App->physics->CreateRectangleSensor(129, 118, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			break;
		case COINS:
			pBody1 = App->physics->CreateRectangleSensor(44, 229, 1, 1);
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
		case SPRING_IN:
			pBody1 = App->physics->CreateRectangleSensor(242, 82, 8, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			break;
		case TOP_RAIL:
			pBody1 = App->physics->CreateRectangleSensor(105, 47, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
			break;
		case LAKE_RAIL:
			pBody1 = App->physics->CreateRectangleSensor(186, 225, 5, 5);
			pBody1->prop = this;
			pBody1->listener = (Module*)App->pManager;
			pBody1->body->SetType(b2BodyType::b2_staticBody);
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
		App->audio->PlayFx(blingSfx);
	}

	void OnCollision(PhysBody* otherBody) {
		if (otherBody->prop != NULL && otherBody->prop->type == PropType::BALL) {
			switch (side)
			{
			case EVO_TOP:
				if (App->scene->evoBotFlag) {
					App->scene->evoTopFlag = true;
				}
				break;
			case EVO_BOT:
				App->scene->evoBotFlag = true;
				break;
			case GET_TOP:
				if (App->scene->getBotFlag) {
					App->scene->getTopFlag = true;
				}
				break;
			case GET_BOT:
				App->scene->getBotFlag = true;
				break;
			case UP_LEFT:
				if (App->scene->dotsLight1.isActive != true) {
					App->scene->dotsLight1.isActive = true;
				}
				break;
			case UP_MID:
				if (App->scene->dotsLight2.isActive != true) {
					App->scene->dotsLight2.isActive = true;
				}
				break;
			case UP_RIGHT:
				if (App->scene->dotsLight3.isActive != true) {
					App->scene->dotsLight3.isActive = true;
				}
				break;
			}
		}
	}

	void EndCollision(PhysBody* otherBody) {
		if (otherBody->prop != NULL && otherBody->prop->type == PropType::BALL) {
			int score;
			switch (side)
			{
			case SPRING_IN:

				break;
			case COINS:
				if (METERS_TO_PIXELS(otherBody->body->GetPosition().y) > 229) {
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
			case TOP_RAIL:
				switchLayer = 1;
				break;
			case LAKE_RAIL:
				if (METERS_TO_PIXELS(otherBody->body->GetPosition().y) > 225) {
					switchLayer = 0;
				}
				else {
					switchLayer = 1;
				}
			case EVO_TOP:
				if (App->scene->evoBotFlag && App->scene->evoTopFlag) {
					score = EVO_SCORE;
					LOG("BOTH LIGHTS EVO");
					if (App->scene->evoMultiplier < 3) {
						App->scene->evoMultiplier++;
					}
					App->scene->evoBotFlag = App->scene->evoTopFlag = false;
					App->scene->currentScore += score * App->scene->evoMultiplier;
					PlaySFX();
				}
				break;
			case EVO_BOT:
				if (METERS_TO_PIXELS(otherBody->body->GetPosition().y) >
					METERS_TO_PIXELS(pBody1->body->GetPosition().y)) {
					App->scene->evoBotFlag = false;
				}
				break;
			case GET_TOP:
				if (App->scene->getBotFlag && App->scene->getTopFlag) {
					score = GET_SCORE;
					LOG("BOTH LIGHTS GET");
					if (App->scene->getMultiplier < 3) {
						App->scene->getMultiplier++;
					}
					App->scene->getBotFlag = App->scene->getTopFlag = false;
					App->scene->currentScore += score * App->scene->getMultiplier;
					PlaySFX();
				}
				break;
			case GET_BOT:
				if (METERS_TO_PIXELS(otherBody->body->GetPosition().y) >
					METERS_TO_PIXELS(pBody1->body->GetPosition().y)) {
					App->scene->getBotFlag = false;
				}
				break;
			default:
				break;
			}
		}
	}

	bool CleanUp() {
		delete pBody1;
		pBody1 = nullptr;

		delete pBody2;
		pBody2 = nullptr;

		delete pBody3;
		pBody3 = nullptr;

		delete pBody4;
		pBody4 = nullptr;

		return true;
	}

private:

	SensorSide side;

	PhysBody* pBody1 = nullptr;
	PhysBody* pBody2 = nullptr;
	PhysBody* pBody3 = nullptr;
	PhysBody* pBody4 = nullptr;

	int blingSfx;
	int switchLayer = -1;

};