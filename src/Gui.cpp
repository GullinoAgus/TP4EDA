#include <iostream>
#include <string>
#include "Gui.h"
#include "resource.h"

using namespace std;

Gui::Gui()
{
	Worm* p2worm = world.getWormArr();
	string dirWalkingTexts = ".//Resources//wwalking//wwalk-F";
	string dirJumpingTexts = ".//Resources//wjump//wjump-F";

	//TODO: INICIALIZAR ALLEGRO ACA PPOR FA 
	this->closeWindow = true;

	for (int i = 0; i < N_FRAMES; i++)
	{
		if (i <= WF15 )
		{
			this->wormTextArr[i] = Bitmap(&(dirWalkingTexts + to_string(i) + ".png")[0]);
		}
		else
		{
			this->wormTextArr[i] = Bitmap(&(dirJumpingTexts + to_string(i) + ".png")[0]);
		}
	}
	this->worldText = Bitmap(".//Resources//Scenario.png");
	
	

}