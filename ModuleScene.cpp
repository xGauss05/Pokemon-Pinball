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
	greenArrow1 = { 33, 257, { 0, 0, 16, 24 }, false };
	greenArrow2 = { 43, 273, {16, 0, 16, 24}, false };
	greenArrow3 = { 53, 289, {32, 0,  16, 24}, false };
	blueArrow1 = { 47, 232, {48, 0, 16, 24}, true };
	blueArrow2 = { 54, 247, {64, 0, 16, 24}, true };
	blueArrow3 = { 61, 262, {80, 0, 16, 24}, true };
	redArrow1 = { 193, 257, {96, 0, 16, 24}, false };
	redArrow2 = { 183, 273, {112, 0, 16, 24}, false };
	redArrow3 = { 173, 289, {128, 0, 16, 24}, false };

	/*
	yellowArrow1;
	yellowArrow2;
	yellowArrow3;
	yellowArrow4;

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
	*/
	// Dots
	dotsLight1 = { 84, 105, { 216, 32, 8, 8 }, false };;
	dotsLight2 = { 105, 105, { 216, 32, 8, 8 }, false };;
	dotsLight3 = { 126, 105, { 216, 32, 8, 8 }, false };;

	// Lives
	livesLight1 = { 98, 338, { 232, 32, 12, 12 }, true };
	livesLight2 = { 114, 338, { 232, 32, 12, 12 }, true };
	livesLight3 = { 130, 338, { 232, 32, 12, 12 }, true };

	groundAssets.add(&greenArrow1);
	groundAssets.add(&greenArrow2);
	groundAssets.add(&greenArrow3);
	groundAssets.add(&blueArrow1);
	groundAssets.add(&blueArrow2);
	groundAssets.add(&blueArrow3);
	groundAssets.add(&redArrow1);
	groundAssets.add(&redArrow2);
	groundAssets.add(&redArrow3);
	groundAssets.add(&livesLight1);
	groundAssets.add(&livesLight2);
	groundAssets.add(&livesLight3);
	groundAssets.add(&dotsLight1);
	groundAssets.add(&dotsLight2);
	groundAssets.add(&dotsLight3);

	wailmerTexture = App->textures->Load("pinball/Textures/wailmer_sprite.png");
	wailmerIdle.PushBack({ 0,0,40,29 });
	wailmerIdle.PushBack({ 40,0,40,29 });
	wailmerIdle.speed = 0.05f;
	wailmerIdle.loop = true;
	wailmerAnim = &wailmerIdle;

	wailmerSpit.PushBack({ 80,0,40,29 });
	wailmerSpit.PushBack({ 120,0,40,29 });
	wailmerSpit.PushBack({ 160,0,40,29 });
	wailmerSpit.loop = false;
	wailmerSpit.speed = 0.2f;

	seedotTexture = App->textures->Load("pinball/Textures/seedot_sprite.png");
	seedotIdle.PushBack({ 18,31,17,22 });
	seedotIdle.PushBack({ 36,31,17,22 });
	seedotIdle.speed = 0.05f;
	seedotIdle.loop = true;
	seedotAnim = &seedotIdle;

	basketIdle.PushBack({ 0,12,27,17 });
	basketIdle.loop = true;
	basketAnim = &basketIdle;

	pelipperTexture = App->textures->Load("pinball/Textures/pelipper_sprite.png");
	pelipperIdle.PushBack({ 0,0,43,36 });
	pelipperIdle.PushBack({ 43,0,43,36 });
	pelipperIdle.speed = 0.05f;
	pelipperIdle.loop = true;
	pelipperAnim = &pelipperIdle;

	for (int i = 2; i < 5; i++) {
		pelipperBack.PushBack({ i * 43, 0,43,36 });
	}
	pelipperBack.speed = 0.05f;
	pelipperBack.loop = true;

	zigzagoonTexture = App->textures->Load("pinball/Textures/zigzagoon_sprite.png");
	zigzagoonIdle.PushBack({ 0, 0,46,35 });
	zigzagoonIdle.PushBack({ 46,0,46,35 });
	zigzagoonIdle.speed = 0.05f;
	zigzagoonIdle.loop = true;
	zigzagoonAnim = &zigzagoonIdle;

	for (int i = 2; i < 5; i++) {
		zigzagoonDance.PushBack({ i * 46, 0,46,35 });
	}
	zigzagoonDance.speed = 0.1f;
	zigzagoonDance.loop = true;


	minunTexture = App->textures->Load("pinball/Textures/minun_sprite.png");
	minunIdle.PushBack({ 0,0,30,36 });
	minunIdle.PushBack({ 30,0,30,36 });
	minunIdle.PushBack({ 60,0,30,36 });
	minunIdle.speed = 0.05f;
	minunIdle.loop = true;
	minunAnim = &minunIdle;

	for (int i = 0; i < 7; i++) {
		minunJump.PushBack({ i * 30, 36,30,36 });
	}
	minunJump.speed = 0.2f;
	minunJump.loop = true;

	lightningTexture = App->textures->Load("pinball/Textures/lightning_sprite.png");
	for (int i = 0; i < 9; i++) {
		lightningPlusle.PushBack({ i * 31, 0,31,21 });
	}
	lightningPlusle.speed = 0.1f;
	lightningPlusle.loop = true;
	lightPlusleAnim = &lightningPlusle;

	for (int i = 0; i < 9; i++) {
		lightningMinun.PushBack({ i * 31, 22,31,21 });
	}
	lightningMinun.speed = 0.1f;
	lightningMinun.loop = true;
	lightMinunAnim = &lightningMinun;

	plusleTexture = App->textures->Load("pinball/Textures/plusle_sprite.png");
	plusleIdle.PushBack({ 0,0,30,36 });
	plusleIdle.PushBack({ 30,0,30,36 });
	plusleIdle.PushBack({ 60,0,30,36 });
	plusleIdle.speed = 0.05f;
	plusleIdle.loop = true;
	plusleAnim = &plusleIdle;

	for (int i = 0; i < 7; i++) {
		plusleJump.PushBack({ i * 30, 36,30,36 });
	}
	plusleJump.speed = 0.2f;
	plusleJump.loop = true;

	pikachuTexture = App->textures->Load("pinball/Textures/pikachu_sprite.png");
	for (int i = 0; i < 2; i++) {
		pikachuIdle.PushBack({ i * 38, 0,38,91 });
	}
	pikachuIdle.speed = 0.05f;
	pikachuIdle.loop = true;
	pikachuAnim = &pikachuIdle;

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
		int points[50] = {
		141, 100,
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
		197, 194,
		210, 206,
		190, 244,
		197, 239,
		207, 219,
		211, 211,
		210, 180,
		203, 152,
		192, 129,
		175, 112,
		156, 102
		};

		mapCollisionLake0 = App->physics->CreateChain(0, 0, points, 50);
	}
	{
		int points[12] = {
		196, 344,
		191, 344,
		191, 373,
		160, 392,
		163, 398,
		196, 378
		};


		mapCollisionRight0 = App->physics->CreateChain(0, 0, points, 12);

	}
	{
		int points[12] = {
		44, 344,
		49, 344,
		49, 373,
		80, 392,
		77, 398,
		44, 378
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
		97, 102,
		99, 102,
		99, 120,
		97, 120
		};

		mapCollisionTopRectA0 = App->physics->CreateChain(0, 0, points, 8);

	}

	{
		int points[8] = {
		119, 100,
		121, 100,
		121, 118,
		119, 118
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
	48, 255,
	17, 195,
	10, 155,
	14, 111,
	28, 79,
	56, 53,
	88, 44,
	126, 49,
	145, 60,
	158, 76,
	165, 98,
	165, 150,
	146, 150,
	146, 96,
	136, 76,
	116, 66,
	87, 64,
	64, 71,
	42, 89,
	31, 115,
	28, 153,
	36, 188,
	65, 245
		};

		mapCollisionLeftRail1 = App->physics->CreateChain(0, 0, points, 46);

	}

	{
		int points[20] = {
	164, 250,
	190, 206,
	226, 170,
	232, 155,
	232, 60,
	249, 60,
	249, 161,
	243, 179,
	205, 217,
	184, 264
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

	{
		int points[20] = {
		194, 24,
		154, 25,
		115, 33,
		92, 40,
		70, 47,
		75, 66,
		103, 56,
		123, 50,
		155, 43,
		194, 42
		};

		mapCollisionTopRail2 = App->physics->CreateChain(0, 0, points, 20);
	}

	mapCollisionRigthRail2->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionTopRail2->body->SetType(b2BodyType::b2_staticBody);

	mapCollisionsLayer2.add(mapCollisionRigthRail2);
	mapCollisionsLayer2.add(mapCollisionTopRail2);

	switchLayer(2);
	switchLayer(1);
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

	// Pelipper score multiplier
	temp = std::to_string(pelipperMultiplier);
	num_char = temp.c_str();
	App->fonts->BlitText(142, 195, font, num_char);

}

void ModuleScene::drawAnimations() {
	wailmerAnim->Update();
	seedotAnim->Update();
	basketAnim->Update();
	pelipperAnim->Update();
	zigzagoonAnim->Update();
	plusleAnim->Update();
	minunAnim->Update();
	pikachuAnim->Update();
	lightMinunAnim->Update();
	lightPlusleAnim->Update();

	App->renderer->Blit(wailmerTexture, 168, 170, &(wailmerAnim->GetCurrentFrame()));

	if (seedotMultiplier >= 2.5f) {
		App->renderer->Blit(seedotTexture, 25, 280, &(seedotAnim->GetCurrentFrame()));
	}
	if (seedotMultiplier >= 2.0f) {
		App->renderer->Blit(seedotTexture, 10, 280, &(seedotAnim->GetCurrentFrame()));
	}
	if (seedotMultiplier >= 1.5f) {
		App->renderer->Blit(seedotTexture, 18, 283, &(seedotAnim->GetCurrentFrame()));
	}

	if (minunTrigger) {
		if (minunAnim != &minunJump) minunAnim = &minunJump;
	}
	else {
		if (minunAnim != &minunIdle) minunAnim = &minunIdle;
	}

	if (plusleTrigger) {
		if (plusleAnim != &plusleJump) plusleAnim = &plusleJump;
	}
	else {
		if (plusleAnim != &plusleIdle) plusleAnim = &plusleIdle;
	}

	if (pelipperTrigger) {
		if (pelipperAnim != &pelipperBack) pelipperAnim = &pelipperBack;
	}
	else {
		if (pelipperAnim != &pelipperIdle) pelipperAnim = &pelipperIdle;
	}

	if (zigzagoonTrigger) {
		if (zigzagoonAnim != &zigzagoonDance) zigzagoonAnim = &zigzagoonDance;
	}
	else {
		if (zigzagoonAnim != &zigzagoonIdle) zigzagoonAnim = &zigzagoonIdle;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) pikachuTrigger = false;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) pikachuTrigger = true;

	if (pikachuTrigger) {
		App->renderer->Blit(pikachuTexture, 191, 318, &(pikachuAnim->GetCurrentFrame()));
	}
	else {
		App->renderer->Blit(pikachuTexture, 14, 318, &(pikachuAnim->GetCurrentFrame()));
	}

	App->renderer->Blit(seedotTexture, 13, 295, &(basketAnim->GetCurrentFrame()));
	App->renderer->Blit(pelipperTexture, 150, 120, &(pelipperAnim->GetCurrentFrame()));
	App->renderer->Blit(zigzagoonTexture, 195, 270, &(zigzagoonAnim->GetCurrentFrame()));
	App->renderer->Blit(minunTexture, 66, 145, &(minunAnim->GetCurrentFrame()));
	App->renderer->Blit(plusleTexture, 35, 162, &(plusleAnim->GetCurrentFrame()));

	if (!minunTrigger) App->renderer->Blit(lightningTexture, 66, 145, &(lightMinunAnim->GetCurrentFrame()));

	if (!plusleTrigger) App->renderer->Blit(lightningTexture, 35, 162, &(lightPlusleAnim->GetCurrentFrame()));
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

void ModuleScene::ResetTable() {
	if (lives <= -1) {
		previousScore = currentScore;
		if (currentScore > highestScore) {
			highestScore = currentScore;
		}
		currentScore = 0;
		lives = 3;
		livesLight1.isActive = livesLight2.isActive = livesLight3.isActive = true;
	}
	evoMultiplier = getMultiplier = 0;
	greenArrow1.isActive = greenArrow2.isActive = greenArrow3.isActive = false;
	redArrow1.isActive = redArrow2.isActive = redArrow3.isActive = false;
	dotsLight1.isActive = dotsLight2.isActive = dotsLight3.isActive = false;
	upLeftFlag = upMidFlag = upRightFlag = false;
	plusleTrigger = minunTrigger = pelipperTrigger = zigzagoonTrigger = pikachuTrigger = false;
	pelipperMultiplier = ballMultiplier = 1;
	seedotMultiplier = 1.0f;
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
	App->pManager->CreateProp(PropType::PELIPPER_BUTTON);
	App->pManager->CreateProp(PropType::TROUGH);
	App->pManager->CreateProp(PropType::SENSOR_EVO_BOT);
	App->pManager->CreateProp(PropType::SENSOR_EVO_TOP);
	App->pManager->CreateProp(PropType::SENSOR_GET_BOT);
	App->pManager->CreateProp(PropType::SENSOR_GET_TOP);
	App->pManager->CreateProp(PropType::SENSOR_HOLE);
	App->pManager->CreateProp(PropType::SENSOR_UP_LEFT);
	App->pManager->CreateProp(PropType::SENSOR_UP_MID);
	App->pManager->CreateProp(PropType::SENSOR_UP_RIGHT);
	App->pManager->CreateProp(PropType::SENSOR_COINS);
	App->pManager->CreateProp(PropType::SENSOR_SPRING_IN);
	App->pManager->CreateProp(PropType::SENSOR_TOP_RAIL);
	App->pManager->CreateProp(PropType::SENSOR_MOUNTAIN);
	App->pManager->CreateProp(PropType::SENSOR_LAKE_RAIL);
	App->pManager->CreateProp(PropType::SLINGSHOT_LEFT);
	App->pManager->CreateProp(PropType::SLINGSHOT_RIGHT);
	App->pManager->CreateProp(PropType::WAILMER);

	return ret;
}

update_status ModuleScene::Update()
{
	drawScene();
	drawScore();
	drawAnimations();

	if (lives < 3 && livesLight3.isActive) livesLight3.isActive = false;
	if (lives < 2 && livesLight2.isActive) livesLight2.isActive = false;
	if (lives < 1 && livesLight1.isActive) livesLight1.isActive = false;

	if (evoMultiplier >= 1 && !greenArrow1.isActive) greenArrow1.isActive = true;
	if (evoMultiplier >= 2 && !greenArrow2.isActive) greenArrow2.isActive = true;
	if (evoMultiplier >= 3 && !greenArrow3.isActive) greenArrow3.isActive = true;

	if (getMultiplier >= 1 && !redArrow1.isActive) redArrow1.isActive = true;
	if (getMultiplier >= 2 && !redArrow2.isActive) redArrow2.isActive = true;
	if (getMultiplier >= 3 && !redArrow3.isActive) redArrow3.isActive = true;

	if (wailmerTrigger && wailmerAnim != &wailmerSpit) {
		wailmerAnim = &wailmerSpit;
	}

	if (wailmerAnim->HasFinished() && wailmerAnim != &wailmerIdle) {
		wailmerAnim = &wailmerIdle;
		wailmerSpit.Reset();
		wailmerTrigger = false;
	}

	if (dotsLight1.isActive && dotsLight2.isActive && dotsLight3.isActive) {
		dotsLight1.isActive = dotsLight2.isActive = dotsLight3.isActive = false;
		if (ballMultiplier < 4) {
			ballMultiplier++;
		}
	}

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
