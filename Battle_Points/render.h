#ifndef RENDER_H
#define RENDER_H

////////////////////
#include "SDL.h"
//#include"SDL_image.h"
////////////////////
#include "logic.h"
#include "TextureManager.h"
#include "Designer.h"

class Render final 
{
    static Render* render;
    Render();
public:
    static Render* getInstanse();
    static void init(int width, int height);
    static void start(Logic * logic);
    void onUpdate(Logic * logic);

private:
	//
	 SDL_Window* window;
	 SDL_Renderer* renderer;
	 //Объекты окна
	 Designer *fonTex;

	 void CheckTex(Logic * logic);
};
#endif//RENDER_H
