#include "EventHandler.h"
//
#include <iostream>
//
EventHandler * EventHandler::evH;

void EventHandler::start(Logic *l)
{
	evH = new EventHandler;
	
	//Пока не конец игры

	while (!l->exitGame.objPush) {

		SDL_WaitEvent(&evH->ev);
		if (evH->ev.type == SDL_QUIT) {
			l->endGame = true;
			l->exitGame.objPush = true;
		}else if (evH->ev.type == SDL_MOUSEBUTTONDOWN) {
			if (evH->ev.button.button == SDL_BUTTON_LEFT) {
				if (evH->ev.button.clicks == 1) {
					l->NewGame(evH->ev.button.x, evH->ev.button.y);
				}
			}
		}else if (l->startGame.objPush) {
			while (!(l->endGame)) {
				evH->onUpdate(l);
			}
			
			std::cout << "EV\n";
			return;
		}
	}
}

void EventHandler::onUpdate(Logic *l)
{
	if (l->endGame) {
		return;
	}
	//Здесь прописываем, что происходит при тех или иных условиях нажатия кнопки

	SDL_WaitEvent(&ev);

	if (ev.type == SDL_QUIT) {
		l->endGame = true;
		l->exitGame.objPush = true;
	}
	else if (ev.type == SDL_MOUSEBUTTONDOWN) {
		if (ev.button.button == SDL_BUTTON_LEFT) {
			if (ev.button.clicks == 1) {
				l->UnitPushCheck(ev.button.x, ev.button.y);

			}
			else if (ev.button.clicks == 2) {
				l->setUnitMoveTo(ev.button.x, ev.button.y);
			}
		}
		if (ev.button.button == SDL_BUTTON_RIGHT) {
			l->UnitDropPush();
		}
		if (ev.button.button == SDL_BUTTON_MIDDLE) {
			l->AllUnitPush();
		}

	}
	//if (ev.type == SDL_MOUSEMOTION)
	//	std::cout<<"(" << ev.button.x << " " << ev.button.y<<")" << std::endl; //Если нужно будет посмотреть позицию
}

