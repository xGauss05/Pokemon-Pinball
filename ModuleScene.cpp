#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "PropsManager.h"

#include <string>

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
		int points[44] = {
		36, 233,
		25, 209,
		29, 174,
		40, 147,
		58, 124,
		80, 107,
		80, 130,
		72, 136,
		69, 147,
		95, 193,
		95, 197,
		86, 201,
		64, 182,
		52, 188,
		63, 214,
		54, 221,
		55, 227,
		49, 217,
		43, 201,
		27, 210,
		42, 243,
		36, 238
		};

		mapCollisionMarket0 = App->physics->CreateChain(0, 0, points, 44);

	}

	{
		int points[8] = {
		96, 102,
		101, 102,
		101, 120,
		96, 120
		};

		mapCollisionTopRectA0 = App->physics->CreateChain(0, 0, points, 8);

	}

	{
		int points[8] = {
		117, 100,
		122, 100,
		122, 118,
		117, 118
		};

		mapCollisionTopRectB0 = App->physics->CreateChain(0, 0, points, 8);

	}

	mapCollisionLake0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionMarket0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionLeft0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionRight0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionGeneral0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionTopRectA0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionTopRectB0->body->SetType(b2BodyType::b2_staticBody);

	mapCollisionsLayer0.add(mapCollisionLake0);
	mapCollisionsLayer0.add(mapCollisionMarket0);
	mapCollisionsLayer0.add(mapCollisionLeft0);
	mapCollisionsLayer0.add(mapCollisionRight0);
	mapCollisionsLayer0.add(mapCollisionGeneral0);
	mapCollisionsLayer0.add(mapCollisionTopRectA0);
	mapCollisionsLayer0.add(mapCollisionTopRectB0);

	{
		int points[46] = {
	39, 237,
	19, 195,
	12, 155,
	15, 111,
	29, 79,
	57, 55,
	89, 46,
	126, 49,
	145, 60,
	158, 76,
	165, 98,
	165, 134,
	148, 134,
	148, 96,
	137, 74,
	116, 62,
	87, 62,
	64, 71,
	42, 89,
	31, 115,
	28, 153,
	36, 188,
	53, 223
		};

		mapCollisionLeftRail1 = App->physics->CreateChain(0, 0, points, 46);

	}

	{
		int points[20] = {
	177, 228,
	190, 206,
	226, 170,
	232, 155,
	232, 113,
	249, 113,
	249, 161,
	243, 179,
	205, 217,
	191, 244
		};

		mapCollisionRightRail1 = App->physics->CreateChain(0, 0, points, 20);

	}

	mapCollisionLeftRail1->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionRightRail1->body->SetType(b2BodyType::b2_staticBody);

	mapCollisionsLayer1.add(mapCollisionLeftRail1);
	mapCollisionsLayer1.add(mapCollisionRightRail1);

	{
		int points[8] = {
		234, 60,
		234, 443,
		252, 443,
		252, 60
		};

		mapCollisionRigthRail2 = App->physics->CreateChain(0, 0, points, 8);

	}

	mapCollisionRigthRail2->body->SetType(b2BodyType::b2_staticBody);

	mapCollisionsLayer2.add(mapCollisionRigthRail2);


	switchLayer(2);
	switchLayer(1);
	switchLayer(0);

	switchLayer(2);


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
	App->pManager->BlitPropsByLayer(0);

	// Blit Layer1
	App->renderer->Blit(mapLayer1, 0, 0);

	// TODO Blit Pokeball here if this layer
	App->pManager->BlitPropsByLayer(1);

	// Blit Layer2
	App->renderer->Blit(mapLayer2, 0, 0);

	// TODO Blit Pokeball here if this layer
	App->pManager->BlitPropsByLayer(2);

	// Blit Layer3
	App->renderer->Blit(mapLayer3, 0, 0);

}

void ModuleScene::drawScore() {

	// Current score
	std::string temp = std::to_string(currentScore);
	char const* num_char = temp.c_str();
	App->fonts->BlitText(App->renderer->camera.x + 5, SCREEN_HEIGHT - 10, font, num_char);

	// Highest score
	temp = std::to_string(highestScore);
	num_char = temp.c_str();
	App->fonts->BlitText(App->renderer->camera.x + 5, App->renderer->camera.y + 5, font, "HIGH");
	App->fonts->BlitText(App->renderer->camera.x + 5, App->renderer->camera.y + 12, font, num_char);

	// Previous score
	temp = std::to_string(previousScore);
	num_char = temp.c_str();
	App->fonts->BlitText(App->renderer->camera.x + 5, App->renderer->camera.y + 19, font, "PREV");
	App->fonts->BlitText(App->renderer->camera.x + 5, App->renderer->camera.y + 26, font, num_char);

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

	currentLayer = 0;
	currentScore = 0;
	initTextures();
	initMapCollisions();

	App->pManager->CreateProp(PropType::FLIPPER_LEFT);
	App->pManager->CreateProp(PropType::FLIPPER_RIGHT);
	App->pManager->CreateProp(PropType::BUMPERTOP);
	App->pManager->CreateProp(PropType::BUMPERRIGHT);
	App->pManager->CreateProp(PropType::BUMPERLEFT);
	App->pManager->CreateProp(PropType::BALL);
	App->pManager->CreateProp(PropType::SPRING);
	App->pManager->CreateProp(PropType::MINUN_BUTTON);
	App->pManager->CreateProp(PropType::PLUSLE_BUTTON);
	App->pManager->CreateProp(PropType::SEEDOT_BUTTON);
	App->pManager->CreateProp(PropType::ZIGZAGOON_BUTTON);

	return ret;
}

update_status ModuleScene::Update()
{
	drawScene();
	drawScore();

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


