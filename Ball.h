#pragma once
#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"

class ModulePhysics;
class SDL_Texture;

class Ball : public Prop {
public:
	Ball(PropType type) : Prop(type) {
		texture = App->textures->Load("pinball/Textures/ball.png");
		ballSfx = App->audio->LoadFx("pinball/Sounds/ball_collides.wav");
		x = 100;
		y = 100;
		radius = 8;
		spawn.x = 243;
		spawn.y = 350;
		pBody = App->physics->CreateCircle(x, y, radius);
		pBody->listener = (Module*)App->pManager;

		pBody->body->SetType(b2BodyType::b2_dynamicBody);
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
		pBody->prop = this;
	}

	void PlaySFX() {
		App->audio->PlayFx(ballSfx);
	}

	void Blit() {
		App->renderer->Blit(texture,
			METERS_TO_PIXELS(pBody->body->GetPosition().x - 8),
			METERS_TO_PIXELS(pBody->body->GetPosition().y - 8),
			NULL,
			1.0f,
			pBody->GetRotation());
	}

	bool Update() {
		if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) TeleportTo(spawn);

		return true;
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();
		if (bodyB->prop != NULL) {
			int score;
			switch (bodyB->prop->type) {
			case PropType::BUMPERTOP:
				App->scene->currentScore += BUMPER_SCORE;
				LOG("Ball collided BUMPERTOP");
				break;
			case PropType::BUMPERRIGHT:
				App->scene->currentScore += BUMPER_SCORE;
				LOG("Ball collided BUMPERIGHT");
				break;
			case PropType::BUMPERLEFT:
				App->scene->currentScore += BUMPER_SCORE;
				LOG("Ball collided BUMPERLEFT");
				break;
			case PropType::MINUN_BUTTON:
				App->scene->currentScore += MINUN_SCORE;
				LOG("Ball collided MINUN_BUTTON");
				break;
			case PropType::PLUSLE_BUTTON:
				App->scene->currentScore += PLUSLE_SCORE;
				LOG("Ball collided PLUSLE_BUTTON");
				break;
			case PropType::ZIGZAGOON_BUTTON:
				App->scene->currentScore += ZIGZAGOON_SCORE;
				LOG("Ball collided ZIGZAGOON_BUTTON");
				break;
			case PropType::SEEDOT_BUTTON:
				score = SEEDOT_SCORE;
				App->scene->currentScore += score * App->scene->seedotMultiplier;
				LOG("Ball collided SEEDOT_BUTTON");
				break;
			case PropType::PELIPPER_BUTTON:
				score = PELIPPER_SCORE;
				App->scene->currentScore += score * App->scene->pelipperMultiplier;
				LOG("Ball collided PELIPPER_BUTTON");
				break;
			case PropType::TROUGH:
				//TeleportTo(spawn);
				LOG("Ball collided TROUGH");
				break;
			default:
				LOG("Ball collided ???");
				break;
			}

		}
	}

	void BlitByLayer(int layer) {
		int l = App->scene->currentLayer;
		if (l == layer) {
			Blit();
		}
	}

	void TeleportTo(iPoint position) {
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0.0f);
		pBody->body->ApplyForce(b2Vec2(0.1f, 0.0f), pBody->body->GetWorldCenter(), true);
	}

private:
	int x;
	int y;
	int radius;

	// SFX
	int ballSfx;

	// Spawn position
	iPoint spawn;

	PhysBody* pBody;
	SDL_Texture* texture;
};