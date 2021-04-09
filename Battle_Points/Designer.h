#ifndef DESIGNER_H
#define DESIGNER_H
///////////////////
#include "SDL.H"
#include "SDL_image.h"

///////////////////
#include "Position.h"
#include "TextureManager.h"
#include <iostream> //��������
#include "Scope.h"
//������� �����, ��� ��������� ������� �����

class Designer
{
public:
	Designer();
	~Designer() 
	{
	}
	//���������� � ���� ��� ��������� � ���������
	void setTextureAndRender(const char* textureSheet, SDL_Renderer* ren);
	void Renderer();
	void setTexture(const char* textureSheet);
	
	//��� ��������� �������
	void setSizeObj(int h, int w);
	void setPos(int x, int y);
	void setX(int x) { pos.x = x; dstRect.x = pos.x; setScope();}//������ �������� � ������������� ������� ���������
	void setY(int y) { pos.y = y; dstRect.y = pos.y; setScope();}
	const int getX() { return pos.x; }
	const int getY() { return pos.y; }
	const int getWidth() { return srcRect.w; }
	const int getHeight() { return srcRect.h; }
	
	//��� ����������� �������
	//*������������ �������� �� ������� ����� ����
	void setPosMoveTo(int X, int Y) { moveTo.x = X; moveTo.y = Y; } 
	void MoveTo();
	bool pushChek(int X, int Y , bool mouse);
	void pushDrop() { objPush = false;}
	const Position getMoveTo() { return moveTo; }
	const Position getPos() { return pos; }
	Position getCen();

	//*������� ��������� �������
	Scope getScope() { return scope; }
	void setScope(); //����� ������� ��������� ������� 

	//*���� �� ������ ������, �� true
	bool objPush; //����� �� �������
	bool Update; //����������, ��������� ��������� � obj ��� ���
private:
	SDL_Texture * objTexture;
	SDL_Renderer* renderer;
	SDL_Rect srcRect, dstRect;
	

	//*����� �� ���������� ������ ���� ������ �������� ����� ������ � ������� setX() ��� setY()
	//*��� �� � pos �������� � dstRect, ����� �� ����� �������������� 
	Position pos;
	Scope scope; //������� ��������� �������. 
	Position centr;
	//������ ������� � �������� ����� ���������
	Position moveTo;
	int stepSpeed; //�������� ������������

	
};

#endif //DESIGNER_H