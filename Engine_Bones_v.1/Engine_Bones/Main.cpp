#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

#include "mmgr/mmgr.h"

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char ** argv)
{
	LOG(LogTypeCase::L_CASUAL, "Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG(LogTypeCase::L_CASUAL, "-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG(LogTypeCase::L_CASUAL, "-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG(LogTypeCase::L_CASUAL, "Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG(LogTypeCase::L_CASUAL, "-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG(LogTypeCase::L_CASUAL, "Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
			break;

		case MAIN_FINISH:

			LOG(LogTypeCase::L_CASUAL, "-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG(LogTypeCase::L_CASUAL, "Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	int leaks = MAX(0, m_getMemoryStatistics().totalAllocUnitCount - 23);
	delete App;
	LOG(LogTypeCase::L_CASUAL, "With %d memory leaks!\n", (leaks > 0) ? leaks : 0);
	LOG(LogTypeCase::L_CASUAL, "Exiting game '%s'...\n", TITLE);
	return main_return;
}