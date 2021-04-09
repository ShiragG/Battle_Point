#ifndef FORT_H
#define FORT_H

#include "Designer.h"

class Fort :public Designer
{
public:
	Fort(int HP, bool Side) : Designer()
	{
		hp = HP;
		side = Side;
		life = false;
	}
	~Fort() {}
	void setHP(int HP) { hp = HP; }
	int getHP() { return hp; }
	void setSide(bool Side) { side = Side; }
	bool getSide() { return side; }
	void setLife(bool Life) { life = Life; }
	bool getLife() { return life; }

	//Виртуальные методы для реализации других классов
	virtual void setDamage(int Damage) = 0;
	virtual int getDamage() = 0;
	//virtual void init() = 0;

private:
	int hp;  //Здоровье базы
	bool side;  //на какой "стороне" находится база (хорошая(1), плохая(2) , ничейная(0))
	bool life;
};
#endif // !FORT_H

