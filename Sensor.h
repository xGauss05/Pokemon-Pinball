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
	EVO_TOP, EVO_BOT, GET_TOP, GET_BOT,
	H, O, L, E, UP_LEFT, UP_MID,
	UP_RIGHT, COINS, MOUNTAIN,
	SPRING_IN, TOP_RAIL, LAKE_RAIL
};

class Sensor : public Prop {
public:
	Sensor(PropType type, SensorSide sSide) : Prop(type) {

		this->side = sSide;
		blingSfx = App->audio->LoadFx("pinball/Sounds/bling.wav");
		switch (sSide)
		{
		case EVO_TOP:
			pBody = App->physics->CreateRectangleSensor(39, 131, 10, 5);
			break;
		case EVO_BOT:
			pBody = App->physics->CreateRectangleSensor(26, 247, 5, 5);
			break;
		case GET_TOP:
			pBody = App->physics->CreateRectangleSensor(202, 131, 5, 5);
			break;
		case GET_BOT:
			pBody = App->physics->CreateRectangleSensor(210, 247, 5, 5);
			break;
		case H:
			pBody = App->physics->CreateRectangleSensor(32, 359, 5, 5);
			break;
		case O:
			pBody = App->physics->CreateRectangleSensor(56, 359, 5, 5);
			break;
		case L:
			pBody = App->physics->CreateRectangleSensor(182, 359, 5, 5);
			break;
		case E:
			pBody = App->physics->CreateRectangleSensor(206, 359, 5, 5);
			break;
		case UP_LEFT:
			pBody = App->physics->CreateRectangleSensor(87, 118, 5, 5);
			break;
		case UP_MID:
			pBody = App->physics->CreateRectangleSensor(108, 118, 5, 5);
			break;
		case UP_RIGHT:
			pBody = App->physics->CreateRectangleSensor(129, 118, 5, 5);
			break;
		case COINS:
			pBody = App->physics->CreateRectangleSensor(44, 229, 1, 1);
			break;
		case MOUNTAIN:
			pBody = App->physics->CreateRectangleSensor(155, 120, 5, 5);
			break;
		case SPRING_IN:
			pBody = App->physics->CreateRectangleSensor(242, 82, 8, 5);
			break;
		case TOP_RAIL:
			pBody = App->physics->CreateRectangleSensor(105, 47, 5, 5);
			break;
		case LAKE_RAIL:
			pBody = App->physics->CreateRectangleSensor(186, 225, 5, 5);
			break;
		default:
			pBody = App->physics->CreateRectangleSensor(0, 0, 5, 5);
			break;
		}

		pBody->prop = this;
		pBody->listener = (Module*)App->pManager;
		pBody->body->SetType(b2BodyType::b2_staticBody);
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
				App->scene->pikachuTrigger = true;
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
					METERS_TO_PIXELS(pBody->body->GetPosition().y)) {
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
					METERS_TO_PIXELS(pBody->body->GetPosition().y)) {
					App->scene->getBotFlag = false;
				}
				break;
			case H:
				if (App->scene->holeLight1.isActive != true) {
					App->scene->holeLight1.isActive = true;
				}
				break;
			case O:
				if (App->scene->holeLight2.isActive != true) {
					App->scene->holeLight2.isActive = true;
				}
				break;
			case L:
				if (App->scene->holeLight3.isActive != true) {
					App->scene->holeLight3.isActive = true;
				}
				break;
			case E:
				if (App->scene->holeLight4.isActive != true) {
					App->scene->holeLight4.isActive = true;
				}
				break;
			default:
				break;
			}
		}
	}

	bool CleanUp() {
		delete pBody;
		pBody = nullptr;

		return true;
	}

private:

	int switchLayer = -1;

	PhysBody* pBody = nullptr;

	// SFX
	int blingSfx;

	SensorSide side;

};