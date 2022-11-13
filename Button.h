#pragma once
#include "Application.h"
#include "Globals.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"

enum ButtonType {
	MINUN,
	PLUSLE,
	ZIGZAGOON,
	SEEDOT,
	PELIPPER
};

class Button :public Prop {
public:
	Button(PropType type, ButtonType btnType) : Prop(type) {
		// noTexture needed
		this->btnType = btnType;
		buttonSfx = App->audio->LoadFx("pinball/Sounds/button_hit.wav");
		switch (btnType) {
		case ButtonType::MINUN:
			x = 93;
			y = 200;
			break;
		case ButtonType::PLUSLE:
			x = 61;
			y = 220;
			break;
		case ButtonType::ZIGZAGOON:
			x = 188;
			y = 312;
			break;
		case ButtonType::SEEDOT:
			x = 52;
			y = 312;
			break;
		case ButtonType::PELIPPER:
			x = 152;
			y = 182;
			break;
		default:
			break;
		}

		pBody = App->physics->CreateRectangleSensor(x, y, 5, 5);
		pBody->listener = (Module*)App->pManager;

		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
		pBody->body->SetType(b2BodyType::b2_staticBody);
		pBody->prop = this;

	}

	void PlaySFX() {
		App->audio->PlayFx(buttonSfx);
	}

	void OnCollision(PhysBody* bodyB) {
		if (bodyB->prop != NULL) {
			switch (bodyB->prop->type) {
			case PropType::BALL:
				PlaySFX();
				if (btnType == ButtonType::PELIPPER) {
					App->scene->scoreMultiplier++;
					if (App->scene->scoreMultiplier >= 4) {
						App->scene->scoreMultiplier = 1;
					}
				}
			}
		}
	}

public:
	int scoreMultiplier = 1;

private:
	int x;
	int y;
	int buttonSfx;
	
	ButtonType btnType;
	PhysBody* pBody;
};