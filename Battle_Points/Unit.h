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
			
			life = false; //�������
			setPosMoveTo(NULL, NULL); //������� �������, � ������� ����� ���������
			objPush = false; //�������� ���� �������
			fight = false; //�������� ���������
			setPos(-80, -80); //����������� �� �������� ����
			Update = true; //C������ ��������, ���� ���� (������� ���������)
			
		}
	};
	//void setEnFound(bool found) { enFound = found; }


private:
	int hp;
	int at;
	//������� ����� ���������� � Desiner
	//���� ���, �� true
	bool life;
	//���� ��� ���, �� ���� �� ������������� �� �������
	bool fight;
	
	
};


#endif//UNIT_H
