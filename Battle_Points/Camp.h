#ifndef CAMP_H
#define CAMP_H

#include "Fort.h"

class Camp : public Fort
{
public:
	Camp() : Fort(700, NULL)
	{}
	~Camp() {}
	void setDamage(int Damage) {
		if (getHP() > 0) 
			setHP(getHP() - Damage / 2); //Скорость захвата Лагеря, снижена в двое
		else {
			Update = true; //Показатель, что произошли изменения
			setSide(!(getSide())); //Если Camp захвачен, то меняет свою сторону на противоположную
			setHP(1000); //Востанавливает здоровье
		}
	}
	int getDamage() { return false; };
	void init(int HP, bool Side) {
		setHP(HP);
		setSide(Side);
	}
};
#endif // !CAMP_H

