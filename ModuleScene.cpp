#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	// Initialise all the internal class variables, at least to NULL pointer
	circle = box = NULL;
	ray_on = false;
	sensed = false;
}

ModuleScene::~ModuleScene()
{
	// You should do some memory cleaning here, if required
}

bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	// Set camera position
	App->renderer->camera.x = App->renderer->camera.y = 0;

	// Load map textures
	mapLayer0 = App->textures->Load("pinball/Textures/Map/Layer0.png");
	mapLayer1 = App->textures->Load("pinball/Textures/Map/Layer1.png");
	mapLayer2 = App->textures->Load("pinball/Textures/Map/Layer2.png");
	mapLayer3 = App->textures->Load("pinball/Textures/Map/Layer3.png");


	assetsTexture = App->textures->Load("pinball/Textures/Assets_Map.png");
	// Load Assets for the Map
	// Arrows
	greenArrow1 = { 33, 257, { 0, 0, 16, 24 }, true };
	greenArrow2 = { 43, 273, {16, 0, 16, 24}, true };
	greenArrow3 = { 53, 289, {32, 0,  16, 24}, true };
	blueArrow1 = { 47, 232, {48, 0, 16, 24}, true };
	blueArrow2 = { 54, 247, {64, 0, 16, 24}, true };
	blueArrow3 = { 61, 262, {80, 0, 16, 24}, true };
	redArrow1 = { 193, 257, {96, 0, 16, 24}, true };
	redArrow2 = { 183, 273, {112, 0, 16, 24}, true };
	redArrow3 = { 173, 289, {128, 0, 16, 24}, true };

	/*
	yellowArrow1;
	yellowArrow2;
	yellowArrow3;
	yellowArrow4;

	// Bumpers
	bumperLeft;
	bumperRight;

	// TODO Water Animation

	// TODO Screen Animation

	// TODO Hole Animation

	// TODO P Animation

	// H-O-L-E Letters
	holeLight1;
	holeLight2;
	holeLight3;
	holeLight4;

	// Saver
	saverLatios;

	// Dots
	dotsLight1;
	dotsLight2;
	dotsLight3;

	// Lives
	livesLight1;
	livesLight2;
	livesLight3;

	*/

	groundAssets.add(&greenArrow1);
	groundAssets.add(&greenArrow2);
	groundAssets.add(&greenArrow3);
	groundAssets.add(&blueArrow1);
	groundAssets.add(&blueArrow2);
	groundAssets.add(&blueArrow3);
	groundAssets.add(&redArrow1);
	groundAssets.add(&redArrow2);
	groundAssets.add(&redArrow3);

	// Load textures
	circle = App->textures->Load("pinball/wheel.png");
	box = App->textures->Load("pinball/crate.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	font = App->fonts->Load("pinball/font.png", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ.@'&-                       ", 8);
	// Create a big red sensor on the bottom of the screen.
	// This sensor will not make other objects collide with it, but it can tell if it is "colliding" with something else
	lower_ground_sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	// Add this module (ModuleSceneIntro) as a listener for collisions with the sensor.
	// In ModulePhysics::PreUpdate(), we iterate over all sensors and (if colliding) we call the function ModuleSceneIntro::OnCollision()
	lower_ground_sensor->listener = this;

	return ret;
}

bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleScene::Update()
{

	// Blit Layer0
	App->renderer->Blit(mapLayer0, 0, 0);

	for (p2List_item<GroundAsset*>* i = groundAssets.getFirst(); i; i = i->next) {
		if (i->data->isActive) {
			App->renderer->Blit(assetsTexture, i->data->x, i->data->y, &(i->data->rect));
		}
	}

	// TODO Blit Pokeball here if this layer

	// Blit Layer1
	App->renderer->Blit(mapLayer1, 0, 0);

	// TODO Blit Pokeball here if this layer

	// Blit Layer2
	App->renderer->Blit(mapLayer2, 0, 0);

	// TODO Blit Pokeball here if this layer

	// Blit Layer3
	App->renderer->Blit(mapLayer3, 0, 0);

	App->fonts->BlitText(20, 20, 0, "HOLA");

	// If user presses SPACE, enable RayCast
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on; // Enable raycast mode

		// Origin point of the raycast is be the mouse current position now (will not change)
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	// Example of how to create a PhysBody

	/*circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
	circles.getLast()->data->ctype = ColliderType::BALL;
	circles.getLast()->data->listener = this;*/ 

	// Prepare for raycast -----------------------------------------------------

	// The target point of the raycast is the mouse current position (will change over game time)
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// Total distance of the raycast reference segment
	int ray_hit = ray.DistanceTo(mouse);

	// Declare a vector. We will draw the normal to the hit surface (if we hit something)
	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------

	// Circles
	p2List_item<PhysBody*>* c = circles.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());

		c = c->next;
	}

	// Boxes
	c = boxes.getFirst();
	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);

		// Always paint boxes texture
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());

		// Are we hitting this box with the raycast?
		if (ray_on)
		{
			// Test raycast over the box, return fraction and normal vector
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if (hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	// Raycasts -----------------
	if (ray_on == true)
	{
		// Compute the vector from the raycast origin up to the contact point (if we're hitting anything; otherwise this is the reference length)
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		// Draw a line from origin to the hit point (or reference length if we are not hitting anything)
		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		// If we are hitting something with the raycast, draw the normal vector to the contact point
		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	// Keep playing
	return UPDATE_CONTINUE;
}

void ModuleScene::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	// Play Audio FX on every collision, regardless of who is colliding
	switch (bodyB->ctype)
	{
	case ColliderType::BALL:
		LOG("Colliding with BALL");
		break;
	case ColliderType::ITEM:
		LOG("Colliding with ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Colliding with UNKNOWN");
		break;
		/*case ColliderType::NEW_TYPE:
			LOG("Colliding with NEW_TYPE");
			break;*/
	}

	// Do something else.
}
