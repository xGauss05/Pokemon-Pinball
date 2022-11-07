#pragma once
#include "Module.h"
#include "Globals.h"
#include "Animation.h"
#include "p2Point.h"
#include "ModuleTextures.h"


class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();

	update_status Update();

	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	SDL_Texture* ballTexture = nullptr;
	Animation ballAnimation;
	Animation* currentAnimation = nullptr;

};