#pragma once
#include "Module.h"
#include "Globals.h"
#include "Prop.h"
#include "p2List.h"

class PropsManager : public Module
{
public:

	PropsManager(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~PropsManager();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Prop* CreateProp(PropType type);

	void DestroyProp(Prop* prop);

	void AddProp(Prop* prop);

	void OnCollision(PhysBody* pBody1, PhysBody* pBody2);

	void EndCollision(PhysBody* pBody1, PhysBody* pBody2);


public:

	p2List<Prop*> props;

};
