#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H 

#include "SDL_image.h"
#include <iostream>

class TextureManager
{
public:
	
	static SDL_Texture* LoadTexture(const char* filename, SDL_Renderer* renderer) {
		
		SDL_Surface* tmpSurfac = IMG_Load(filename);
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmpSurfac);
		
		if (tex == NULL) {
			std::cout << "error Texture";
		}
		SDL_FreeSurface(tmpSurfac);

		return tex;
	}
};

#endif //TEXTUREMANAGER_H
