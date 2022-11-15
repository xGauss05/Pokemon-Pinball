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
			
		spawn = iPoint(243, 350);
		afterRelease = iPoint(178,34);

		pBody = App->physics->CreateCircle(x, y, radius);
		pBody->listener = (Module*)App->pManager;

		pBody->body->ResetMassData();

		b2MassData* data = new b2MassData;
		
		data->center= b2Vec2(0.0f, 0);
		data->mass = 0.1f;
		pBody->body->SetMassData(data);
		pBody->body->SetBullet(true);
	
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
		if (switchLayer != -1) {
			App->scene->switchLayer(switchLayer);
			switchLayer = -1;
		}

		if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) { 
			iPoint position;
			position.x = App->input->GetMouseX();
			position.y = App->input->GetMouseY();
			TeleportTo(position); 
		}
		if (lose) {
			lose = false;
			TeleportTo(spawn);
			pBody->body->ApplyLinearImpulse({ (float32)(-50), 0 }, pBody->body->GetPosition(), true);
		}

		if (release) {
			release = false;
			App->scene->switchLayer(2);
			TeleportTo(afterRelease);
			pBody->body->ApplyForceToCenter({-30, 0}, true);

		}

		if (METERS_TO_PIXELS(pBody->body->GetPosition().y) >= SCREEN_HEIGHT+50) {
			TeleportTo(spawn);
			App->scene->switchLayer(2);
		}

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
				lose = true;
				LOG("Ball collided TROUGH");
				break;
			case PropType::SLINGSHOT_LEFT:
				App->scene->currentScore += SLINGSHOT_SCORE;
			
				LOG("Ball collided SLINGSHOT_LEFT");
				break;
			case PropType::SLINGSHOT_RIGHT:
				App->scene->currentScore += SLINGSHOT_SCORE;
				
				LOG("Ball collided SLINGSHOT_RIGHT");
				break;
			case PropType::SENSOR_EVO_BOT:
				LOG("y ball: %d", METERS_TO_PIXELS(pBody->body->GetPosition().y));
				LOG("y sensor: %d", METERS_TO_PIXELS(bodyB->body->GetPosition().y));
				break;
			default:
				LOG("Ball collided ???");
				
				break;
			}
		}
	}

	void EndCollision(PhysBody* otherBody) {
		if (otherBody->prop != NULL) {
			switch (otherBody->prop->type) {
			case PropType::SENSOR_SPRING_IN:
				LOG("Ball END collided SPRING");
					release = true;
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
		//pBody->body->SetLinearVelocity(b2Vec2(0, 0));
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0.0f);
		pBody->body->ApplyForce(b2Vec2(0.1f, 0.0f), pBody->body->GetWorldCenter(), true);
	}

private:
	int x;
	int y;
	int radius;

	int switchLayer = -1;

	// SFX
	int ballSfx;

	// Spawn position
	iPoint spawn, afterRelease;
	bool lose, release;
	PhysBody* pBody;
	SDL_Texture* texture;
};