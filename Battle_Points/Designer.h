#ifndef DESIGNER_H
#define DESIGNER_H
///////////////////
#include "SDL.H"
#include "SDL_image.h"

///////////////////
#include "Position.h"
#include "TextureManager.h"
#include <iostream> //Временно
#include "Scope.h"
//Базовый класс, для отрисовки объекта сцены

class Designer
{
public:
	Designer();
	~Designer() 
	{
	}
	//Присвоение в поле для отрисовки и отрисовка
	void setTextureAndRender(const char* textureSheet, SDL_Renderer* ren);
	void Renderer();
	void setTexture(const char* textureSheet);
	
	//Для изменения объекта
	void setSizeObj(int h, int w);
	void setPos(int x, int y);
	void setX(int x) { pos.x = x; dstRect.x = pos.x; setScope();}//Задают значение и пересчитывают область видимости
	void setY(int y) { pos.y = y; dstRect.y = pos.y; setScope();}
	const int getX() { return pos.x; }
	const int getY() { return pos.y; }
	const int getWidth() { return srcRect.w; }
	const int getHeight() { return srcRect.h; }
	
	//Для перемещения объекта
	//*Присваивание значений до которых нужно идти
	void setPosMoveTo(int X, int Y) { moveTo.x = X; moveTo.y = Y; } 
	void MoveTo();
	bool pushChek(int X, int Y , bool mouse);
	void pushDrop() { objPush = false;}
	const Position getMoveTo() { return moveTo; }
	const Position getPos() { return pos; }
	Position getCen();

	//*Область видимости объекта
	Scope getScope() { return scope; }
	void setScope(); //Задаёт область видимости объекта 

	//*Если на объект нажали, то true
	bool objPush; //нажат ли элемент
	bool Update; //показатель, произошли изменения в obj или нет
private:
	SDL_Texture * objTexture;
	SDL_Renderer* renderer;
	SDL_Rect srcRect, dstRect;
	

	//*Чтобы не нарушалась логика игры менять значения нужно только с помощью setX() или setY()
	//*Или же с pos задавать и dstRect, иначе не будет отрисовываться 
	Position pos;
	Scope scope; //Область видимости объекта. 
	Position centr;
	//Память объекта к которому нужно двигаться
	Position moveTo;
	int stepSpeed; //Скорость передвижения

	
};

#endif //DESIGNER_H