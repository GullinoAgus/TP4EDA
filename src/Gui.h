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
	Bitmap worldText;
	Bitmap wormTextArr[N_FRAMES];

	struct
	{
		ALLEGRO_TIMER* fps;
	} timer;

	bool keysDown[ALLEGRO_KEY_MAX];
	double keysDownTime[ALLEGRO_KEY_MAX];

	World world;

	bool initAllegro();
	bool initImGui(void);
	void eventDispatcher(ALLEGRO_EVENT& ev);

	bool startMoving(int key);
	bool startJumping(int key);
	bool stopMoving(int key);

	bool isValidWormKey(WormsByName worm, int key);
	bool wormEvent(void);

	bool drawWorld();
	bool drawWorms();

#ifdef DEBUG
	bool drawPlayableBox(void);
#endif
};

#endif // !_Gui_h_


