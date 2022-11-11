#pragma once
#include "Application.h"
#include "Module.h"
#include "Prop.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"

class ModulePhysics;
class SDL_Texture;

class Ball : Prop {
public:
	Ball(PropType type) : Prop(type) {
		texture = App->textures->Load("pinball/Textures/test.png");
		x = 50;
		y = 50;
		radius = 7;
		pBody = App->physics->CreateCircle(x, y, radius);
		pBody->body->SetType(b2BodyType::b2_dynamicBody);

		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
	}

	void Blit() {

		App->renderer->Blit(texture, METERS_TO_PIXELS(pBody->body->GetPosition().x - radius), METERS_TO_PIXELS(pBody->body->GetPosition().y - radius), NULL, 1.0f, pBody->GetRotation());
	}

private:
	int x;
	int y;
	int radius;

	PhysBody* pBody;

	SDL_Texture* texture;
};