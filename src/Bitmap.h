#pragma once

#include "allegro.h"

class Bitmap
{
public:
	unsigned int height;
	unsigned int width;
	ALLEGRO_BITMAP* bitmap;

	Bitmap(const char* directory = NULL);

};

