#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "ModuleDebug.h"
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

	// H-O-L-E Letters
	holeLight1 = { 28, 343, { 152, 32, 10, 10 }, false };
	holeLight2 = { 52, 343, { 168, 32, 10, 10 }, false };
	holeLight3 = { 178, 343, { 184, 32, 10, 10 }, false };
	holeLight4 = { 202, 343, { 200, 32, 10, 10 }, false };

	// Dots
	dotsLight1 = { 84, 105, { 216, 32, 8, 8 }, false };
	dotsLight2 = { 105, 105, { 216, 32, 8, 8 }, false };
	dotsLight3 = { 126, 105, { 216, 32, 8, 8 }, false };

	// Lives
	livesLight1 = { 98, 338, { 232, 32, 12, 12 }, true };
	livesLight2 = { 114, 338, { 232, 32, 12, 12 }, true };
	livesLight3 = { 130, 338, { 232, 32, 12, 12 }, true };

	counterNo1 = { 143, 196, { 161, 81, 5, 7 }, false };
	counterNo2 = { 142, 196, { 169, 81, 5, 7 }, false };
	counterNo3 = { 142, 196, { 177, 81, 5, 7 }, false };

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
	groundAssets.add(&holeLight1);
	groundAssets.add(&holeLight2);
	groundAssets.add(&holeLight3);
	groundAssets.add(&holeLight4);
	groundAssets.add(&counterNo1);
	groundAssets.add(&counterNo2);
	groundAssets.add(&counterNo3);

	pButton.PushBack({ 200,48,8,10 });
	pButton.PushBack({ 216,48,8,10 });
	pButton.PushBack({ 232,48,8,10 });
	pButton.PushBack({ 200,64,8,10 });
	pButton.PushBack({ 216,64,8,10 });
	pButton.speed = 0.2f;
	pButton.loop = true;
	pButtonAnim = &pButton;

	martArrow.PushBack({ 0,24,26,31 });
	martArrow.PushBack({ 0,0,0,0 });
	martArrow.speed = 0.05f;
	martArrow.loop = true;
	martArrowAnim = &martArrow;

	bumperArrow.PushBack({ 32,24,31,21 });
	bumperArrow.PushBack({ 0,0,0,0 });
	bumperArrow.speed = 0.05f;
	bumperArrow.loop = true;
	bumperArrowAnim = &bumperArrow;

	catchArrow.PushBack({ 65,24,29,31 });
	catchArrow.PushBack({ 0,0,0,0 });
	catchArrow.speed = 0.05f;
	catchArrow.loop = true;
	catchArrowAnim = &catchArrow;

	hatchArrow.PushBack({ 97,24,29,31 });
	hatchArrow.PushBack({ 0,0,0,0 });
	hatchArrow.speed = 0.05f;
	hatchArrow.loop = true;
	hatchArrowAnim = &hatchArrow;

	screen.PushBack({ 0,96,50,40 });
	screen.PushBack({ 56,96,50,40 });
	screen.PushBack({ 112,96,50,40 });
	screen.PushBack({ 56,96,50,40 });
	screen.PushBack({ 0,96,50,40 });
	screen.PushBack({ 0,136,50,40 });
	screen.PushBack({ 56,136,50,40 });
	screen.PushBack({ 0,136,50,40 });
	screen.PushBack({ 0,176,50,40 });
	screen.PushBack({ 56,177,50,40 });
	screen.PushBack({ 0,216,50,40 });
	screen.PushBack({ 56,216,50,40 });
	screen.PushBack({ 0,216,50,40 });
	screen.PushBack({ 56,177,50,40 });
	screen.PushBack({ 0,176,50,40 });
	screen.PushBack({ 0,136,50,40 });
	screen.PushBack({ 56,136,50,40 });
	screen.PushBack({ 0,136,50,40 });
	screen.loop = true;
	screen.speed = 0.1f;
	screenAnim = &screen;

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

	wailmerPit.PushBack({ 152,0,31,31 });
	wailmerPit.PushBack({ 184,0,31,31 });
	wailmerPit.PushBack({ 216,0,31,31 });
	wailmerPit.PushBack({ 184,0,31,31 });
	wailmerPit.loop = true;
	wailmerPit.speed = 0.1f;
	wailmerPitAnim = &wailmerPit;

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
		int points[56] = {
			157, 219,
			176, 185,
			200, 191,
			175, 231,
			176, 231,
			201, 191,
			210, 190,
			211, 209,
			192, 244,
			193, 244,
			212, 209,
			211, 182,
			205, 159,
			197, 142,
			186, 125,
			169, 109,
			141, 102,
			138, 108,
			138, 117,
			151, 120,
			167, 130,
			167, 145,
			142, 176,
			142, 182,
			148, 176,
			164, 184,
			163, 197,
			155, 218
		};

		mapCollisionLake0 = App->physics->CreateChain(0, 0, points, 56);
	}

	{
		int points[10] = {
		196, 342,
		191, 373,
		160, 392,
		163, 398,
		196, 378
		};


		mapCollisionRight0 = App->physics->CreateChain(0, 0, points, 10);

	}

	{
		int points[10] = {
		44, 342,
		49, 373,
		80, 392,
		77, 398,
		44, 378
		};

		mapCollisionLeft0 = App->physics->CreateChain(0, 0, points, 10);

	}

	{
		int points[34] = {
		52, 222,
		64, 215,
		52, 189,
		64, 182,
		87, 202,
		96, 197,
		81, 175,
		72, 156,
		72, 139,
		80, 130,
		80, 107,
		66, 116,
		52, 131,
		40, 145,
		33, 164,
		36, 187,
		42, 201
		};

		mapCollisionMarket0 = App->physics->CreateChain(0, 0, points, 34);

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

	{
		int points[8] = {
		41, 244,
		40, 244,
		28, 221,
		29, 221
		};

		mapCollisionMarketRail0 = App->physics->CreateChain(0, 0, points, 8);

	}

	mapCollisionLake0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionMarket0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionMarketRail0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionLeft0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionRight0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionGeneral0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionTopRectA0->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionTopRectB0->body->SetType(b2BodyType::b2_staticBody);

	mapCollisionsLayer0.add(mapCollisionLake0);
	mapCollisionsLayer0.add(mapCollisionMarket0);
	mapCollisionsLayer0.add(mapCollisionMarketRail0);
	mapCollisionsLayer0.add(mapCollisionLeft0);
	mapCollisionsLayer0.add(mapCollisionRight0);
	mapCollisionsLayer0.add(mapCollisionGeneral0);
	mapCollisionsLayer0.add(mapCollisionTopRectA0);
	mapCollisionsLayer0.add(mapCollisionTopRectB0);

	{
		int points[52] = {
		41, 244,
		40, 244,
		22, 209,
		10, 164,
		10, 120,
		23, 83,
		43, 62,
		71, 46,
		98, 43,
		123, 45,
		139, 53,
		159, 72,
		166, 97,
		166, 136,
		165, 136,
		165, 97,
		158, 72,
		139, 54,
		123, 46,
		98, 44,
		71, 47,
		43, 63,
		24, 83,
		11, 120,
		11, 164,
		23, 209
		};

		mapCollisionLeftRailA1 = App->physics->CreateChain(0, 0, points, 52);

	}

	{
		int points[48] = {
		57, 226,
		42, 199,
		35, 173,
		32, 144,
		33, 118,
		43, 90,
		66, 71,
		83, 64,
		119, 65,
		134, 76,
		146, 97,
		146, 141,
		147, 141,
		147, 97,
		135, 76,
		119, 64,
		83, 63,
		66, 70,
		42, 90,
		32, 118,
		31, 144,
		34, 173,
		41, 199,
		56, 226
		};

		mapCollisionLeftRailB1 = App->physics->CreateChain(0, 0, points, 48);

	}

	{
		int points[20] = {
		177, 230,
		189, 207,
		228, 163,
		231, 153,
		231, 60,
		230, 60,
		230, 153,
		227, 163,
		188, 207,
		176, 230
		};

		mapCollisionRightRailA1 = App->physics->CreateChain(0, 0, points, 20);

	}

	{
		int points[20] = {
		189, 244,
		205, 215,
		241, 178,
		249, 164,
		249, 55,
		250, 55,
		250, 164,
		242, 178,
		206, 215,
		190, 244
		};

		mapCollisionRightRailB1 = App->physics->CreateChain(0, 0, points, 20);

	}


	mapCollisionLeftRailA1->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionLeftRailB1->body->SetType(b2BodyType::b2_staticBody);

	mapCollisionRightRailA1->body->SetType(b2BodyType::b2_staticBody);
	mapCollisionRightRailB1->body->SetType(b2BodyType::b2_staticBody);

	mapCollisionsLayer1.add(mapCollisionLeftRailA1);
	mapCollisionsLayer1.add(mapCollisionLeftRailB1);

	mapCollisionsLayer1.add(mapCollisionRightRailA1);
	mapCollisionsLayer1.add(mapCollisionRightRailB1);

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

	martArrowAnim->Update();
	bumperArrowAnim->Update();
	catchArrowAnim->Update();
	hatchArrowAnim->Update();
	pButtonAnim->Update();
	wailmerPitAnim->Update();

	App->renderer->Blit(assetsTexture, 169, 180, &(wailmerPitAnim->GetCurrentFrame()));
	App->renderer->Blit(assetsTexture, 149, 170, &(pButtonAnim->GetCurrentFrame()));
	App->renderer->Blit(assetsTexture, 73, 214, &(martArrowAnim->GetCurrentFrame()));
	App->renderer->Blit(assetsTexture, 104, 186, &(bumperArrowAnim->GetCurrentFrame()));
	App->renderer->Blit(assetsTexture, 144, 222, &(catchArrowAnim->GetCurrentFrame()));
	App->renderer->Blit(assetsTexture, 155, 249, &(hatchArrowAnim->GetCurrentFrame()));

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
	screenAnim->Update();

	App->renderer->Blit(assetsTexture, 22, 115, &(screenAnim->GetCurrentFrame()));

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
		holeLight1.isActive = holeLight2.isActive = holeLight3.isActive = holeLight4.isActive = false;
		hasGivenLife = false;
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
	switchLayer(0);
	switchLayer(1);
	switchLayer(2);

	App->pManager->CreateProp(PropType::FLIPPER_LEFT);
	App->pManager->CreateProp(PropType::FLIPPER_RIGHT);
	App->pManager->CreateProp(PropType::BUMPERTOP);
	App->pManager->CreateProp(PropType::BUMPERRIGHT);
	App->pManager->CreateProp(PropType::BUMPERLEFT);
	App->debug->ball = (Ball*)App->pManager->CreateProp(PropType::BALL);
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
	App->pManager->CreateProp(PropType::SENSOR_H);
	App->pManager->CreateProp(PropType::SENSOR_O);
	App->pManager->CreateProp(PropType::SENSOR_L);
	App->pManager->CreateProp(PropType::SENSOR_E);
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

	if (holeLight1.isActive && holeLight2.isActive && holeLight3.isActive && holeLight4.isActive && !hasGivenLife) {
		if (lives < 3) {
			lives++;
			switch (lives) {
			case 1:
				livesLight1.isActive = true;
				break;
			case 2:
				livesLight2.isActive = true;
				break;
			case 3:
				livesLight3.isActive = true;
				break;
			}
		}
		hasGivenLife = true;
	}

	if (pelipperMultiplier == 1) {
		counterNo1.isActive = true;
	}
	else {
		counterNo1.isActive = false;
	}

	if (pelipperMultiplier == 2) {
		counterNo2.isActive = true;
	}
	else {
		counterNo2.isActive = false;
	}

	if (pelipperMultiplier == 3) {
		counterNo3.isActive = true;
	}
	else {
		counterNo3.isActive = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
		if (holeLight1.isActive || holeLight2.isActive || holeLight3.isActive || holeLight4.isActive) {
			if (!hasGivenLife) {
				bool auxiliar1 = holeLight4.isActive;
				bool auxiliar2 = holeLight3.isActive;
				holeLight3.isActive = auxiliar1;

				auxiliar1 = holeLight2.isActive;
				holeLight2.isActive = auxiliar2;

				auxiliar2 = holeLight1.isActive;
				holeLight1.isActive = auxiliar1;

				holeLight4.isActive = auxiliar2;
			}
		}

		if (dotsLight1.isActive || dotsLight2.isActive || dotsLight3.isActive) {
			bool auxiliar1 = dotsLight3.isActive;
			bool auxiliar2 = dotsLight2.isActive;
			dotsLight2.isActive = auxiliar1;

			auxiliar1 = dotsLight1.isActive;
			dotsLight1.isActive = auxiliar2;

			dotsLight3.isActive = auxiliar1;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		if (holeLight1.isActive || holeLight2.isActive || holeLight3.isActive || holeLight4.isActive) {
			if (!hasGivenLife) {
				bool auxiliar1 = holeLight1.isActive;
				bool auxiliar2 = holeLight2.isActive;
				holeLight2.isActive = auxiliar1;

				auxiliar1 = holeLight3.isActive;
				holeLight3.isActive = auxiliar2;

				auxiliar2 = holeLight4.isActive;
				holeLight4.isActive = auxiliar1;

				holeLight1.isActive = auxiliar2;
			}
		}

		if (dotsLight1.isActive || dotsLight2.isActive || dotsLight3.isActive) {
			bool auxiliar1 = dotsLight1.isActive;
			bool auxiliar2 = dotsLight2.isActive;
			dotsLight2.isActive = auxiliar1;

			auxiliar1 = dotsLight3.isActive;
			dotsLight3.isActive = auxiliar2;

			dotsLight1.isActive = auxiliar1;
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
	App->textures->Unload(mapLayer0);
	App->textures->Unload(mapLayer1);
	App->textures->Unload(mapLayer2);
	App->textures->Unload(mapLayer3);
	App->textures->Unload(wailmerTexture);
	App->textures->Unload(seedotTexture);
	App->textures->Unload(pelipperTexture);
	App->textures->Unload(zigzagoonTexture);
	App->textures->Unload(minunTexture);
	App->textures->Unload(plusleTexture);
	App->textures->Unload(lightningTexture);
	App->textures->Unload(pikachuTexture);

	screenAnim = nullptr;
	wailmerAnim = nullptr;
	basketAnim = nullptr;
	pelipperAnim = nullptr;
	zigzagoonAnim = nullptr;
	plusleAnim = nullptr;
	minunAnim = nullptr;
	lightPlusleAnim = nullptr;
	lightMinunAnim = nullptr;
	martArrowAnim = nullptr;
	bumperArrowAnim = nullptr;
	catchArrowAnim = nullptr;
	hatchArrowAnim = nullptr;
	pButtonAnim = nullptr;
	wailmerPitAnim = nullptr;

	mapCollisionsLayer0.clear();
	mapCollisionsLayer1.clear();
	mapCollisionsLayer2.clear();

	delete mapCollisionLake0;
	mapCollisionLake0 = nullptr;
	delete mapCollisionMarket0;
	mapCollisionMarket0 = nullptr;
	delete mapCollisionMarketRail0;
	mapCollisionMarketRail0 = nullptr;
	delete mapCollisionLeft0;
	mapCollisionLeft0 = nullptr;
	delete mapCollisionRight0;
	mapCollisionRight0 = nullptr;
	delete mapCollisionGeneral0;
	mapCollisionGeneral0 = nullptr;
	delete mapCollisionTopRectA0;
	mapCollisionTopRectA0 = nullptr;
	delete mapCollisionTopRectB0;
	mapCollisionTopRectB0 = nullptr;
	delete mapCollisionLeftRailA1;
	mapCollisionLeftRailA1 = nullptr;
	delete mapCollisionLeftRailB1;
	mapCollisionLeftRailB1 = nullptr;
	delete mapCollisionRightRailA1;
	mapCollisionRightRailA1 = nullptr;
	delete mapCollisionRightRailB1;
	mapCollisionRightRailB1 = nullptr;
	delete mapCollisionRigthRail2;
	mapCollisionRigthRail2 = nullptr;
	delete mapCollisionTopRail2;
	mapCollisionTopRail2 = nullptr;

	return true;
}
