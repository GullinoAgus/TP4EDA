#ifndef BITMAP_H
#define	BITMAP_H	1

#include <allegro5/allegro.h>
class Bitmap
{
public:
    unsigned int height;
    unsigned int width;
    ALLEGRO_BITMAP* bitmap;

    Bitmap(const char* directory = NULL);

};

#endif // ! BITMAP_H
