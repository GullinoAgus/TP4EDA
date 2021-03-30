#ifndef _GUI_H_
#define _GUI_H_

#include "World.h"
#include "Bitmap.h"
#include <allegro5/allegro5.h>

class Gui
{
public:
	Gui();
	~Gui();

	bool show();

private:
	bool closeWindow;

	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* evQueue;
	ALLEGRO_EVENT ev;
	Bitmap worldText;
	Bitmap wormTextArr[N_FRAMES];

	struct
	{
		ALLEGRO_TIMER* fps;
	} timer;

	struct ImGuiIO* io;

	World world;

	bool initAllegro();
	bool initImGui(void);

	bool drawWorld();
	bool drawWorms();

#ifdef DEBUG
	bool drawPlayableBox(void);
#endif
};

#endif // !_Gui_h_


