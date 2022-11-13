#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "ModuleDebug.h"
#include <chrono>
using namespace std::chrono;
#include <thread>

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

Application* App = NULL;

int main(int argc, char ** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		high_resolution_clock::time_point start = high_resolution_clock::now();

		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

		//Time per cycle
		high_resolution_clock::time_point endCycle = high_resolution_clock::now();
		App->debug->elapsedCycle = duration_cast<microseconds>(endCycle - start);

		//This is to cap FPS, the diplaying of FPS on screen is calculated underneath
		if (App->debug->elapsedCycle < std::chrono::microseconds(16666))
		{
			std::this_thread::sleep_for(std::chrono::microseconds(std::chrono::microseconds(16666) - App->debug->elapsedCycle));
		}

		//Time per cycle + delay
		high_resolution_clock::time_point endFrame = high_resolution_clock::now();
		App->debug->elapsedFrame = duration_cast<microseconds>(endFrame - start);

		//Calculate FPSs
		App->debug->FPS = 1 / ((double)App->debug->elapsedFrame.count() * 10E-7);
	}

	delete App;
	LOG("Exiting game '%s'...\n", TITLE);
	return main_return;
}