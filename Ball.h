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
		x = 243;
		y = 350;
		radius = 8;
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
		App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x - radius), METERS_TO_PIXELS(pBody->body->GetPosition().y - radius), NULL, 1.0f, pBody->GetRotation());
	}

	void OnCollision(PhysBody* bodyB) {
		PlaySFX();
		if (bodyB->prop != NULL) {
			switch (bodyB->prop->type) {
			case PropType::BUMPERTOP:
				
				App->scene->score += BUMPER_SCORE;
				LOG("YEET");
				break;
			case PropType::BUMPERRIGHT:

				App->scene->score += BUMPER_SCORE;
				LOG("YEET");
				break;
			case PropType::BUMPERLEFT:

				App->scene->score += BUMPER_SCORE;
				LOG("YEET");
				break;
			default:
				App->scene->score += BUMPER_SCORE;

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

private:
	int x;
	int y;
	int radius;
	int ballSfx;
	PhysBody* pBody;

	SDL_Texture* texture;
};