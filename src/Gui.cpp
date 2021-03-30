#include <iostream>
#include <string>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h> 
#include <allegro5/allegro_image.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_allegro5.h>

#include "Gui.h"
#include "resource.h"

#define DISPLAY_WIDTH	1120		//TODO: Adjust to background. 
#define DISPLAY_HEIGHT	696			//		Set like this to fit my screen :) 

#define BACKGROUND_DISPLACEMENT_X	410.0	// See todo in previous line.
#define BACKGROUND_DISPLACEMENT_Y	0.0

#define PLAYABLE_AREA_X1	(702.0-BACKGROUND_DISPLACEMENT_X)
#define PLAYABLE_AREA_Y1	(395.0-BACKGROUND_DISPLACEMENT_Y)
#define PLAYABLE_AREA_X2	(1212.0-BACKGROUND_DISPLACEMENT_X)
#define PLAYABLE_AREA_Y2	(616.0-BACKGROUND_DISPLACEMENT_Y)

#define FPS				50

using namespace std;

Gui::Gui() 
	: world({PLAYABLE_AREA_X2-PLAYABLE_AREA_X1, PLAYABLE_AREA_Y2-PLAYABLE_AREA_Y1})
{

	string dirWalkingTexts = "Resources\\wwalking\\wwalk-F";
	string dirJumpingTexts = "Resources\\wjump\\wjump-F";
	
	Worm* p2worm = world.getWormArr();

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

	this->io = &(ImGui::GetIO()); (void)io;

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

	// Start timers
	al_start_timer(this->timer.fps);

	while (!this->closeWindow)
	{
		while (al_get_next_event(this->evQueue, &this->ev))
		{
			ImGui_ImplAllegro5_ProcessEvent(&ev);
			switch (ev.type)
			{
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					closeWindow = true;
				break;

				/*case ALLEGRO_EVENT_DISPLAY_RESIZE:
					ImGui_ImplAllegro5_InvalidateDeviceObjects();
					al_acknowledge_resize(display);
					ImGui_ImplAllegro5_CreateDeviceObjects();
					break;
				*/
				case ALLEGRO_EVENT_TIMER:
					if (ev.timer.source == this->timer.fps)
					{
						this->drawWorld();
#ifdef DEBUG
						this->drawPlayableBox();
#endif
						this->drawWorms();
						al_flip_display();
					}
					break;

				default:
					break;
			}
		}
	}

	return true;
}

bool Gui::drawWorld(void)
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	// Bitmap is drawn this way so the display can fit in my monitor :)
	al_draw_bitmap_region(this->worldText.bitmap, 
		BACKGROUND_DISPLACEMENT_X, BACKGROUND_DISPLACEMENT_Y, 
		DISPLAY_WIDTH, DISPLAY_HEIGHT,
		0, 0, 0);
	/*al_draw_bitmap(this->worldText.bitmap, 0, 0, 0);*/
	return true;
}

bool Gui::drawWorms(void)
{
	Point_t position = { 0 };
	Worm * wrmarr = world.getWormArr();

	for (int i = 0; i < MAX_WORMS; i++)
	{
		wrmarr[i].getCurrentPosition(position.x, position.y);
		WormPointing pointing = wrmarr[i].getPointingDirection();
		
		Point_t pointVertex = { 0 };

		position.x += PLAYABLE_AREA_X1;	// Set X = 0 relative to playable area
		position.y += PLAYABLE_AREA_Y2 - WORM_HEIGHT;	// Set Y = 0 relative to playable area
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
						al_map_rgb(255,0,0), 2.0);
	}

	return true;
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