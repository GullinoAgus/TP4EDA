#include <iostream>
#include <string>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_image.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_allegro5.h>

#include "Gui.h"
#include "config.h"

#define DISPLAY_WIDTH				1120	// Set like this to fit my screen :) 
#define DISPLAY_HEIGHT				696	

#define BACKGROUND_DISPLACEMENT_X	410.0	// See comment in DISPLAY_WIDTH.
#define BACKGROUND_DISPLACEMENT_Y	0.0

#define COLOR_BACKGROUND_RGB		200,100,050		// Arguments for al_map_rgb 

#define PLAYABLE_AREA_X1			(702.0-BACKGROUND_DISPLACEMENT_X-(WORM_WIDTH/2))
#define PLAYABLE_AREA_Y1			(395.0-BACKGROUND_DISPLACEMENT_Y)
#define PLAYABLE_AREA_X2			(1211.0-BACKGROUND_DISPLACEMENT_X)
#define PLAYABLE_AREA_Y2			(616.0-BACKGROUND_DISPLACEMENT_Y)

#define ARRSIZE(a)					(sizeof(a)/sizeof(a[0]))

enum IsaacKeys
{
	ISAAC_F_KEY = 0,
	ISAAC_N_KEYS = KEYS_BY_WORM,
	ISAAC_KEY_LEFT = ALLEGRO_KEY_A,
	ISAAC_KEY_UP = ALLEGRO_KEY_W,
	ISAAC_KEY_RIGHT = ALLEGRO_KEY_D,
};

enum RayKeys
{
	RAY_F_KEY = ISAAC_N_KEYS,
	RAY_N_KEYS = KEYS_BY_WORM,
	RAY_KEY_LEFT = ALLEGRO_KEY_LEFT,
	RAY_KEY_UP = ALLEGRO_KEY_UP,
	RAY_KEY_RIGHT = ALLEGRO_KEY_RIGHT,
};


/* X, Y, FLIP */
const int spectators[][3]
{
	{722, 176, 1},
	{752, 176, 1},
	{786, 176, 1},
	{816, 178, 1},
	{862, 178, 1},
	{1030,178, 0},
	{1070,178, 0},
	{1100,176, 0},
	{1140,176, 0},
	{1170,176, 0},
};

using namespace std;

Gui::Gui() 
	: world({PLAYABLE_AREA_X2-PLAYABLE_AREA_X1-(double)WORM_WIDTH, PLAYABLE_AREA_Y2-PLAYABLE_AREA_Y1})
{

	string dirWalkingTexts = "Resources\\wwalking\\wwalk-F";
	string dirJumpingTexts = "Resources\\wjump\\wjump-F";

	if (!this->initAllegro())
		return;

	if (!this->initImGui())
		return;

	this->closeWindow = false;

	for (int i = WF1; i < N_FRAMES; i++)
	{
		if (i <= WF15 )
		{
			//												+ 1 because namefiles start in 1 :)
			this->wormTextArr[i] = Bitmap(&(dirWalkingTexts + to_string(i + 1) + ".png")[0]);
		}
		else
		{
			this->wormTextArr[i] = Bitmap(&(dirJumpingTexts + to_string(i - WF15) + ".png")[0]);
		}
		
		if (this->wormTextArr[i].bitmap == NULL)
		{
			cout << "Bitmap " << i << " could not be loaded." << endl;
			return;
		}
	}
	this->worldText = Bitmap("Resources\\Scenario.png");
	if (this->worldText.bitmap == NULL)
	{
		cout << "Bitmap could not be loaded." << endl;
		return;
	}

	this->spectatorsBitmap = WF1;
	this->spectatorsBitmapForward = true;

	// Keyboard
	this->validKeyCodes[0] = ISAAC_KEY_LEFT;
	this->validKeyCodes[1] = ISAAC_KEY_UP;
	this->validKeyCodes[2] = ISAAC_KEY_RIGHT;
	this->validKeyCodes[3] = RAY_KEY_LEFT;
	this->validKeyCodes[4] = RAY_KEY_UP;
	this->validKeyCodes[5] = RAY_KEY_RIGHT;

	for (int i = 0; i < WORM_KEYS ; i++)
	{
		this->keysDown[i] = false;
		this->keysDownTime[i] = 0;
	}
}

