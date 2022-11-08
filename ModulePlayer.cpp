#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading ball(player)");
	ballTexture = App->textures->Load("pinball/Textures/Ball/myBigBalls.png"); // pending to change

	// ballAnimation pushbacks
	// ballAnimation.PushBack({ 0, 0, 0, 0 });
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading ball(player)");
	
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB) {
	switch (bodyB->ctype) {
		switch (bodyB->ctype)
		{
		case ColliderType::BALL:
			LOG("BALL colliding with BALL");
			break;
		case ColliderType::ITEM:
			LOG("BALL Colliding with ITEM");
			break;
		case ColliderType::UNKNOWN:
			LOG("BALL colliding with UNKNOWN");
			break;
		/*case ColliderType::NEW_TYPE:
			LOG("Colliding with NEW_TYPE");
			break;*/
		}
	}
	// Do something else
}
