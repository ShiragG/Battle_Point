#include <thread>
#include <iostream>
#include "SDL.h"
/*==================*/
#include "render.h"
#include "logic.h"
#include "EventHandler.h"


int main(int argc, char* argv[])
{
	int width = 800;
	int height = 600;

	Logic::init(width, height);
	Render::init(width, height);

	//Запускаем в разные потоки 
	std::thread logic_thread(Logic::start);
	std::thread render_thread(Render::start, Logic::getInstanse());
	//Запускаем обработчик событий
	EventHandler::start(Logic::getInstanse());

	//Если endGame == true то прерываются все процессы и игра закрывается
	logic_thread.join();
	render_thread.join();

	return 0;
}