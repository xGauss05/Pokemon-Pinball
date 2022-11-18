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
		ballTexture = App->textures->Load("pinball/Textures/ball.png");
		pokeball.PushBack({ 0,0,15,15 });
		superball.PushBack({ 16,0,15,15 });
		ultraball.PushBack({ 32,0,15,15 });
		masterball.PushBack({ 48,0,15,15 });
		pokeball.loop = superball.loop = ultraball.loop = masterball.loop = true;
		pokeball.speed = superball.speed = ultraball.speed = masterball.speed = 1;
		ballAnim = &pokeball;
		ballSfx = App->audio->LoadFx("pinball/Sounds/ball_collides.wav");

		radius = 8;

		spawn = iPoint(243, 350);

		afterRelease = iPoint(178, 34);

		pBody = App->physics->CreateCircle(spawn.x, spawn.y, radius);
		pBody->listener = (Module*)App->pManager;

		pBody->body->ResetMassData();

		b2MassData* data = new b2MassData;

		data->center = b2Vec2(0.0f, 0.0f);
		data->mass = 0.1f;
		data->I = 0.001f;
		pBody->body->SetMassData(data);
		pBody->body->SetBullet(true);

		pBody->body->SetFixedRotation(false);
		pBody->body->SetType(b2BodyType::b2_dynamicBody);
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(spawn.x), PIXEL_TO_METERS(spawn.y)), 0.0f);
		pBody->prop = this;
	}

	void PlaySFX() {
		App->audio->PlayFx(ballSfx);
	}

	void Blit() {
		App->renderer->Blit(ballTexture,
			METERS_TO_PIXELS(pBody->body->GetPosition().x - radius),
			METERS_TO_PIXELS(pBody->body->GetPosition().y - radius),
			&(ballAnim->GetCurrentFrame()),
			1.0f,
			pBody->GetRotation());
	}

	bool PreUpdate() {
		if (switchLayer != -1) {
			App->scene->switchLayer(switchLayer);
			switchLayer = -1;
		}

		if (METERS_TO_PIXELS(pBody->body->GetPosition().y) >= SCREEN_HEIGHT + 50) {
			TeleportTo(spawn);
			App->scene->switchLayer(2);
		}
	}

	bool Update() {

		switch (App->scene->ballMultiplier) {
		case 1:
			if (ballAnim != &pokeball) ballAnim = &pokeball;
			break;
		case 2:
			if (ballAnim != &superball) ballAnim = &superball;
			break;
		case 3:
			if (ballAnim != &ultraball) ballAnim = &ultraball;
			break;
		case 4:
			if (ballAnim != &masterball) ballAnim = &masterball;
			break;
		default:
			break;
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
			pBody->body->ApplyForceToCenter({ -30, 0 }, true);
		}

		if (wailmerSpit) {
			wailmerSpit = false;
			pBody->body->ApplyLinearImpulse({ (float32)(-0.55), (float32)(0.55) }, pBody->body->GetPosition(), true);
		}

		return true;
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();
		if (bodyB->prop != NULL) {
			int score;
			switch (bodyB->prop->type) {
			case PropType::BUMPERTOP:
				score = BUMPER_SCORE;
				LOG("Ball collided BUMPERTOP");
				break;
			case PropType::BUMPERRIGHT:
				score = BUMPER_SCORE;
				LOG("Ball collided BUMPERIGHT");
				break;
			case PropType::BUMPERLEFT:
				score = BUMPER_SCORE;	
				LOG("Ball collided BUMPERLEFT");
				break;
			case PropType::MINUN_BUTTON:
				score = MINUN_SCORE;
				LOG("Ball collided MINUN_BUTTON");
				break;
			case PropType::PLUSLE_BUTTON:
				score = PLUSLE_SCORE;
				LOG("Ball collided PLUSLE_BUTTON");
				break;
			case PropType::ZIGZAGOON_BUTTON:
				score = ZIGZAGOON_SCORE;
				LOG("Ball collided ZIGZAGOON_BUTTON");
				break;
			case PropType::SEEDOT_BUTTON:
				score = SEEDOT_SCORE;
				score *= App->scene->seedotMultiplier;
				LOG("Ball collided SEEDOT_BUTTON");
				break;
			case PropType::PELIPPER_BUTTON:
				score = PELIPPER_SCORE;
				score *= App->scene->pelipperMultiplier;
				LOG("Ball collided PELIPPER_BUTTON");
				break;
			case PropType::TROUGH:
				score = 0;
				lose = true;
				LOG("Ball collided TROUGH");
				break;
			case PropType::SLINGSHOT_LEFT:
				score = SLINGSHOT_SCORE;
				LOG("Ball collided SLINGSHOT_LEFT");
				break;
			case PropType::SLINGSHOT_RIGHT:
				score = SLINGSHOT_SCORE;
				LOG("Ball collided SLINGSHOT_RIGHT");
				break;
			case PropType::SENSOR_UP_LEFT:
				score = UP_SCORE;
				break;
			case PropType::SENSOR_UP_MID:
				score = UP_SCORE;
				break;
			case PropType::SENSOR_UP_RIGHT:
				score = UP_SCORE;
				break;
			case PropType::SENSOR_H:
				score = HOLE_SCORE;
				break;
			case PropType::SENSOR_O:
				score = HOLE_SCORE;
				break;
			case PropType::SENSOR_L:
				score = HOLE_SCORE;
				break;
			case PropType::SENSOR_E:
				score = HOLE_SCORE;
				break;
			case PropType::WAILMER:
				score = WAILMER_SCORE;
				App->scene->wailmerTrigger = true;
				wailmerSpit = true;
				LOG("Ball collided WAILMER");
				break;
			default:
				LOG("Ball collided ???");
				score = 0;
				break;
			}
			App->scene->currentScore += score * App->scene->ballMultiplier;
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
		if (l == layer) Blit();
	}

	void TeleportTo(iPoint position) {
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0.0f);
		pBody->body->ApplyForce(b2Vec2(0.1f, 0.0f), pBody->body->GetWorldCenter(), true);
	}

	bool CleanUp() {
		App->textures->Unload(ballTexture);

		ballAnim = nullptr;

		delete pBody;

		return true;
	}

private:

	int radius;

	int switchLayer = -1;

	// SFX
	int ballSfx;

	// Ball animations
	Animation pokeball, superball, ultraball, masterball;
	Animation* ballAnim = nullptr;

	// Spawn position
	iPoint spawn, afterRelease;
	bool lose, release, wailmerSpit;
	PhysBody* pBody;
	SDL_Texture* ballTexture;
};