#include <iostream>
#include "render.h"

Render * Render::render;
Render::Render() 
{
}

Render * Render::getInstanse() {
    return render;
}
void Render::start(Logic * logic) {
	SDL_Delay(5); //Немного ждёт, прежде, чем инициализировать объекты 
	while (!logic->exitGame.objPush) {
		if (logic->startGame.objPush) {
			logic->InitObjInRender(render->renderer); //Инициализирует все объекты logic (даёт текстуры и рендер)
			render->fonTex->setTexture("img/Fon.png");
			////////////
			while (!(logic->endGame)) {
				render->onUpdate(logic);
			}

			SDL_Delay(3000);
			return;
		}
	}
}
void Render::init(int width, int height) {
	
	render = new Render();

	//Инициализация объектов 
	render->fonTex = new Designer();
	

	//Инициализация игрового окна
	SDL_Init(SDL_INIT_EVERYTHING);
	render->window = SDL_CreateWindow("Battle Points", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	render->renderer = SDL_CreateRenderer(render->window, -1, 0);
	//
	//Создаём фоновый текст и задаём его 
	SDL_RenderClear(render->renderer);
	render->fonTex->setTextureAndRender("img/BattlePoints.png", render->renderer);
	render->fonTex->setSizeObj(height, width);
	render->fonTex->setPos(0, 0);
	render->fonTex->Renderer();
	//
	SDL_RenderPresent(render->renderer);
}
void Render::onUpdate(Logic * logic) {
	
	SDL_RenderClear(renderer); //Очищает render
	CheckTex(logic);//Проверка текстур на изменение
	/*	Порядок отрисовки
	1)Фоновый текст
	2)Базы
	3)Юниты
	*/

	//Рисуем фон
	fonTex->Renderer();
	
	///
	//Рисуем Форты
	if(logic->gBase.getLife())
	logic->Obj[0]->Renderer();
	if(logic->evBase.getLife())
	logic->Obj[1]->Renderer();
	
	//Рисуем лагеря
	//Проверка. Поменял лагерь команду или нет.....
	///
	int _size = logic->mCamp.size();
	for (int i = 3; i < _size + 3; i++) {
		logic->Obj[i]->Renderer();
	}
	//Рисуем замок
	
	logic->Obj[2]->Renderer();
	
	///
	//Рисуем юнитов
	_size = logic->mUnit.size();
	
	for (int i = 0; i < _size; i++) {
		//Если true, то живой (отрисовывается)
		
		if (logic->mUnit[i].getLife()) 
			logic->mUnit[i].Renderer();
	
		if (logic->mEvUnit[i].getLife())
		logic->mEvUnit[i].Renderer();
		
	}

	///
	SDL_RenderPresent(render->renderer);

}
void Render::CheckTex(Logic * l)
{
	//Проверка состояния фортов
	//Проверка замка
	if (l->castle.Update) //Если в объекте произошли изменения, то проверяется
		if (l->castle.getSide()) {//Проверяем на какой стороне (хорошая, плохая)
			l->castle.setTexture("img/Castle.png"); //Задаём текстуры
			l->castle.Update = false; //Указываем, что проверили изменения
		}
		else {
			l->castle.setTexture("img/CastleEv.png");
			l->castle.Update = false;
		}
		///
		//Проверка лагерей
		int _size = l->mCamp.size();
		for (int i = 0; i < _size; i++) {
			if (l->mCamp[i].Update) { //Если произошли изменения
				if (l->mCamp[i].getSide()) {
					l->mCamp[i].setTexture("img/Camp.png");
					l->mCamp[i].Update = false;
				}
				else {
					l->mCamp[i].setTexture("img/CampEv.png");
					l->mCamp[i].Update = false;
				}
			}
		}
		///
		//Проверка Юнитов
		_size = l->mUnit.size();
		for (int i = 0; i < _size; i++) {
			if (l->mUnit[i].Update) {
				if (l->mUnit[i].objPush) { //Если на юнит нажали, то обводит его 
					
					l->mUnit[i].setTexture("img/UnitUp.png");
					l->mUnit[i].Update = false;
				}
				else {	//Если снято нажатие или юнит умер, то снимает выделение
					l->mUnit[i].setTexture("img/Unit.png");
					l->mUnit[i].Update = false;
				}
			}
		}
		///
}
