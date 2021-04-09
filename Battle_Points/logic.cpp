
#include "logic.h"
#include <iostream>

//Служит для остановки работы потока, если данные используются другим потоком

///
Logic * Logic::logic;

Logic::Logic() 
{
}

void Logic::NewGame(int X, int Y)
{//Проверяет нажатие на кнопки
	startGame.pushChek(X, Y,true);
	exitGame.pushChek(X, Y,true);
}
//Для работы с logic
Logic * Logic::getInstanse()
{
	return logic;
}
void Logic::init(int Width, int Height)
{
	logic = new Logic;

	logic->timeRespUnBas = 0;
	logic->timeRespUnCam = 0;

	//Инициализируем начальные кнопки
	logic->startGame.setPos(276,331);
	logic->startGame.setSizeObj(44,253);

	logic->exitGame.setPos(274,400);
	logic->exitGame.setSizeObj(44,253);
	///


	//Инициализируем параметры
	logic->width = Width;
	logic->height = Height;


	//Инициализируем Форты (хороошие, плохие)
	//Инициализируем главные базы
	logic->evBase.init(1500, false, 5); //здоровье, сторона, урон
	logic->evBase.setSizeObj(80, 80);
	logic->evBase.setPos(0,0);
	logic->evBase.setLife(true);


//	logic->gBase = new Base;
	logic->gBase.init(1500, true, 5);
	logic->gBase.setSizeObj(80, 80);
	logic->gBase.setPos(Width - 80, Height - 80);
	logic->gBase.setLife(true);
	///
	//Инициализируем Лагеря
	Camp cap;
	cap.init(1000, NULL);
	for (int i = 0; i < 8; i++) {
		if (i < 4) {
			logic->mCamp.push_back(cap);
			logic->mCamp[i].setSide(true);	//делаем три хороших лагеря
			logic->mCamp[i].setSizeObj(40, 80);
		}else {
			logic->mCamp.push_back(cap);
			logic->mCamp[i].setSide(false); // И три плохих
			logic->mCamp[i].setSizeObj(40, 80);
		}
	}
	logic->mCamp[0].setPos(50, 220);
	logic->mCamp[1].setPos(280,20 );
	logic->mCamp[2].setPos(480, 60);
	logic->mCamp[3].setPos(80, 360);
	logic->mCamp[4].setPos(250,480);
	logic->mCamp[5].setPos(650,180);
	logic->mCamp[6].setPos(430,520 );
	logic->mCamp[7].setPos(620,350 );
	///
	//инициализируем Замок
	logic->castle.init(500,150, false, 5);
	logic->castle.setSizeObj(130, 130);
	logic->castle.setPos(Width / 2 - logic->castle.getWidth()/2,
		Height / 2 - logic->castle.getHeight()/2);//Помещаем замок в центр

	//Инициализируем юнитов (хорошие, плохие)
	Unit un;
	un.init(100, 2);
	for (int i = 0; i < 30; i++) {
		logic->mUnit.push_back(un);
		logic->mEvUnit.push_back(un);
		//присваиваем объектам false
		logic->mUnit[i].setLife(false);
		logic->mEvUnit[i].setLife(false);
	}

	//Заполняем вектор obj
	
	logic->Obj.push_back(&(logic->gBase));
	logic->Obj.push_back(&(logic->evBase));
	logic->Obj.push_back(&(logic->castle));
	
	int _size = logic->mCamp.size();
	for (int i = 0; i < _size; i++) {
		logic->Obj.push_back(&(logic->mCamp[i]));
	}
	///
}
void Logic::start() {
	
		while (!logic->exitGame.objPush) { //Пока кнопка exit не нажата
			if (logic->startGame.objPush) { //Проверяем, нажата ли кнопка start
				
				while (!(logic->endGame)) {//Запускаем цикл обработки логики
					logic->onUpdate();
				}
				return;
			}
		}
}
void Logic::onUpdate()
{
	
	SDL_Delay(30); //Ограничение скорости выполнения логики игры

	//Если база хороших или плохих уничтожена, то игра окончена
	if (!gBase.getLife() || !evBase.getLife()) 
		endGame = true;
	
	EnemyHeadquarters(); //Скрипты для врагов
	UnitCheckEnemy(); //Проверка области у каждого юнита
	AllUnitFight(); //Просчитывание сражений
	UnitCheckFort(false); //Проверка для хороших юнитов (находится ли он в форте)
	UnitCheckFort(true); //Проверка для плохих
	AllUnitMoveTo(); //Передвигается к заданной точке 
	GetOutOfTheBorder();//Проверка, зашёл ли юнит за игровою область

	//Респавн юнитов по таймеру
	timeNow = SDL_GetTicks();
	if (timeNow > timeRespUnBas + 5000) { 
		//Cоздания юнита в замке и на базе
		CreateUnitOnCB();
		timeRespUnBas = timeNow; //Запускаем таймер занаво
	}
	if (timeNow > timeRespUnCam + 8000) {
		//Создание юнита в лагере
		CreateUnitOnCamp();
		timeRespUnCam = timeNow;
	}
	///
}
void Logic::InitObjInRender(SDL_Renderer * ren)
{//Инициализируем юнитов и форты для работы в render
	//хорошие юниты
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {

		mUnit[i].setTextureAndRender("img/Unit.png", ren);
	}
	//плохие юниты
	_size = mEvUnit.size();
	for (int i = 0; i < _size; i++) {

		mEvUnit[i].setTextureAndRender("img/UnitEvil.png", ren);
	}

	//Иниц. форты
	gBase.setTextureAndRender("img/Fort.png", ren);
	evBase.setTextureAndRender("img/FortEvil.png", ren);
	//Иниц. лагеря
	_size = mCamp.size();
	for (int i = 0; i < _size; i++) {
			mCamp[i].setTextureAndRender("img/CampDead.png", ren);
	}
	//Иниц. Замок
	castle.setTextureAndRender("img/CastleDead.png", ren);
	
}
///
// Для работы с объектами игры приватные
void Logic::CreateUnitOnCB()
{
	//Создание юнита на базе хороших
	for (int i = 0; i < 5; i++) {
		if (!mUnit[i].getLife()) {
			mUnit[i].setLife(true);
			mUnit[i].setPos(gBase.getX() + 20, gBase.getY() + 20);
			mUnit[i].setHp(100);
		
			break;
		}
	}
	///
	//Создание юнита на базе плохих
	for (int i = 0; i < 5; i++) {
		if (!mEvUnit[i].getLife()) {
			mEvUnit[i].setLife(true);
			mEvUnit[i].setPos(evBase.getX() + 20, evBase.getY() + 20);
			mEvUnit[i].setHp(100);
	
			break;
		}
	}
	///

	//Создание юнитов в замке
	if(castle.getLife()) //Если замок уже кем то захвачен, то проверяет кем
	if (castle.getSide()) {
		for (int i = 5; i < 14; i++) {
			if (!mUnit[i].getLife()) {
				mUnit[i].setLife(true);
				mUnit[i].setPos(castle.getX() + castle.getWidth()/2, castle.getY() + castle.getHeight()/2);
				mUnit[i].setHp(100);
				break;
			}
		}
	}
	else {
		for (int i = 5; i < 14; i++) {
			if (!mEvUnit[i].getLife()) {
				mEvUnit[i].setLife(true);
				mEvUnit[i].setPos(castle.getX() + castle.getWidth()/2, castle.getY() + castle.getHeight()/2);
				mEvUnit[i].setHp(100);
				break;
			}
		}
	}
	///
}
void Logic::CreateUnitOnCamp()
{
	int _size = mCamp.size();
	for (int i = 0; i < _size; i++) {
		if (mCamp[i].getLife()) {  //Захвачен уже кем либо лагерь
			if (mCamp[i].getSide()) { //Если хорошими
				int s = mUnit.size();
				for (int j = 14; j < s; j++) {
					if (!mUnit[j].getLife()) {
						if (i + 14 + i == j || i + 15 + i == j) { //Проверка, чтобы в лагере могло разместиться только 2 юнита
							mUnit[j].setLife(true);
							mUnit[j].setPos(mCamp[i].getX() + mCamp[i].getWidth()/2, mCamp[i].getY() + mCamp[i].getHeight()/2);
							mUnit[j].setHp(100);
							mUnit[j].setScope();
							break;
						}
					}
				}
			}
			else {	//Если плохими
				int s = mEvUnit.size();
				for (int j = 14; j < s; j++) {
					if (!mEvUnit[j].getLife()) {
						if (i + 14 + i == j || i + 15 + i == j) { //Проверка, чтобы в лагере могло разместиться только 2 юнита
							mEvUnit[j].setLife(true);
							mEvUnit[j].setPos(mCamp[i].getX() + 20, mCamp[i].getY() + 20);
							mEvUnit[j].setHp(100);
							mEvUnit[j].setScope();
							break;
						}
					}
				}
			}
		}
	}
}
bool Logic::CheckMoveTo(int X, int Y)
{
	int _size = mUnit.size();
	for(int i = 0; i < _size; i++){
		if (mUnit[i].getMoveTo().x == X
			&& mUnit[i].getMoveTo().y == Y)
			return true;
	}
	return false;
}
void Logic::UnitMoveOver()
{
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		
	}
}
void Logic::GetOutOfTheBorder()
{
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mUnit[i].getLife()) {
			if (mUnit[i].getX() < 0)
				mUnit[i].setX(0);
			if (mUnit[i].getX() + mUnit[i].getWidth() > width)
				mUnit[i].setX(width - mUnit[i].getWidth());
			if (mUnit[i].getY() < 0)
				mUnit[i].setY(0);
			if (mUnit[i].getY() + mUnit[i].getHeight() > height)
				mUnit[i].setY(height - mUnit[i].getHeight());
		}
	}
	 _size = mEvUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mEvUnit[i].getLife()) {
			if (mEvUnit[i].getX() < 0)
				mEvUnit[i].setX(0);
			if (mEvUnit[i].getX() + mEvUnit[i].getWidth() > width)
				mEvUnit[i].setX(width - mEvUnit[i].getWidth());
			if (mEvUnit[i].getY() < 0)
				mEvUnit[i].setY(0);
			if (mEvUnit[i].getY() + mEvUnit[i].getHeight() > height)
				mEvUnit[i].setY(height - mEvUnit[i].getHeight());
		}
	}
}
void Logic::AllUnitMoveTo()
{
		//Передвигается к заданным координатам (хороший юнит)
		int _size = mUnit.size();
		for (int i = 0; i < _size; i++)
			if (mUnit[i].getLife() && !(mUnit[i].getFight())) {
				if ((mUnit[i].getX() != mUnit[i].getMoveTo().x)//Проверка на случай если точка уже достигла заданных координат 
					|| (mUnit[i].getY() != mUnit[i].getMoveTo().y)) {
					mUnit[i].MoveTo();
				}
			}
		
		// Передвигается к заданным координатам (плохой юнит)
		_size = logic->mEvUnit.size();
		for (int i = 0; i < _size; i++) {
			if (mEvUnit[i].getLife() && !(mEvUnit[i].getFight())) {

				if ((mEvUnit[i].getX() != mEvUnit[i].getMoveTo().x) 		//Проверка на случай если точка уже достигла заданных координат
					|| (mEvUnit[i].getY() != mEvUnit[i].getMoveTo().y)) {

					mEvUnit[i].MoveTo();

				}
			}
		}
}
bool Logic::CheckScope(int x, int y, Scope s)
{//Проверка входной точки
	//Если попадает в диапазон области, то true
	if ((s.x1y1.x <= x && x <= s.x2y1.x)
		&& (s.x1y1.y <= y && y <= s.x1y2.y)) {
		return  true;
	}
	return false;
}
//*Метод сражения
void Logic::AllUnitFight()
{//Метод сражения

	for (int i = 0; i < 80; i++) {
		if (FBuf[i].lock) {
			switch (FBuf[i].whoAt)
			{
			case 1: //Если 1, то урон наносит хороший юнит
				if (FBuf[i].un->getLife() && FBuf[i].evUn->getFight()) {
					
					FBuf[i].evUn->setDamage(FBuf[i].un->getAt());

					FBuf[i].whoAt = 2; // В следующий раз, бьёт другой юнит

					if (FBuf[i].evUn->getHp() <= 0) {
						FBuf[i].evUn->setFight(false); //Задаём юниту что он не сражается
						FBuf[i].un->setFight(false);//Задаём юниту что он не сражается
						FBuf[i].lock = false; //открываем ячейку для записи

					}
				}
				else {
					FBuf[i].evUn->setFight(false);
					FBuf[i].un->setFight(false);
					FBuf[i].lock = false;
				}
				break;
			case 2: //Если 2, то урон наносит плохой юнит
				if (FBuf[i].evUn->getLife() && FBuf[i].un->getFight()) {
					FBuf[i].un->setDamage(FBuf[i].evUn->getAt());

					FBuf[i].whoAt = 1; // В следующий раз, бьёт другой юнит

					if (FBuf[i].un->getHp() <= 0) {
						FBuf[i].evUn->setFight(false);//Задаём юниту что он не сражается
						FBuf[i].un->setFight(false);//Задаём юниту что он не сражается
						FBuf[i].lock = false; //открываем ячейку для записи
					}
				}
				else {
					FBuf[i].un->setFight(false);
					FBuf[i].evUn->setFight(false);
					FBuf[i].lock = false;

				}
				break;
			}
		}
	}
}
void Logic::ChoiceOfTheEnemy(int i, int j)
{	//
	int x1 = mEvUnit[j].getX();
	int y1 = mEvUnit[j].getY();
	//Если юниты попали в зону видимости друг друга, то происходит битва
	if (mUnit[i].pushChek(x1 + mEvUnit[j].getWidth() / 2, y1 + mEvUnit[j].getHeight() / 2 , false)) {//Координаты середины вражеского юнита 
		for (int num = 0; num < 80; num++) {
			if (!FBuf[num].lock) {  //Если ячейка памяти не закрыта, то происходит запись
				if (mUnit[i].getLife() && mEvUnit[j].getLife()) { //Если оба юнита живы
					std::srand(SDL_GetTicks()); //*Рандомно выбирает кто бьёт первый

					FBuf[num].un = &mUnit[i]; //Запись юнита в буфер				mUnit[i].setFight(true); //Показатель, что юнит сражается
					mUnit[i].setFight(true);

					FBuf[num].evUn = &mEvUnit[j];
					mEvUnit[j].setFight(true);

					FBuf[num].lock = true; //Закрытие ячейки

					FBuf[num].whoAt = 1 + rand() % 2; //Рандомно присваивает, кто первый будет атаковать
				}
				break;
			}
		}

	}
}
///
//Для проверки области видимости
bool Logic::UnitCheckPos(Designer un, Designer unEv)
{//Проверка пересечения области видимости
	Scope sv = unEv.getScope(); //Область видимости второго юнита		 
	//Если хотябы одна точка области попадает в другую область возвращает true 
	if (
		CheckScope(sv.x1y1.x, sv.x1y1.y, un.getScope()) ||  //Проверяется 1 координата области
		CheckScope(sv.x2y1.x, sv.x2y1.y, un.getScope()) ||  //2
		CheckScope(sv.x2y2.x, sv.x2y2.y, un.getScope()) ||  //3
		CheckScope(sv.x1y2.x, sv.x1y2.y, un.getScope())     //4
		)
		return true;
	else
		return false;
}
bool Logic::FortCheckPos(Fort *f, Designer un)
{//Проверка, что юнит находится в зоне форта
	if (f->pushChek(un.getX(), un.getY() , false) ||
		f->pushChek(un.getX(), un.getY() + un.getHeight() , false) ||
		f->pushChek(un.getX() + un.getWidth(), un.getY() + un.getHeight() , false) ||
		f->pushChek(un.getX(), un.getY() + un.getHeight() , false)
		)
		return true;// Если находится
	else
		return false; //Если не находится
}
void Logic::UnitCheckEnemy()
{//Проверяет окружение юнита
	//Вызывает методы (идти к) если вражеский юнит найден
	//Помещает юниты в буфер (сражающихся), если юниты достигли друг друга
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) { //Проход по массиву хороших юнитов
		if (mUnit[i].getLife()) {  //Проверка, что юнит живой
			for (int j = 0; j < _size; j++) {//Проход по массиву плохих юнитов
				if (mEvUnit[j].getLife()) { //Если вражеский юнит живой
					if (UnitCheckPos(mUnit[i], mEvUnit[j])) { //Проверка, что найдена общая точка
						
						if (!(mUnit[i].getFight())) { // Если (добрый) юнит не дерётся, то идёт к другому
							mUnit[i].setPosMoveTo(mEvUnit[j].getX() + 5, mEvUnit[j].getY() + 5);
						}
						if (!(mEvUnit[j].getFight())) { // Если (злой) юнит не дерётся, то идёт к другому
							mEvUnit[j].setPosMoveTo(mUnit[i].getX() - 5, mUnit[i].getY() - 5);
						}
						ChoiceOfTheEnemy(i, j); //Проверяет, дошёл ли к врагу и записывает в буфер сражения
					}
				}
			}
		}
	}
}
void Logic::UnitCheckFort(bool Evil)
{//Проверка фортов
	std::vector<Unit*> mUn; //обобщённая работа с массивом
	int _size = mEvUnit.size();
	if (Evil) {			//Проверка, с каким массивом работать
		//mUn = &mEvUnit;
		for (int i = 0; i < _size; i++) { //Копирует ссылки на объект
			mUn.push_back(&mEvUnit[i]);
		}
	}
	else {
		for (int i = 0; i < _size; i++) { //Копирует ссылки на объект
			mUn.push_back(&mUnit[i]);
		}
	}
	///

	//Для каждого юнита проверяется условие, попадает ли он в зону форта
	 _size = mUn.size();
	for (int i = 0; i < _size; i++) {
		if (mUn[i]->getLife() && !(mUn[i]->getFight())) { //Если жив и не сражается
			//Проверка Базы
			if (Evil) { //Если юнит плохой
				if (FortCheckPos(&gBase, *mUn[i])) { //Если на вражеской базе
					if (gBase.getLife()) {
						gBase.setDamage(mUn[i]->getAt()); //наносит урон
						mUn[i]->setDamage(gBase.getDamage()); //получает урон
					}
				}
			}else if (FortCheckPos(&evBase, *mUn[i])) {
				if (evBase.getLife()) {
					evBase.setDamage(mUn[i]->getAt());
					mUn[i]->setDamage(evBase.getDamage());
					
				}
			}
			///
			//Проверка замка
			if (FortCheckPos(&castle, *mUn[i])) {
				//Захватывает базу
				if (!(castle.getLife())) {
					castle.setSide(Evil); //Если захватывают, то принимает сторону врага
					castle.setLife(true); //Становится активным
					castle.Update = true; //Показывает, что нужно сменить текстуру
					castle.setDamage(mUn[i]->getAt());
					mUn[i]->setDamage(castle.getDamage());
				}
				else if (Evil == castle.getSide()) {
					castle.setDamage(mUn[i]->getAt());
					mUn[i]->setDamage(castle.getDamage());
				}
			}
			///
			//Проверка лагеря
			int size = mCamp.size();
			for (int j = 0; j < size; j++) {
				if (FortCheckPos(&mCamp[j], *mUn[i])) {
					//Захватывает лагерь
					if (!(mCamp[j].getLife())) { //Если лагерь захватывается первый раз
						mCamp[j].setSide(Evil); //Если лагерь захватывают, то он становится на сторону врага персонажа
						mCamp[j].setLife(true);
						mCamp[j].Update = true; //Показывает, что нужно сменить текстуру 
						mCamp[j].setDamage(mUn[i]->getAt()); //Идёт захват точки
					}else if (Evil == mCamp[j].getSide()) { //Проверка, чей лагерь
						mCamp[j].setDamage(mUn[i]->getAt());//Если вражеский, то атакует
					}
				}
			}
			///
		}
	}

}
///
//Для работы с объектами игры публичные
void Logic::setUnitMoveTo(int X, int Y)
{//Задаёт значение для юнита куда нужно двигаться
	int sch = 1;
	int maxUn = 6;

	X -= mUnit[0].getWidth() / 2;
	Y -= mUnit[0].getHeight() / 2;

	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mUnit[i].objPush && mUnit[i].getLife() && !(mUnit[i].getFight())) {
			
			while (CheckMoveTo(X,Y)) { //Если найдены похожие координаты у точки, то смещаем координаты
				if (sch == maxUn) {
					X = X - 8 * 6;
					Y = Y - 8 * 5;
					Y += mUnit[i].getHeight();
					maxUn += 6;
				}
				else {
					X += 8;					//Чтобы точки не нализали друг на друга
					Y += 8;

					sch++;
				}
				
			}
			mUnit[i].setPosMoveTo(X, Y);
		}
	}
}
void Logic::UnitPushCheck(int X, int Y)
{
	//Вызывается в обработчике событий мыши и проверяется на какой объект нажали
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mUnit[i].getLife() && !(mUnit[i].objPush)) { //Если жив и уже не нажат
			mUnit[i].pushChek(X, Y , true); //Проверяется попал ли в область нажатия
			if (mUnit[i].objPush)//Если на юнит нажали, то нужно обновить текстуру
				mUnit[i].Update = true;
		}
	}
}
void Logic::UnitDropPush()
{
	//Сбрасывает у всех юнитов objPush
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mUnit[i].objPush && mUnit[i].getLife()) {
			mUnit[i].pushDrop();
			mUnit[i].Update = true;
		}
	}

	/*mUnit[0].setLife(true); //Временная проверка боёвки(удалить)
	mUnit[0].setPos(650, 450);
	mUnit[0].setHp(2);
	mUnit[1].setLife(true);
	mUnit[1].setPos(650, 450);
	mUnit[1].setHp(2);

	mEvUnit[0].setLife(true);
	mEvUnit[0].setPos(350, 450);
	mEvUnit[0].setHp(2);
	mEvUnit[1].setLife(true);
	mEvUnit[1].setPos(350, 450);
	mEvUnit[1].setHp(2);*/

}
void Logic::AllUnitPush()
{
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mUnit[i].getLife() && !(mUnit[i].objPush)) { //Если жив и уже не нажат
			mUnit[i].objPush = true;
		mUnit[i].Update = true;
		}
	}
}
///
//Простой Интеллект для врага
void Logic::EnemyHeadquarters()
{
	//Задаём центр окна 
	Position centerWin;
	centerWin.x = width / 2;
	centerWin.y = height / 2;
	///
	//Узнаём состояние окружения
	int NumCamps = 0; //Количество захваченных лагерей
	int _size = mCamp.size();
	for (int i = 0; i < _size; i++) {
		if (mCamp[i].getLife() && !mCamp[i].getSide()) {
			NumCamps++;
		}
	}


	 _size = mEvUnit.size();
	for (int num = 0; num < _size; num++) {
		if (mEvUnit[num].getLife() && !(mEvUnit[num].getFight())) {//Проверка, что жив и не дерётся
			//Определяет в какой четверти находится
			
			if (mEvUnit[num].getPos().x <= centerWin.x && mEvUnit[num].getPos().y <= centerWin.y) { 
				//Находится в левом верхнем углу
				ScriptOne(num, NumCamps); //Запускаем условие первое
			}
			else if (mEvUnit[num].getPos().x > centerWin.x && mEvUnit[num].getPos().y < centerWin.y) {
				//Находится в правом верхнем углу
				ScriptTwo(num, NumCamps);
			}
			else if (mEvUnit[num].getPos().x < centerWin.x && mEvUnit[num].getPos().y > centerWin.y) {
				//Находится в левом нижнем углу
				ScriptThree(num, NumCamps);
			}else {
				//Находится в правом нижнем углу
				ScriptFour(num, NumCamps);
			}
		}
	}
}
//Скрипты
void Logic::ScriptOne(int numEl, const int NumCamps)
{
	//Обязательно должны быть захвачены 0 и 1 лагерь 
	if (!mCamp[0].getLife() || mCamp[0].getSide()) {
		mEvUnit[numEl].setPosMoveTo(mCamp[0].getCen().x, mCamp[0].getCen().y);
	}else if (!mCamp[1].getLife() || mCamp[1].getSide()) {
		mEvUnit[numEl].setPosMoveTo(mCamp[1].getCen().x, mCamp[1].getCen().y);
	}else {
		if (NumCamps >= 4) { //Если количество лагерей 4 или более, то идут захватывать замок
			if (!castle.getLife() || castle.getSide()) { //Если замок вражеский то захватываем
				mEvUnit[numEl].setPosMoveTo(castle.getCen().x, castle.getCen().y);

			}else { //Если уже захвачен, то идём атаковать вражескую базу
				mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
			}
		}
		else { //Иначе идёт захватывать 2 лагерь
			if (!mCamp[2].getLife() || mCamp[2].getSide()) {//Если не захвачен
				mEvUnit[numEl].setPosMoveTo(mCamp[2].getCen().x, mCamp[2].getCen().y);
			}
			else {//Иначе идём к 3
				if (!mCamp[3].getLife() || mCamp[3].getSide()) {//Если не захвачен
					mEvUnit[numEl].setPosMoveTo(mCamp[3].getCen().x, mCamp[3].getCen().y);
				}
			}
		}
	}
}
void Logic::ScriptTwo(int numEl, const int NumCamps)
{
	if (NumCamps >= 4) {//захватываем замок или базу
		if (!castle.getLife() || castle.getSide()) { //Если замок вражеский то захватываем
			mEvUnit[numEl].setPosMoveTo(castle.getCen().x, castle.getCen().y);
		}
		else { //Если уже захвачен, то идём атаковать вражескую базу
			mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
		}
	}
	else if (!mCamp[2].getLife() || mCamp[2].getSide()) {//Идём к 2
		//Если не захвачен
		mEvUnit[numEl].setPosMoveTo(mCamp[2].getCen().x, mCamp[2].getCen().y);

	}else if (!mCamp[5].getLife() || mCamp[5].getSide()) {//Иначе идём к 5
		//Если не захвачен 
		mEvUnit[numEl].setPosMoveTo(mCamp[5].getCen().x, mCamp[5].getCen().y);
	}
	else if (!mCamp[6].getLife() || mCamp[6].getSide()) {//Идём к 6
														 //Если не захвачен 
		mEvUnit[numEl].setPosMoveTo(mCamp[6].getCen().x, mCamp[6].getCen().y);
	}
	else {//Идём на базу
		mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
	}
}
void Logic::ScriptThree(int numEl, const int NumCamps)
{
	if (NumCamps >= 4) {
		if (!castle.getLife() || castle.getSide()) { //Если замок вражеский то захватываем
			mEvUnit[numEl].setPosMoveTo(castle.getCen().x, castle.getCen().y);
		}
		else { //Если уже захвачен, то идём атаковать вражескую базу
			mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
		}
	}else if (!mCamp[3].getLife() || mCamp[3].getSide()) {
		mEvUnit[numEl].setPosMoveTo(mCamp[3].getCen().x, mCamp[3].getCen().y);

	}else if (!mCamp[4].getLife() || mCamp[4].getSide()) {
		mEvUnit[numEl].setPosMoveTo(mCamp[4].getCen().x, mCamp[4].getCen().y);

	}else if (!mCamp[7].getLife() || mCamp[7].getSide()) {
		mEvUnit[numEl].setPosMoveTo(mCamp[7].getCen().x, mCamp[7].getCen().y);
	
	}else {
		mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
	}
}
void Logic::ScriptFour(int numEl, const int NumCamps)
{//Просто атакуем базу
	mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
}
///