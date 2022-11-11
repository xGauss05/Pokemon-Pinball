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
	ray_on = false;
}

ModuleScene::~ModuleScene()
{
	// You should do some memory cleaning here, if required
}

void ModuleScene::initTextures()
{
	LOG("Loading Scene assets");

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

	font = App->fonts->Load("pinball/font.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,&!-", 8);
}

void ModuleScene::drawScene()
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


	leftFlipper->Blit();
	ball->Blit();

}

void ModuleScene::doRayCast()
{
	// If user presses SPACE, enable RayCast
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on; // Enable raycast mode

		// Origin point of the raycast is be the mouse current position now (will not change)
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	// Prepare for raycast -----------------------------------------------------

	// The target point of the raycast is the mouse current position (will change over game time)
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// Total distance of the raycast reference segment
	int ray_hit = ray.DistanceTo(mouse);

	// Declare a vector. We will draw the normal to the hit surface (if we hit something)
	fVector normal(0.0f, 0.0f);

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
}

bool ModuleScene::Start()
{
	bool ret = true;

	// Set camera position
	App->renderer->camera.x = App->renderer->camera.y = 0;

	initTextures();

	//leftFlipper = new Flipper(PropType::FLIPPER_LEFT, LEFT);
	
	ball = new Ball(PropType::BALL);

	// Example of how to create a PhysBody

	/*circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
	circles.getLast()->data->ctype = ColliderType::BALL;
	circles.getLast()->data->listener = this;*/

	// Add this module (ModuleSceneIntro) as a listener for collisions with the sensor.
	// TODO

	return ret;
}

update_status ModuleScene::Update()
{
	// Keep playing
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate()
{
	//drawScene();
	//leftFlipper->Blit();
	ball->Blit();
	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp(){

	LOG("Unloading Intro scene");

	return true;
}

void ModuleScene::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	// Do something else. You can also check which bodies are colliding (sensor? ball? player?)
}
