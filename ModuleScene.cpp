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

	font = App->fonts->Load("pinball/font.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,&!- ", 1);
}

void ModuleScene::initMapCollisions()
{
	{
		int points[62] = {
		141, 461,
		96, 461,
		96, 429,
		23, 386,
		23, 330,
		32, 321,
		44, 321,
		44, 295,
		21, 263,
		11, 233,
		7, 204,
		7, 168,
		14, 141,
		27, 113,
		61, 84,
		92, 73,
		121, 70,
		157, 74,
		186, 90,
		208, 113,
		223, 137,
		230, 165,
		233, 199,
		231, 226,
		217, 264,
		194, 294,
		194, 319,
		208, 319,
		217, 328,
		217, 387,
		143, 429 };

		mapCollisionGeneral0 = App->physics->CreateChain(0, 0, points, 62);
	}
	{
		int points[52] = {
		141, 100,
		137, 110,
		139, 118,
		150, 120,
		164, 125,
		164, 143,
		142, 181,
		148, 177,
		165, 186,
		156, 217,
		161, 216,
		175, 188,
		193, 197,
		178, 226,
		177, 231,
		189, 210,
		203, 220,
		190, 244,
		197, 239,
		207, 219,
		212, 211,
		211, 180,
		204, 152,
		192, 130,
		175, 112,
		156, 102
		};

		mapCollisionLake0 = App->physics->CreateChain(0, 0, points, 52);
	}
	{
		int points[12] = {
		191, 342,
		191, 375,
		149, 402,
		154, 406,
		196, 378,
		196, 342
		};

		mapCollisionRight0 = App->physics->CreateChain(0, 0, points, 12);

	}
	{
		int points[12] = {
		43, 342,
		48, 342,
		48, 372,
		91, 402,
		86, 406,
		43, 377
		};

		mapCollisionLeft0 = App->physics->CreateChain(0, 0, points, 12);

	}
	{
		int points[26] = {
		50, 192,
		63, 215,
		54, 220,
		54, 227,
		33, 182,
		35, 150,
		57, 123,
		79, 107,
		79, 129,
		67, 145,
		94, 196,
		87, 200,
		65, 182
		};

		mapCollisionMarket0 = App->physics->CreateChain(0, 0, points, 26);

	}

	mapCollisionLake0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionMarket0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionLeft0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionRight0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionGeneral0->body->SetType(b2BodyType::b2_staticBody);


	mapCollisionsLayer0.add(mapCollisionLake0);
	mapCollisionsLayer0.add(mapCollisionMarket0);
	mapCollisionsLayer0.add(mapCollisionLeft0);
	mapCollisionsLayer0.add(mapCollisionRight0);
	mapCollisionsLayer0.add(mapCollisionGeneral0);


	switchLayer(0);

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
	if (currentLayer == 0) {
		ball->Blit();
	}

	// Blit Layer1
	App->renderer->Blit(mapLayer1, 0, 0);

	// TODO Blit Pokeball here if this layer
	if (currentLayer == 1) {
		ball->Blit();
	}

	// Blit Layer2
	App->renderer->Blit(mapLayer2, 0, 0);

	// TODO Blit Pokeball here if this layer
	if (currentLayer == 2) {
		ball->Blit();
	}

	// Blit Layer3
	App->renderer->Blit(mapLayer3, 0, 0);


	leftFlipper->Blit();
	rightFlipper->Blit();
	ball->Blit();
	spring->Blit();

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

void ModuleScene::switchLayer(int newLayer)
{
	switch (currentLayer)
	{
	case 0:
		for (p2List_item<PhysBody*>* mapPBody = mapCollisionsLayer0.getFirst(); mapPBody != NULL; mapPBody = mapPBody->next) {
			mapPBody->data->body->SetActive(false);
		}
		break;
	case 1:
		for (p2List_item<PhysBody*>* mapPBody = mapCollisionsLayer1.getFirst(); mapPBody != NULL; mapPBody = mapPBody->next) {
			mapPBody->data->body->SetActive(false);
		}
		break;
	case 2:
		for (p2List_item<PhysBody*>* mapPBody = mapCollisionsLayer2.getFirst(); mapPBody != NULL; mapPBody = mapPBody->next) {
			mapPBody->data->body->SetActive(false);
		}
		break;
	default:
		break;
	}

	if (newLayer <= 2 && newLayer >= 0) {
		currentLayer = newLayer;
	}

	switch (currentLayer)
	{
	case 0:
		for (p2List_item<PhysBody*>* mapPBody = mapCollisionsLayer0.getFirst(); mapPBody != NULL; mapPBody = mapPBody->next) {
			mapPBody->data->body->SetActive(true);
		}
		break;
	case 1:
		for (p2List_item<PhysBody*>* mapPBody = mapCollisionsLayer1.getFirst(); mapPBody != NULL; mapPBody = mapPBody->next) {
			mapPBody->data->body->SetActive(true);
		}
		break;
	case 2:
		for (p2List_item<PhysBody*>* mapPBody = mapCollisionsLayer2.getFirst(); mapPBody != NULL; mapPBody = mapPBody->next) {
			mapPBody->data->body->SetActive(true);
		}
		break;
	default:
		break;
	}

}

bool ModuleScene::Start()
{
	bool ret = true;

	// Set camera position
	App->renderer->camera.x = App->renderer->camera.y = 0;
	App->audio->PlayMusic("pinball/Music/bgm.ogg");
	initTextures();
	initMapCollisions();

	currentLayer = 0;

	leftFlipper = new Flipper(PropType::FLIPPER_LEFT, LEFT);

	rightFlipper = new Flipper(PropType::FLIPPER_RIGHT, RIGHT);

	ball = new Ball(PropType::BALL);

	spring = new Spring(PropType::SPRING);

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

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		rightFlipper->PlaySFX();
		rightFlipper->Kick();
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
		leftFlipper->PlaySFX();
		leftFlipper->Kick();
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP) {
		rightFlipper->StopKick();
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP) {
		leftFlipper->StopKick();
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		spring->StartLoading();
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		spring->Release();
	}

	drawScene();

	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate()
{

	return UPDATE_CONTINUE;
}

bool ModuleScene::CleanUp() {

	LOG("Unloading Intro scene");

	return true;
}

void ModuleScene::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA->prop != NULL) {
		switch (bodyA->prop->type) {
		case PropType::BALL:
			ball->OnCollision(bodyB);
			break;
		}
	}
	
	
}