Gui::~Gui(void)
{
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();

	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_shutdown_primitives_addon();
	al_shutdown_image_addon();

	if (this->timer.fps != NULL) al_destroy_timer(this->timer.fps);
	if (this->timer.spectators != NULL) al_destroy_timer(this->timer.spectators);
    if (this->evQueue != NULL) al_destroy_event_queue(this->evQueue);
    if (this->display != NULL) al_destroy_display(this->display);
}

bool Gui::initAllegro(void)
{
	if (!al_init()) {
		fprintf(stderr, " failed to initialize allegro !\n");
		return false;
	}

	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize primitives addon!\n");
		return false;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize keyboard!\n");
		return false;
	}

	if (!al_install_mouse()) {
		fprintf(stderr, "failed to initialize mouse!\n");
		return false;
	}

	if (!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize image addon!\n");
		return false;
	}

	this->display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	if (this->display == NULL)
	{
		fprintf(stderr, "Unable to create display. Aborting.\n");
		return false;
	}

	this->evQueue = al_create_event_queue();
	if (this->evQueue == NULL)
	{
		fprintf(stderr, "Unable to create event queue.\n");
		return false;
	}
	al_register_event_source(this->evQueue, al_get_display_event_source(this->display));
	al_register_event_source(this->evQueue, al_get_keyboard_event_source());
	al_register_event_source(this->evQueue, al_get_mouse_event_source());

	return true;
}

bool Gui::initImGui(void)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGui::StyleColorsDark();

	ImGui_ImplAllegro5_Init(this->display);

	return true;
}

bool Gui::show(void)
{
	if (this->display == NULL)
	{
		cout << "Display has not been initialized." << endl;
		return false;
	}
	else if (this->evQueue == NULL)
	{
		cout << "Event queue has not been initialized." << endl;
		return false;
	}
	else if (this->worldText.bitmap == NULL)
	{
		cout << "Background has not been loaded." << endl;
		return false;
	}
	else
	{
		for (int i = WF1; i < N_FRAMES; i++)
		{
			if (this->wormTextArr[i].bitmap == NULL)
			{
				cout << "Sprite number " << i << " has not been loaded." << endl;
				return false;
			}
		}
	}

	// Create and register timers
	this->timer.fps = al_create_timer(1.0 / FPS);
	if (this->timer.fps == NULL)
	{
		fprintf(stderr, "Unable to create timer. Aborting.\n");
		return false;
	}
	al_register_event_source(this->evQueue, al_get_timer_event_source(this->timer.fps));

	this->timer.spectators = al_create_timer(15.0/FPS);
	if (this->timer.spectators == NULL)
	{
		fprintf(stderr, "Unable to create timer. Aborting.\n");
		return false;
	}
	al_register_event_source(this->evQueue, al_get_timer_event_source(this->timer.spectators));


	// Start timers
	al_start_timer(this->timer.fps);
	al_start_timer(this->timer.spectators);

	ALLEGRO_EVENT ev;
	while (!this->closeWindow)
	{
		while (al_get_next_event(this->evQueue, &ev))
		{
			eventDispatcher(ev);
		}
	}

	return true;
}

void Gui::eventDispatcher(ALLEGRO_EVENT& ev)
{
	//ImGui_ImplAllegro5_ProcessEvent(&ev);
	int index = -1;
	switch (ev.type)
	{
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			closeWindow = true;
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			index = this->getKeyIndexByCode(ev.keyboard.keycode);
			if (index >= 0 && noKeyPressed(index))
			{
				this->keysDown[index] = true;
			}
			
			break;

		case ALLEGRO_EVENT_KEY_UP:
			index = this->getKeyIndexByCode(ev.keyboard.keycode);
			if (index >= 0)
			{
				this->keysDown[index] = false;

				// Only stops movement when key was not pressed long enough
				if (isgreater(this->keysDownTime[index], 0.0) && islessequal(this->keysDownTime[index], WORM_IDLE_TIME)) 
				{
					this->keysDownTime[index] = 0;
					this->stopMoving(index);
				}
			}

			break;

		case ALLEGRO_EVENT_TIMER:
			if (ev.timer.source == this->timer.fps)
			{
				// Increase key-pressed timer
				for (int key = 0; key < WORM_KEYS; key++)
				{
					// New key-press event
					if (this->keysDown[key] && islessequal(this->keysDownTime[key], 0.0))
					{
						if (!startMoving(key)) startJumping(key);

						this->keysDownTime[key] += al_get_timer_speed(this->timer.fps);
					}
					// Key pressed for too long, clear it (will be reset in next tick)
					else if (isgreater(this->keysDownTime[key], 1.0))
					{
						this->keysDownTime[key] = 0.0;
						this->stopMoving(key);
					}
					// Key pressed or movement in progress
					else if (this->keysDown[key] || isgreater(this->keysDownTime[key], 0.0))
					{
						this->keysDownTime[key] += al_get_timer_speed(this->timer.fps);
					}
				}
				this->wormEvent();


				this->drawWorld();
#ifdef DEBUG
				this->drawPlayableBox();
#endif
				this->drawWorms();
				al_flip_display();
			}
			if (ev.timer.source == this->timer.spectators) 
			{
				this->updateSpectatorsBitmap();
			}
			break;

		default:
			break;
	}
}

