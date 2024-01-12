#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "FileSystemManager.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG(LogTypeCase::L_CASUAL, "Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG(LogTypeCase::L_CASUAL, "SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch(e.type)
		{
			case SDL_DROPFILE:
				LOG(LogTypeCase::L_CASUAL, "File To Import path: %s", e.drop.file);
				FileSystem::ReadFyleType(e.drop.file);
				SDL_free(e.drop.file);
			break;

			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	if (quit == true /*||keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP*/)
	{
		if (!App->editor->isMovingParent)
		{
			return UPDATE_STOP;
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG(LogTypeCase::L_CASUAL, "Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

float3 ModuleInput::GetMousePosition()
{
	return float3(mouse_x, mouse_y, mouse_z);
}

void ModuleInput::ReadInput(string& word)
{
	if(GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		word.append("0");
	}
	
	if(GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		word.append("1");
	}
	
	if(GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		word.append("2");
	}
	
	if(GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		word.append("3");
	}
	
	if(GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		word.append("4");
	}
	
	if(GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	{
		word.append("5");
	}
	
	if(GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	{
		word.append("6");
	}
	
	if(GetKey(SDL_SCANCODE_7) == KEY_DOWN)
	{
		word.append("7");
	}
	
	if(GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		word.append("8");
	}
	
	if(GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		word.append("9");
	}

	if (GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		word.append("Q");
	}
	
	if (GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		word.append("W");
	}

	if (GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		word.append("E");
	}

	if (GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		word.append("R");
	}

	if (GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		word.append("T");
	}

	if (GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
	{
		word.append("Y");
	}

	if (GetKey(SDL_SCANCODE_U) == KEY_DOWN)
	{
		word.append("U");
	}

	if (GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		word.append("I");
	}

	if (GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		word.append("O");
	}

	if (GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		word.append("P");
	}

	if (GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		word.append("W");
	}

	if (GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		word.append("A");
	}
	
	if (GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		word.append("S");
	}
	
	if (GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		word.append("D");
	}
	
	if (GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		word.append("F");
	}
	
	if (GetKey(SDL_SCANCODE_G) == KEY_DOWN)
	{
		word.append("G");
	}
	
	if (GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		word.append("H");
	}
	
	if (GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		word.append("J");
	}
	
	if (GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		word.append("K");
	}
	
	if (GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		word.append("L");
	}
	
	if (GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		word.append("Z");
	}
	
	if (GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		word.append("X");
	}
	
	if (GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		word.append("C");
	}
	
	if (GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		word.append("V");
	}
	
	if (GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		word.append("B");
	}
	
	if (GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		word.append("N");
	}
	
	if (GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		word.append("M");
	}
	
	if (GetKey(SDL_SCANCODE_COMMA) == KEY_DOWN)
	{
		word.append(",");
	}
	
	if (GetKey(SDLK_SEMICOLON) == KEY_DOWN)
	{
		word.append(";");
	}
	
	if (GetKey(SDLK_SLASH) == KEY_DOWN)
	{
		word.append("/");
	}
	
	if (GetKey(SDLK_PERIOD) == KEY_DOWN)
	{
		word.append(".");
	}
	
	if (GetKey(SDLK_COLON) == KEY_DOWN)
	{
		word.append(":");
	}
	
	if (GetKey(SDLK_UNDERSCORE) == KEY_DOWN)
	{
		word.append("_");
	}
	
	if (GetKey(SDLK_MINUS) == KEY_DOWN)
	{
		word.append("-");
	}
	
	if (GetKey(SDLK_LEFTBRACKET) == KEY_DOWN)
	{
		word.append("[");
	}
	
	if (GetKey(SDLK_RIGHTBRACKET) == KEY_DOWN)
	{
		word.append("]");
	}
	
	if (GetKey(SDLK_AMPERSAND) == KEY_DOWN)
	{
		word.append("&");
	}
	
	if (GetKey(SDLK_AT) == KEY_DOWN)
	{
		word.append("@");
	}
	
	if (GetKey(SDLK_DOLLAR) == KEY_DOWN)
	{
		word.append("$");
	}

	if(GetKey(SDLK_QUESTION) == KEY_DOWN)
	{
		word.append("?");
	}
	
	if(GetKey(SDL_SCANCODE_APOSTROPHE) == KEY_DOWN)
	{
		word.append("'");
	}
	
	if(GetKey(SDLK_CARET) == KEY_DOWN)
	{
		word.append("^");
	}
	
	if(GetKey(SDLK_PLUS) == KEY_DOWN)
	{
		word.append("+");
	}
	
	if(GetKey(SDLK_EQUALS) == KEY_DOWN)
	{
		word.append("=");
	}
	
	/*if(GetKey(SDLK_ASTERISK) == KEY_DOWN)
	{
		word.append("*");
	}*/
	
	if(GetKey(SDLK_EXCLAIM) == KEY_DOWN)
	{
		word.append("!");
	}	
	
	if(GetKey(SDLK_HASH) == KEY_DOWN)
	{
		word.append("#");
	}
	
	if(GetKey(SDLK_LEFTPAREN) == KEY_DOWN)
	{
		word.append("(");
	}
	
	if(GetKey(SDLK_PERCENT) == KEY_DOWN)
	{
		word.append("%");
	}
	
	if(GetKey(SDLK_RIGHTPAREN) == KEY_DOWN)
	{
		word.append(")");
	}
	
	//if(GetKey(SDLK_QUOTEDBL) == KEY_DOWN)
	//{
	//	word.append(""");
	//}
	
	if(GetKey(SDLK_LESS) == KEY_DOWN)
	{
		word.append("<");
	}
	
	if(GetKey(SDLK_GREATER) == KEY_DOWN)
	{
		word.append(">");
	}
}
