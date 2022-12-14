#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

struct GroundAsset {
	int x;
	int y;
	SDL_Rect rect;
	bool isActive;
};

class ModuleScene : public Module
{
public:

	// Constructors & Destructors
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	void initTextures();
	void initMapCollisions();
	void drawScene();
	void drawScore();
	void drawAnimations();
	void doRayCast();
	void ResetTable();

	void switchLayer(int newLayer);

	// Main module steps
	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:

	int currentLayer;

	// Lists of physics objects

	// Map Textures
	SDL_Texture* mapLayer0;
	SDL_Texture* mapLayer1;
	SDL_Texture* mapLayer2;
	SDL_Texture* mapLayer3;

	SDL_Texture* assetsTexture;

	// Assets for the Map
	// Arrows
	GroundAsset greenArrow1;
	GroundAsset greenArrow2;
	GroundAsset greenArrow3;
	GroundAsset blueArrow1;
	GroundAsset blueArrow2;
	GroundAsset blueArrow3;
	GroundAsset redArrow1;
	GroundAsset redArrow2;
	GroundAsset redArrow3;

	// H-O-L-E Letters
	GroundAsset holeLight1;
	GroundAsset holeLight2;
	GroundAsset holeLight3;
	GroundAsset holeLight4;

	// Dots
	GroundAsset dotsLight1;
	GroundAsset dotsLight2;
	GroundAsset dotsLight3;
	
	// Lives
	GroundAsset livesLight1;
	GroundAsset livesLight2;
	GroundAsset livesLight3;

	// Pelipper button counter.
	GroundAsset counterNo1;
	GroundAsset counterNo2;
	GroundAsset counterNo3;

	p2List<GroundAsset*> groundAssets;

	// map collisions
	// 0
	PhysBody* mapCollisionLake0;
	PhysBody* mapCollisionMarket0;
	PhysBody* mapCollisionMarketRail0;
	PhysBody* mapCollisionLeft0;
	PhysBody* mapCollisionRight0;
	PhysBody* mapCollisionGeneral0;
	PhysBody* mapCollisionTopRectA0;
	PhysBody* mapCollisionTopRectB0;

	p2List<PhysBody*> mapCollisionsLayer0;

	// 1
	PhysBody* mapCollisionLeftRailA1;
	PhysBody* mapCollisionLeftRailB1;

	PhysBody* mapCollisionRightRailA1;
	PhysBody* mapCollisionRightRailB1;

	p2List<PhysBody*> mapCollisionsLayer1;

	// 2
	PhysBody* mapCollisionRigthRail2;
	PhysBody* mapCollisionTopRail2;

	p2List<PhysBody*> mapCollisionsLayer2;

	// Animation (Pelipper button)
	Animation pButton;
	Animation* pButtonAnim = nullptr;

	// Animations (Yellow arrows)
	Animation martArrow;
	Animation* martArrowAnim = nullptr;
	Animation bumperArrow;
	Animation* bumperArrowAnim = nullptr;
	Animation catchArrow;
	Animation* catchArrowAnim = nullptr;
	Animation hatchArrow;
	Animation* hatchArrowAnim = nullptr;

	// Animations (Screen)
	Animation screen;
	Animation* screenAnim = nullptr;

	// Animations (Wailmer)
	SDL_Texture* wailmerTexture;
	Animation wailmerIdle;
	Animation wailmerSpit;
	Animation* wailmerAnim = nullptr;

	// Animations (Wailmer pit)
	Animation wailmerPit;
	Animation* wailmerPitAnim = nullptr;

	// Animations (Seedot)
	SDL_Texture* seedotTexture;
	Animation seedotIdle;
	Animation* seedotAnim = nullptr;
	Animation basketIdle;
	Animation* basketAnim = nullptr;

	// Animations (Pelipper)
	SDL_Texture* pelipperTexture;
	Animation pelipperIdle;
	Animation pelipperBack;
	Animation* pelipperAnim = nullptr;

	// Animations (Zigzagoon)
	SDL_Texture* zigzagoonTexture;
	Animation zigzagoonIdle;
	Animation zigzagoonDance;
	Animation* zigzagoonAnim = nullptr;

	// Animations (Minun)
	SDL_Texture* minunTexture;
	Animation minunIdle;
	Animation minunJump;
	Animation* minunAnim = nullptr;

	// Animations (Plusle)
	SDL_Texture* plusleTexture;
	Animation plusleIdle;
	Animation plusleJump;
	Animation* plusleAnim = nullptr;

	// Animations (Minun & Plusle lightning)
	SDL_Texture* lightningTexture;
	Animation lightningPlusle;
	Animation lightningMinun;
	Animation* lightPlusleAnim = nullptr;
	Animation* lightMinunAnim = nullptr;

	// Score variables
	unsigned int currentScore = 0;
	unsigned int previousScore = 0;
	unsigned int highestScore = 0;
	int pelipperMultiplier = 1;
	float seedotMultiplier = 1.0f;
	int evoMultiplier = 0;
	int getMultiplier = 0;
	int ballMultiplier = 1;

	// Game variables
	int lives = 3;
	bool plusleTrigger = false;
	bool minunTrigger = false;
	bool pelipperTrigger = false;
	bool zigzagoonTrigger = false;
	bool pikachuTrigger = false;
	bool wailmerTrigger = false;
	bool evoTopFlag = false;
	bool evoBotFlag = false;
	bool getTopFlag = false;
	bool getBotFlag = false;
	bool upLeftFlag = false;
	bool upMidFlag = false;
	bool upRightFlag = false;
	bool hasGivenLife = false;

	// Font
	int font;

	// Raycast
	p2Point<int> ray;
	bool ray_on;
};
