#ifndef UNIT_H
#define UNIT_H

#include"Designer.h"

class Unit : public Designer
{
public:
	Unit() : Designer()
	{
		hp = 100;
		at = 10;
		life = false;
		fight = false;
	}
	~Unit() 
	{
	}
	
	void init(int HP, int AT) { hp = HP; at = AT; };
	const int getAt() { return at; }
	const int getHp() { return hp; }
	const bool getFight() { return fight; }
	const bool getLife() { return life; }
//	const bool getEnFound() { return enFound; }
	void setHp(int Hp) { hp = Hp; }
	void setLife(bool Life) { life = Life;}
	void setFight(bool Fight) { fight = Fight; }
	void setDamage(int damage){
		
		hp -= damage;
		if (hp <= 0) {
			
			life = false; //умирает
			setPosMoveTo(NULL, NULL); //убирает позицию, к которой нужно двигаться
			objPush = false; //перестаёт быть нажатым
			fight = false; //перестаёт сражаться
			setPos(-80, -80); //Переносится из игрового поля
			Update = true; //Cменяет текстуру, если умер (убирает выделение)
			
		}
	};
	//void setEnFound(bool found) { enFound = found; }


private:
	int hp;
	int at;
	//Позиция юнита определена в Desiner
	//Если жив, то true
	bool life;
	//Если идёт бой, то юнит не переключается на другого
	bool fight;
	
	
};


#endif//UNIT_H
