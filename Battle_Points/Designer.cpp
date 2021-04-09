#include "Designer.h"
#include <cmath>


Designer::Designer()
{
	srcRect.w = 25;
	srcRect.h = 35;
	srcRect.x = 0;
	srcRect.y = 0;
	
	dstRect.x = pos.x;
	dstRect.y = pos.y;
	dstRect.w = srcRect.w; 
	dstRect.h = srcRect.h; 

	stepSpeed = 1; //�������� ����

	pos.x = NULL; pos.y = NULL;
	moveTo.x = NULL; moveTo.y = NULL;
	Update = false;
	
}
void Designer::setTextureAndRender(const char * textureSheet, SDL_Renderer * ren)
{
	//����� ��������, � ������ � ������� ����� ��������
	renderer = ren;

	objTexture = TextureManager::LoadTexture(textureSheet, ren);
	
}
void Designer::Renderer()
{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &dstRect);

}
void Designer::setTexture(const char * textureSheet)
{	//�������� ������ �� RENDER!!!
	
	objTexture = TextureManager::LoadTexture(textureSheet, renderer);
	
}
void Designer::setSizeObj(int h, int w)
{
	
	srcRect.h = h; 
	srcRect.w = w;

	dstRect.w = srcRect.w; //��������, ����� �������� �� 2
	dstRect.h = srcRect.h; //��������, ����� �������� �� 2
	
	setScope(); //��������������� ������� ��������� �������
}
void Designer::setPos(int X, int Y)
{
	
	pos.x = X; 
	pos.y = Y;

	dstRect.x = pos.x;
	dstRect.y = pos.y;

	setScope(); //��������������� ������� ��������� ��������
}
bool Designer::pushChek(int X, int Y, bool mouse)
{//��������, ������� �����
	if ((pos.x <= X && X <= (pos.x +srcRect.w)) && (pos.y <= Y && Y <= (pos.y + srcRect.h))) {
		if(mouse)
		objPush = true;

		return true;
	}
	return false;
}
Position Designer::getCen()
{
	centr.x = pos.x + srcRect.w / 2;
	centr.y = pos.y + srcRect.h / 2;
	return centr;
}
void Designer::setScope()
{//������� ������� ��������� �������	
	
	scope.x1y1.x = pos.x - srcRect.w / 2;					// �������� ���������� ����� �������
	scope.x1y1.y = pos.y - srcRect.h / 2;					//	_________
															//	|		|
	scope.x2y1.x =  pos.x + srcRect.w + srcRect.w / 2;		//	|  [o]	|
	scope.x2y1.y = pos.y - srcRect.h / 2;					//	|_______|
															
	scope.x2y2.x = pos.x + srcRect.w + srcRect.w / 2;				
	scope.x2y2.y = pos.y + srcRect.h + srcRect.h / 2;											
															
	scope.x1y2.x = pos.x - srcRect.w / 2;
	scope.x1y2.y = pos.y + srcRect.h + srcRect.h / 2;
	
}
void Designer::MoveTo()
{ //������������ � �������� �����������

	if (moveTo.x != NULL && moveTo.y != NULL) {

		
		//�������� � ������������ ����� �� ��������� � �������� �����������
		if (pos.x > moveTo.x)
			pos.x = pos.x - stepSpeed;
		else if (pos.x < moveTo.x)
			pos.x = pos.x + stepSpeed;

		if (pos.y > moveTo.y)
			pos.y = pos.y - stepSpeed;
		else if (pos.y < moveTo.y)
			pos.y = pos.y + stepSpeed;
		///

		dstRect.x = pos.x;
		dstRect.y = pos.y;
			
	}else if ((moveTo.x == pos.x) && (moveTo.y == pos.y)) {
		moveTo.x = NULL;
		moveTo.y = NULL;

	}
	setScope(); //��������������� ������� ��������� �������
}