bool Gui::startMoving(int keyIndex)
{
	bool validKey = true;

	switch (this->validKeyCodes[keyIndex])
	{
		case ISAAC_KEY_LEFT:
			world.warmUpWorm(WormsByName::Isaac, WormActions::WALK, LEFT);
			break;

		case ISAAC_KEY_RIGHT:
			world.warmUpWorm(WormsByName::Isaac, WormActions::WALK, RIGHT);
			break;

		case RAY_KEY_LEFT:
			world.warmUpWorm(WormsByName::Ray, WormActions::WALK, LEFT);
			break;

		case RAY_KEY_RIGHT:
			world.warmUpWorm(WormsByName::Ray, WormActions::WALK, RIGHT);
			break;

		default:
			validKey = false;
			break;
	}

	return validKey;
}

bool Gui::startJumping(int keyIndex)
{
	bool validKey = true;

	switch (this->validKeyCodes[keyIndex])
	{
		case ISAAC_KEY_UP:
			world.warmUpWorm(WormsByName::Isaac, WormActions::JUMP, NOT_INIT);
			break;

		case RAY_KEY_UP:
			world.warmUpWorm(WormsByName::Ray, WormActions::JUMP, NOT_INIT);
			break;

		default:
			validKey = false;
			break;
	}

	return validKey;
}

bool Gui::stopMoving(int keyIndex)
{
	int keyCode = getKeyCodeByIndex(keyIndex);
	if (keyCode == ISAAC_KEY_LEFT || keyCode == ISAAC_KEY_RIGHT)
	{
		world.forceWormStop(WormsByName::Isaac);
	}

	if (keyCode == RAY_KEY_LEFT || keyCode == RAY_KEY_RIGHT)
	{
		world.forceWormStop(WormsByName::Ray);
	}
	return true;
}

bool Gui::wormEvent()
{
	for (int key = 0; key < WORM_KEYS; key++)
	{
		if (isgreater(keysDownTime[key], 0.0))
		{
#ifdef DEBUG
			cout << "key: " << key << " time: " << keysDownTime[key] << endl;
#endif // DEBUG

			if (key < ISAAC_N_KEYS)
			{
				world.update(WormsByName::Isaac);
			}
			else
			{
				world.update(WormsByName::Ray);
			}
		}
	}

	return true;
}

int Gui::getKeyIndexByCode(int allegro_key_code)
{
	for (int i = 0; i < WORM_KEYS; i++)
	{
		if (this->validKeyCodes[i] == allegro_key_code)
		{
			return i;
		}
	}
	return -1;
}

int Gui::getKeyCodeByIndex(int keyIndex)
{
	if (keyIndex < 0 || keyIndex >= WORM_KEYS) return -1;
	return this->validKeyCodes[keyIndex];
}


bool Gui::onlyOneWormKeyPressed(WormsByName worm)
{
	bool onePressed = false;

	int startIndex = 0, endIndex = 0;
	if (worm == WormsByName::Isaac)
	{
		startIndex = ISAAC_F_KEY;
		endIndex = ISAAC_N_KEYS + startIndex;
	}
	else if (worm == WormsByName::Ray)
	{
		startIndex = RAY_F_KEY;
		endIndex = RAY_N_KEYS + startIndex;
	}

	for (int key = startIndex; key < endIndex; key++)
	{
		if ((this->keysDown[key] || isgreater(this->keysDownTime[key], 0.0)))
		{
			if (onePressed)
			{
				return false;
			}

			onePressed = true;
		}
	}

	return onePressed;
}

