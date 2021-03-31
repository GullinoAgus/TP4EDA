#ifndef _GUI_H_
#define _GUI_H_

#include "World.h"
#include "Bitmap.h"
#include <allegro5/allegro5.h>

#define KEYS_BY_WORM	(3)
#define N_WORMS			(2)
#define WORM_KEYS		(KEYS_BY_WORM * N_WORMS)

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

	// Add some spectators !
	int spectatorsBitmap;
	bool spectatorsBitmapForward;

	struct
	{
		ALLEGRO_TIMER* fps;
		ALLEGRO_TIMER* spectators;
	} timer;

	int validKeyCodes[WORM_KEYS];
	bool keysDown[WORM_KEYS];
	double keysDownTime[WORM_KEYS];

	World world;

	bool initAllegro();
	void eventDispatcher(ALLEGRO_EVENT& ev);

	bool startMoving(int keyIndex);
	bool startJumping(int keyIndex);
	bool stopMoving(int keyIndex);
	bool wormEvent(void);

	int getKeyIndexByCode(int allegro_key_code); /* -1 if not found*/
	int getKeyCodeByIndex(int keyIndex);
	bool isValidWormKey(WormsByName worm, int key);
	bool onlyOneWormKeyPressed(WormsByName worm);
	bool noKeyPressed(int keyIndex);

	bool drawWorld();
	bool drawWorms();
	bool drawSpectators();
	void updateSpectatorsBitmap();

#ifdef DEBUG
	bool drawPlayableBox(void);
#endif
};

#endif // !_Gui_h_


