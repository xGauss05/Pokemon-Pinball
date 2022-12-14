#include "ModuleDebug.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include "ModuleScene.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include <string>
using namespace std;

#include <chrono>
using namespace std::chrono;

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(app, start_enabled) {
	debug = false;
}
ModuleDebug::~ModuleDebug() {
}

bool ModuleDebug::Start() {
	debug = false;
	return true;
}

update_status ModuleDebug::Update() {

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug) {

#pragma region Menu navigation

		if (currentScreen == Screen::HOME)
		{
			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
				time = true;

			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
				gravity = true;

			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
				sprites = true;

			if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
				coefficients = true;

			if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
				colliders = true;
		}

		if (time == true)
		{
			currentScreen = Screen::TIME;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && targetFPS < 120)
				targetFPS += 10;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && targetFPS > 10)
				targetFPS -= 10;
		}
		if (gravity == true)
		{
			currentScreen = Screen::GRAVITY;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && App->physics->GRAVITY_Y < 20.0f)
			{
				App->physics->GRAVITY_Y += 1.0f;
				App->physics->world->SetGravity(b2Vec2(GRAVITY_X, App->physics->GRAVITY_Y));
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && App->physics->GRAVITY_Y > -20.0f)
			{
				App->physics->GRAVITY_Y -= 1.0f;
				App->physics->world->SetGravity(b2Vec2(GRAVITY_X, App->physics->GRAVITY_Y));
			}
		}
		if (sprites == true)
		{
			currentScreen = Screen::SPRITES;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && App->scene->ballMultiplier < 4)
				App->scene->ballMultiplier++;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && App->scene->ballMultiplier > 1)
				App->scene->ballMultiplier--;
		}
		if (colliders == true)
		{
			currentScreen = Screen::COLLIDERS;
		}
		if (coefficients == true)
		{
			//This one has the screen activation at the end due to input difficulties

			if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				c_ballRestitution = true;  
				coefficients = false; 
			}
			if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				c_bumperforce = true;
				coefficients = false;
			}
			if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			{
				c_slingshotforce = true;
				coefficients = false;
			}
			if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN && currentScreen == Screen::COEFFICIENTS)
			{
				c_springforce = true;
				coefficients = false;
			}
			if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
			{
				c_flipperforce = true;
				coefficients = false;
			}

			currentScreen = Screen::COEFFICIENTS;
		}
		if (c_ballRestitution == true)
		{
			currentScreen = Screen::C_BALLRESTITUTION;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && ball->pBody->body->GetFixtureList()->GetRestitution() < 1.0f)
				ball->pBody->body->GetFixtureList()->SetRestitution(ball->pBody->body->GetFixtureList()->GetRestitution() + 0.10f);

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && ball->pBody->body->GetFixtureList()->GetRestitution() > 0.0f)
				ball->pBody->body->GetFixtureList()->SetRestitution(ball->pBody->body->GetFixtureList()->GetRestitution() - 0.10f);
		}
		if (c_bumperforce == true)
		{
			currentScreen = Screen::C_BUMPERFORCE;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && bumperU->force < 200)
			{
				bumperU->force += 10;
				bumperL->force += 10;
				bumperR->force += 10;
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && bumperU->force > 0)
			{
				bumperU->force -= 10;
				bumperL->force -= 10;
				bumperR->force -= 10;
			}
		}
		if (c_slingshotforce == true)
		{
			currentScreen = Screen::C_SLINGSHOTFORCE;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && slingshotL->force < 2.0f)
			{
				slingshotL->force += 0.1f;
				slingshotR->force += 0.1f;
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && slingshotL->force > 0.0f)
			{
				slingshotL->force -= 0.1f;
				slingshotR->force -= 0.1f;
			}
		}
		if (c_springforce == true)
		{
			currentScreen = Screen::C_SPRINGFORCE;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && spring->force < 2000)
			{
				if (spring->force < 100)
					spring->force += 10;
				else
					spring->force += 100;

				spring->joint->SetMaxMotorForce(spring->force);
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && spring->force > 0)
			{
				if (spring->force <= 100)
					spring->force -= 10;
				else
					spring->force -= 100;

				spring->joint->SetMaxMotorForce(spring->force);
			}
		}
		if (c_flipperforce == true)
		{
			currentScreen = Screen::C_FLIPPERFORCE;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && flipperL->force < 3000)
			{
				if (flipperL->force < 100)
				{
					flipperL->force += 10;
					flipperR->force += 10;
				}
				else
				{
					flipperL->force += 100;
					flipperR->force += 100;
				}
				flipperR->joint->SetMotorSpeed(flipperL->force);
				flipperR->joint->SetMaxMotorTorque(flipperL->force);
				flipperL->joint->SetMotorSpeed(-flipperL->force);
				flipperL->joint->SetMaxMotorTorque(flipperL->force);
			}

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && flipperL->force > 0)
			{
				if (flipperL->force <= 100)
				{
					flipperL->force -= 10;
					flipperR->force -= 10;
				}
				else
				{
					flipperL->force -= 100;
					flipperR->force -= 100;
				}
				flipperR->joint->SetMotorSpeed(flipperL->force);
				flipperR->joint->SetMaxMotorTorque(flipperL->force);
				flipperL->joint->SetMotorSpeed(-flipperL->force);
				flipperL->joint->SetMaxMotorTorque(flipperL->force);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
		{
			if (currentScreen == Screen::C_BALLRESTITUTION || currentScreen == Screen::C_BUMPERFORCE || currentScreen == Screen::C_SLINGSHOTFORCE || 
				currentScreen == Screen::C_SPRINGFORCE	   || currentScreen == Screen::C_FLIPPERFORCE)
			{
				currentScreen = Screen::COEFFICIENTS;
				c_ballRestitution = false; c_bumperforce = false; c_slingshotforce = false; c_springforce = false; c_flipperforce = false;
				coefficients = true;
			}
			else if (currentScreen == Screen::HOME)
			{
				debug = false;
			}
			else
			{
				currentScreen = Screen::HOME;
				time = false; gravity = false; sprites = false; coefficients = false; colliders = false;
			}
		}

#pragma endregion

		if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
			iPoint position;
			position.x = App->input->GetMouseX();
			position.y = App->input->GetMouseY();
			ball->TeleportTo(position);
		}

		if (App->physics->mouse_body != nullptr && App->physics->mouse_joint != nullptr)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				// Get new mouse position and re-target mouse_joint there
				b2Vec2 mousePosition;
				mousePosition.x = PIXEL_TO_METERS(App->input->GetMouseX());
				mousePosition.y = PIXEL_TO_METERS(App->input->GetMouseY());
				App->physics->mouse_joint->SetTarget(mousePosition);
			}
		}
		if (App->physics->mouse_body != nullptr && App->physics->mouse_joint != nullptr)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			{
				// Tell Box2D to destroy the mouse_joint
				App->physics->world->DestroyJoint(App->physics->mouse_joint);

				App->physics->mouse_joint = nullptr;
				App->physics->mouse_body = nullptr;
			}
		}
	}
	if (App->physics->mouse_body != nullptr && App->physics->mouse_joint != nullptr && !debug ||
		App->physics->mouse_body != nullptr && App->physics->mouse_joint != nullptr && currentScreen != Screen::COLLIDERS)
	{
		// Tell Box2D to destroy the mouse_joint
		App->physics->world->DestroyJoint(App->physics->mouse_joint);

		App->physics->mouse_joint = nullptr;
		App->physics->mouse_body = nullptr;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleDebug::PostUpdate() {
	
	if (debug)
	{
		DebugDraw();
	}
	return UPDATE_CONTINUE;
}

void ModuleDebug::DebugDraw() {
	
	//Physics objects
	// This will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	if (colliders)
	{
		for (b2Body* b = App->physics->world->GetBodyList(); b; b = b->GetNext())
		{
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				switch (f->GetType())
				{
					// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for (int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 50, 50);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 50, 50);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 0, 255, 0);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 0, 255, 0);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 0, 0, 255);
				}
				break;
				}

				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					// test if the current body contains mouse position
					b2Vec2 p = { PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) };
					if (f->GetShape()->TestPoint(b->GetTransform(), p) == true)
					{

						// If a body was selected we will attach a mouse joint to it
						// so we can pull it around

						//Create a mouse joint using mouse_joint class property

						// We need to keep this body throughout several game frames; you cannot define it as a local variable here. 
						App->physics->mouse_body = b;

						// Get current mouse position
						b2Vec2 mousePosition;
						mousePosition.x = p.x;
						mousePosition.y = p.y;

						// Define new mouse joint
						b2MouseJointDef def;
						def.bodyA = App->physics->worldBody; // First body must be a static ground
						def.bodyB = App->physics->mouse_body; // Second body will be the body to attach to the mouse
						def.target = mousePosition; // The second body will be pulled towards this location
						def.dampingRatio = 0.5f; // Play with this value
						def.frequencyHz = 2.0f; // Play with this value
						def.maxForce = 200.0f * App->physics->mouse_body->GetMass(); // Play with this value

						// Add the new mouse joint into the World
						App->physics->mouse_joint = (b2MouseJoint*)App->physics->world->CreateJoint(&def);
					}
				}
			}
		}
		if (App->physics->mouse_body != nullptr && App->physics->mouse_joint != nullptr)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				// Draw a red line between both anchor points
				App->renderer->DrawLine(METERS_TO_PIXELS(App->physics->mouse_body->GetPosition().x), METERS_TO_PIXELS(App->physics->mouse_body->GetPosition().y), App->input->GetMouseX(), App->input->GetMouseY(), 255, 0, 0);
			}
		}
	}

	SDL_Rect bg;
	if (time)					{ bg = { 2,38,251,92 }; }
	else if (gravity)			{ bg = { 2,38,251,82 }; }
	else if (sprites)			{ bg = { 2,38,251,62 }; }
	else if (coefficients)		{ bg = { 2,38,251,112 }; }
	else if (colliders)			{ bg = { 2,38,251,22 }; }
	else if (c_ballRestitution)	{ bg = { 2,38,251,82 }; }
	else if (c_bumperforce)		{ bg = { 2,38,251,82 }; }
	else if (c_slingshotforce)	{ bg = { 2,38,251,82 }; }
	else if (c_springforce)		{ bg = { 2,38,251,82 }; }
	else if (c_flipperforce)	{ bg = { 2,38,251,82 }; }
	else						{ bg = { 2,38,252,72 }; }
	App->renderer->DrawQuad(bg, 0, 0, 0, 125, true);

	switch (currentScreen)
	{
	case Screen::HOME:
		App->fonts->BlitText(5, 40, 0, "PRESS A NUMBER TO OPEN ITS MENU");

		App->fonts->BlitText(5, 60, 0, "1. TIME OPTIONS");
		App->fonts->BlitText(5, 70, 0, "2. GRAVITY OPTIONS");
		App->fonts->BlitText(5, 80, 0, "3. SPRITES OPTIONS");
		App->fonts->BlitText(5, 90, 0, "4. COEFFICIENT OPTIONS");
		App->fonts->BlitText(5, 100, 0, "5. SHOW COLLIDERS");
		break;
		
	case Screen::TIME:
		App->fonts->BlitText(5, 40, 0, "TIME OPTIONS");
		App->fonts->BlitText(5, 60, 0, "PRESS W TO INCREASE AND");
		App->fonts->BlitText(5, 70, 0, "S TO DECREASE THE VALUE");

		LOG("Elapsed Cycle:");
		LOG(std::to_string(elapsedCycle.count()).c_str());
		LOG("Elapsed Frame:");
		LOG(std::to_string(elapsedFrame.count()).c_str());

		App->fonts->BlitText(5, 90, 0, "TARGET FPS ");
		App->fonts->BlitText(100, 90, 0, std::to_string(targetFPS).c_str());
		App->fonts->BlitText(5, 100, 0, "CURRENT FPS ");
		App->fonts->BlitText(100, 100, 0, std::to_string(FPS).c_str());
		LOG("FPS:");
		LOG(std::to_string(FPS).c_str());

		App->fonts->BlitText(5, 120, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::GRAVITY:
		App->fonts->BlitText(5, 40, 0, "GRAVITY OPTIONS");
		App->fonts->BlitText(5, 60, 0, "PRESS W TO INCREASE AND");
		App->fonts->BlitText(5, 70, 0, "S TO DECREASE THE VALUE");

		App->fonts->BlitText(5, 90, 0, "GRAVITY ");
		App->fonts->BlitText(120, 90, 0, std::to_string(App->physics->GRAVITY_Y).c_str());

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::SPRITES:
		App->fonts->BlitText(5, 40, 0, "SPRITE OPTIONS");
		App->fonts->BlitText(5, 60, 0, "PRESS W TO INCREASE AND S TO");
		App->fonts->BlitText(5, 70, 0, "DECREASE THE BALL LEVEL");
		App->fonts->BlitText(5, 90, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::COEFFICIENTS:
		App->fonts->BlitText(5, 40, 0, "COEFFICIENT OPTIONS");
		App->fonts->BlitText(5, 60, 0, "PRESS A NUMBER TO SELECT AN");
		App->fonts->BlitText(5, 70, 0, "OPTION");
		App->fonts->BlitText(5, 80, 0, "1. BALL RESTITUTION");
		App->fonts->BlitText(5, 90, 0, "2. BUMPERS FORCE");
		App->fonts->BlitText(5, 100, 0, "3. SLINGSHOTS FORCE");
		App->fonts->BlitText(5, 110, 0, "4. SPRING FORCE");
		App->fonts->BlitText(5, 120, 0, "5. FLIPPERS FORCE");

		App->fonts->BlitText(5, 140, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::C_BALLRESTITUTION:
		App->fonts->BlitText(5, 40, 0, "BALL RESTITUTION");
		App->fonts->BlitText(5, 60, 0, "PRESS W TO INCREASE AND S TO");
		App->fonts->BlitText(5, 70, 0, "DECREASE THE BALL RESTITUTION");

		App->fonts->BlitText(5, 90, 0, "CURRENT RESTITUTION");
		App->fonts->BlitText(170, 90, 0, std::to_string(ball->pBody->body->GetFixtureList()->GetRestitution()).c_str());

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;
	case Screen::C_BUMPERFORCE:
		App->fonts->BlitText(5, 40, 0, "BUMPERS FORCE");
		App->fonts->BlitText(5, 60, 0, "PRESS W TO INCREASE AND S TO");
		App->fonts->BlitText(5, 70, 0, "DECREASE THE BUMPERS FORCE");

		App->fonts->BlitText(5, 90, 0, "CURRENT FORCE");
		App->fonts->BlitText(170, 90, 0, std::to_string(bumperU->force).c_str());

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;
	case Screen::C_SLINGSHOTFORCE:
		App->fonts->BlitText(5, 40, 0, "SLINGSHOTS FORCE");
		App->fonts->BlitText(5, 60, 0, "PRESS W TO INCREASE AND S TO");
		App->fonts->BlitText(5, 70, 0, "DECREASE THE SLINGSHOTS FORCE");

		App->fonts->BlitText(5, 90, 0, "CURRENT FORCE");
		App->fonts->BlitText(170, 90, 0, std::to_string(slingshotL->force).c_str());

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;
	case Screen::C_SPRINGFORCE:
		App->fonts->BlitText(5, 40, 0, "SPRING FORCE");
		App->fonts->BlitText(5, 60, 0, "PRESS W TO INCREASE AND S TO");
		App->fonts->BlitText(5, 70, 0, "DECREASE THE SPRING FORCE");

		App->fonts->BlitText(5, 90, 0, "CURRENT FORCE");
		App->fonts->BlitText(170, 90, 0, std::to_string(spring->force).c_str());

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;
	case Screen::C_FLIPPERFORCE:
		App->fonts->BlitText(5, 40, 0, "FLIPPERS FORCE");
		App->fonts->BlitText(5, 60, 0, "PRESS W TO INCREASE AND S TO");
		App->fonts->BlitText(5, 70, 0, "DECREASE THE FLIPPERS FORCE");

		App->fonts->BlitText(5, 90, 0, "CURRENT FORCE");
		App->fonts->BlitText(170, 90, 0, std::to_string(flipperL->force).c_str());

		App->fonts->BlitText(5, 110, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::COLLIDERS:
		App->fonts->BlitText(5, 40, 0, "CLICK ON THE BALL TO DRAG IT");
		App->fonts->BlitText(5, 50, 0, "PRESS BACKSPACE TO GO BACK");
		break;

	case Screen::NONE:
		break;
	default:
		break;
	}
}