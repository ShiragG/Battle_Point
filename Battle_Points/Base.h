#ifndef BASE_H
#define BASE_H

#include "Fort.h"

class Base : public Fort
{
public:
	Base() : Fort(100, false) 
	{}
	Base(int HP, bool Side, int DAMAGE) : Fort(HP, Side )
	{
		damage = DAMAGE;
		setLife(true);
	}
	~Base() {}
	void setDamage(int Damage) {
		if (getHP() > 0) //Проверяем, что база жива
			setHP(getHP() - Damage);//Отнимает здоровье у базы
		else
			setLife(false);
	}
	int getDamage() {
		return damage;
	};
	void init(int HP, bool Side, int DAMAGE) {
		setHP(HP);
		setSide(Side);
		damage = DAMAGE;
	}
private:
	int damage;
};
#endif // !BASE_H

