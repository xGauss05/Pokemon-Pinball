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
		texture = App->textures->Load("pinball/Textures/temp_flipper.png");
		x  = 236;
		y = 364;
		radius = 7;
		pBody = App->physics->CreateCircle(x, y, radius);
		pBody->body->SetType(b2BodyType::b2_dynamicBody);
		
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0.0f);
	}

	void Blit() {
		App->renderer->Blit(texture, x, y);
	}

private:
	int x;
	int y;
	int radius;

	PhysBody* pBody;
	
	SDL_Texture* texture;
};