#ifndef FIGHTBUFCELL_H
#define FIGHTBUFCELL_H
#include "Unit.h"

template<typename T>
struct FightBufCell
{
	bool lock = false;
	//������ ��������
	T *un;
	T *evUn;
	int whoAt;
};
#endif // !FIGHTBUFCELL_H

