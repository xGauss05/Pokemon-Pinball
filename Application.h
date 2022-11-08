#pragma once

#include "p2List.h"
#include "Globals.h"

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleScene;
class ModulePhysics;
class ModuleFonts;
class ModuleDebug;
class PropsManager;

class Application
{
public:

	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleScene* scene;
	ModulePhysics* physics;
	ModuleFonts* fonts;
	ModuleDebug* debug;
	PropsManager* propManager;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
};

extern Application* App;
