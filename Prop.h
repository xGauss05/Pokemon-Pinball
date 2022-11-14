#pragma once
#include "ModuleInput.h"
#include "ModuleRender.h"

class ModuleScene;

enum class PropType
{
	FLIPPER_LEFT,
	FLIPPER_RIGHT,
	SPRING,
	BALL,
	BUMPERTOP,
	BUMPERRIGHT,
	BUMPERLEFT,
	SEEDOT_BUTTON,
	ZIGZAGOON_BUTTON,
	MINUN_BUTTON,
	PLUSLE_BUTTON,
	PELIPPER_BUTTON,
	TROUGH,
	SENSOR_EVO,
	SENSOR_GET,
	SENSOR_HOLE,
	SENSOR_UPGRADE,
	SENSOR_COINS,
	SENSOR_MOUNTAIN,
	SENSOR_SPRING,
	SLINGSHOT_RIGHT,
	SLINGSHOT_LEFT,
	UNKNOWN
};

class Prop
{
public:

	Prop(PropType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual void BlitByLayer(int layer) {

	}

	virtual bool CleanUp()
	{
		return true;
	}

	void Prop::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Prop::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* otherBody) {

	};

	virtual void EndCollision(PhysBody* otherBody) {

	};

public:

	// string name;
	PropType type;
	bool active = true;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	bool renderable = true;
};
