#include "PropsManager.h"
#include "Prop.h"
#include "Flipper.h"
#include "Ball.h"
#include "Spring.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"


PropsManager::PropsManager(Application* app, bool start_enabled) : Module(app, start_enabled) {

}

// Destructor
PropsManager::~PropsManager()
{}

bool PropsManager::Start() {

	bool ret = true;

	//Iterates over the entities and calls Start
	p2List_item<Prop*>* item;
	Prop* prop = NULL;

	for (item = props.getFirst(); item != NULL && ret == true; item = item->next)
	{
		prop = item->data;

		if (prop->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool PropsManager::CleanUp()
{
	bool ret = true;
	p2List_item<Prop*>* item;
	item = props.getLast();

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	props.clear();

	return ret;
}

Prop* PropsManager::CreateProp(PropType type)
{
	Prop* prop = nullptr;

	//L02: DONE 2: Instantiate entity according to the type and add the new entoty it to the list of Entities

	switch (type)
	{
	case PropType::BALL:
		prop = (Prop*)new Ball(type);
		break;
	case PropType::BUMPER:
		//prop = (Prop*)new Bumper(type);
		break;
	case PropType::FLIPPER_LEFT:
		prop = (Prop*)new Flipper(type, FlipperSide::LEFT);
		break;
	case PropType::FLIPPER_RIGHT:
		prop = (Prop*)new Flipper(type, FlipperSide::RIGHT);
		break;
	case PropType::SPRING:
		prop = (Prop*)new Spring(type);
		break;
	case PropType::UNKNOWN:
		break;
	default:
		break;
	}

	// Created entities are added to the list
	AddProp(prop);

	return prop;
}

void PropsManager::DestroyProp(Prop* prop)
{
	p2List_item<Prop*>* item;

	for (item = props.getFirst(); item != NULL; item = item->next)
	{
		if (item->data == prop) props.del(item);
	}
}

void PropsManager::AddProp(Prop* prop)
{
	if (prop != nullptr) props.add(prop);
}

void PropsManager::OnCollision(PhysBody* pBody1, PhysBody* pBody2)
{
	if (pBody1->prop != nullptr) {
		pBody1->prop->OnCollision(pBody2);
	}
}

void PropsManager::EndCollision(PhysBody* pBody1, PhysBody* pBody2)
{
	if (pBody1->prop != nullptr) {
		pBody1->prop->EndCollision(pBody2);
	}
}

update_status PropsManager::Update()
{
	bool ret = true;
	p2List_item<Prop*>* item;
	Prop* prop = NULL;

	for (item = props.getFirst(); item != NULL && ret == true; item = item->next)
	{
		prop = item->data;

		if (prop->active == false) continue;
		ret = item->data->Update();
	}

	return UPDATE_CONTINUE;
}

update_status PropsManager::PostUpdate()
{
	bool ret = true;
	p2List_item<Prop*>* item;
	Prop* prop = NULL;

	for (item = props.getFirst(); item != NULL && ret == true; item = item->next)
	{
		prop = item->data;

		if (prop->active == false) continue;
		ret = item->data->PostUpdate();
	}

	return UPDATE_CONTINUE;
}