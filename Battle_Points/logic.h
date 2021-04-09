
#ifndef LOGIC_H
#define LOGIC_H
#include <vector> 
#include "SDL_render.h" //Для инициализации юнитов в render
#include <thread> //Может и не понадобится
#include <iostream> //для rand()

//////////////////
#include "Unit.h"
#include "Base.h"
#include "Camp.h"
#include "Castle.h"

#include "Scope.h"
#include "FightBufCell.h"

class Logic final 
{
    static Logic* logic;
    Logic();
	
	// Для работы с объектами игры приватные
	void CreateUnitOnCB();//Создание юнита на базе и в замке
	void CreateUnitOnCamp(); //Создание юнита в лагерях
	bool CheckMoveTo(int X, int Y);
	void UnitMoveOver(); //Если юнит наложился на другого, то сдвигается
	void GetOutOfTheBorder();//Проверяет, зашёл ли юнит на границу
	// Метод сражения
	void AllUnitFight();
	void ChoiceOfTheEnemy(int i, int j);
	///
	//Для проверки области видимости
	bool CheckScope(int x, int y , Scope s);
	bool UnitCheckPos(Designer un, Designer unEv);
	///
	//Работа фортов
	bool FortCheckPos(Fort *f, Designer un);
	///
	//Простая логика врагов
	void EnemyHeadquarters();
	//*Сценарии
	void ScriptOne(int numEl, int numCamps);
	void ScriptTwo(int numEl, int numCamps);
	void ScriptThree(int numEl, int numCamps);
	void ScriptFour(int numEl, int numCamps);
	///
public:

	void NewGame(int X, int Y);

	//Для работы с logic
    static Logic * getInstanse();
    static void init(int width, int height);
    static void start();
    void onUpdate();
	void InitObjInRender(SDL_Renderer * ren);
	///
	//Для работы с объектами игры публичные
	void setUnitMoveTo(int X, int Y);
	void UnitPushCheck(int X, int Y); //Вызывается в обработчике событий мыши и проверяется на какой объект нажали
	void UnitDropPush(); //Сбрасывает у всех юнитов objPush
	void UnitCheckEnemy(); //проверка области на наличие врагов
	void AllUnitPush(); //Выделяет всех юнитов
	void UnitCheckFort(bool Evil); //проверка области на наличие фортов
	void AllUnitMoveTo(); // Передвигает объекты к заданным координатам
	///
	// Объекты logic
	std::vector<Unit> mUnit;
	std::vector<Unit> mEvUnit;
	//Базы
	Base gBase;
	Base evBase;
	///
	//Лагеря
	std::vector<Camp> mCamp;
	///
	//Замок
	Castle castle;
	
	///
	
	std::vector<Designer*> Obj;


	//шаблонный буфер
	FightBufCell<Unit> FBuf[80]; //Добавляется сюда, если идёт бой
	///
	//Параметры игры
	int width;
	int height;
	bool endGame = false;
	Designer startGame;
	Designer exitGame;
	///
	//Параметры для работы таймера 
	unsigned int timeNow; //Наше время
	unsigned int timeRespUnBas; //Время респавна юнита в базе и замке
	unsigned int timeRespUnCam; //Время респавна юнита в лагере
};

#endif//LOGIC_H
