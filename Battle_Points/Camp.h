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
			setHP(getHP() - Damage / 2); //�������� ������� ������, ������� � ����
		else {
			Update = true; //����������, ��� ��������� ���������
			setSide(!(getSide())); //���� Camp ��������, �� ������ ���� ������� �� ���������������
			setHP(1000); //�������������� ��������
		}
	}
	int getDamage() { return false; };
	void init(int HP, bool Side) {
		setHP(HP);
		setSide(Side);
	}
};
#endif // !CAMP_H

