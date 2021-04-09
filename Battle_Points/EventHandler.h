#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include "logic.h"
#include "SDL.h"
/*
	����� ��� ��������� ������� � ������� ����
*/
class EventHandler final
{
	static EventHandler *evH;
	SDL_Event ev;
public:
	static void start(Logic *l);
	void onUpdate(Logic *l);
};
#endif//EVENTHANDLER_H
