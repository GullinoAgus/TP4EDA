#include "Bitmap.h"

//constructor del bitmap
Bitmap::Bitmap(const char* directory)
{

	this->bitmap = directory == NULL ? NULL : al_load_bitmap(directory);
	if (this->bitmap != NULL)
	{
		this->height = al_get_bitmap_height(this->bitmap);
		this->width = al_get_bitmap_width(this->bitmap);
	}
	else
	{
		this->height = 0;
		this->width = 0;
	}
}