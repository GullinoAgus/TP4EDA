#ifndef _GUI_H_
#define _GUI_H_

#include "World.h"
#include "Bitmap.h"
#include <allegro5/allegro5.h>

class Gui
{
public:
	Gui();
	bool show();

private:

	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* evQueue;
	ALLEGRO_EVENT ev;
	ALLEGRO_TIMER* fpsTimer;
	Bitmap worldText;
	Bitmap wormTextArr[N_FRAMES];
	bool closeWindow;
	World world;

	bool drawWorld();
	bool drawWorms();

};

#endif // !_Gui_h_