/* True if no other key has been pressed for the worm with this key */
bool Gui::noKeyPressed(int keyIndex)
{
	if (keyIndex > WORM_KEYS) return false;

	bool noPressed = true;
	int startIndex = keyIndex < ISAAC_N_KEYS ? ISAAC_F_KEY : RAY_F_KEY;
	int endIndex = keyIndex < ISAAC_N_KEYS ? ISAAC_N_KEYS : RAY_N_KEYS;
	
	endIndex += startIndex;

	for (int key = startIndex; key < endIndex; key++)
	{
		if ((this->keysDown[key] || isgreater(this->keysDownTime[key], 0.0)))
		{
			noPressed = false;
		}
	}

	return noPressed;
}

bool Gui::drawWorld(void)
{
	al_clear_to_color(al_map_rgb(COLOR_BACKGROUND_RGB));
	// Bitmap is drawn this way so the display can fit in my monitor :)
	al_draw_bitmap_region(this->worldText.bitmap, 
		BACKGROUND_DISPLACEMENT_X, BACKGROUND_DISPLACEMENT_Y, 
		DISPLAY_WIDTH, DISPLAY_HEIGHT,
		0, 0, 0);

	this->drawSpectators();

	return true;
}

bool Gui::drawWorms(void)
{
	Point_t position = { 0 };
	Worm* wrmarr = world.getWormArr();

	for (int i = 0; i < MAX_WORMS; i++)
	{
		wrmarr[i].getCurrentPosition(position.x, position.y);


		position.x += PLAYABLE_AREA_X1;	// Set X = 0 relative to playable area
		position.y += PLAYABLE_AREA_Y2;	// Set Y = 0 relative to playable area

#ifdef DEBUG
		WormPointing pointing = wrmarr[i].getPointingDirection();
		Point_t pointVertex = { 0 };
		pointVertex.x = position.x;
		pointVertex.y = position.y + (WORM_HEIGHT / 2);

		if (pointing == LEFT)
		{
			pointVertex.x -= WORM_WIDTH;
		}
		else if (pointing == RIGHT)
		{
			pointVertex.x += WORM_WIDTH;
		}
		al_draw_triangle(position.x, position.y,
			position.x, position.y + WORM_HEIGHT,
			pointVertex.x, pointVertex.y,
			al_map_rgb(255, 0, 0), 2.0);
#endif

		int flip = wrmarr[i].getPointingDirection() == LEFT ? 0 : ALLEGRO_FLIP_HORIZONTAL;
		int frame = wrmarr[i].getFrame();

		al_draw_bitmap(this->wormTextArr[frame].bitmap, position.x, position.y, flip);

	}

	return true;
}

bool Gui::drawSpectators(void)
{
	int flip = 0;
	for (int i = 0; i < ARRSIZE(spectators); i++)
	{
		flip = spectators[i][2] ? ALLEGRO_FLIP_HORIZONTAL : 0;
		al_draw_bitmap(this->wormTextArr[this->spectatorsBitmap].bitmap, 
			spectators[i][0]-BACKGROUND_DISPLACEMENT_X, 
			spectators[i][1]-2*WORM_HEIGHT/3-BACKGROUND_DISPLACEMENT_Y, flip);
	}
	

	return true;
}

void Gui::updateSpectatorsBitmap(void)
{
	if (this->spectatorsBitmap == WF4)
	{
		this->spectatorsBitmapForward = false;
	}
	else if (this->spectatorsBitmap == WF1)
	{
		this->spectatorsBitmapForward = true;
	}

	this->spectatorsBitmapForward ? this->spectatorsBitmap++ : this->spectatorsBitmap--;

	return;
}

#ifdef DEBUG
bool Gui::drawPlayableBox(void)
{
	al_draw_rectangle(PLAYABLE_AREA_X1, PLAYABLE_AREA_Y1,
		PLAYABLE_AREA_X2, PLAYABLE_AREA_Y2,
		al_map_rgb(0, 0, 255), 2.0);

	al_draw_rectangle(PLAYABLE_AREA_X1, PLAYABLE_AREA_Y2,
		PLAYABLE_AREA_X2, PLAYABLE_AREA_Y2 + WORM_HEIGHT,
		al_map_rgb(0, 255, 0), 2.0);

	return true;
}
#endif
