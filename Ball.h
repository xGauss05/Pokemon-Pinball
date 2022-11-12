#pragma once
#include "Application.h"
#include "Module.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

class ModulePhysics;
class SDL_Texture;

class Ball : public Prop {
public:
	Ball(PropType type) : Prop(type) {
		texture = App->textures->Load("pinball/Textures/test.png");
		x = 100;
		y = 100;
		radius = 7;
		type = PropType::BALL;
		pBody = App->physics->CreateCircle(x, y, radius);
		pBody->body->SetType(b2BodyType::b2_dynamicBody);
		pBody->listener = (Module*)App->scene;
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
		pBody->prop = this;
		ballSfx = App->audio->LoadFx("pinball/Sounds/ball_collides.wav");
	}

	void PlaySFX() {
		App->audio->PlayFx(ballSfx);
	}

	void Blit() {
		App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x - radius), METERS_TO_PIXELS(pBody->body->GetPosition().y - radius), NULL, 1.0f, pBody->GetRotation());
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();
	}

private:
	int x;
	int y;
	int radius;
	int ballSfx;
	PhysBody* pBody;

	SDL_Texture* texture;
};