#include <iostream>
#include "Gui.h"
#include "res"

Gui::Gui()
{
	for (int i = 0; i < N_FRAMES; i++)
	{
		this->wormTextArr[i] = Bitmap();
	}
}