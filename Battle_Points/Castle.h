#ifndef CASTLE_H
#define CASTLE_H

#include"Fort.h"

class Castle : public Fort
{
public:
	Castle() : Fort(1000,NULL)
	{
	}
	~Castle() {};
	int getCastleGateHp() { return castleGateHp; }
	int getDamage() { return damage; }
	void setDamage(int Damage) {
		if (castleGateHp > 0)
			castleGateHp -= Damage;
		else if (getHP() > 0)
			setHP(getHP() - Damage);
		else {
			Update = true; //Показатель, что произошли изменения
			setSide(!(getSide()));
			setHP(500);
			castleGateHp = 150;
		}
	}
	void init(int HP,int gateHP, bool Side, int DAMAGE) {
		setHP(HP);
		castleGateHp = gateHP;
		setSide(Side);
		damage = DAMAGE;
	}
private:
	int damage;
	int castleGateHp; //Здоровье ворот замка
};
#endif // !CASTLE_H

