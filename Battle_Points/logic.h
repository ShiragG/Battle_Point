
#ifndef LOGIC_H
#define LOGIC_H
#include <vector> 
#include "SDL_render.h" //��� ������������� ������ � render
#include <thread> //����� � �� �����������
#include <iostream> //��� rand()

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
	
	// ��� ������ � ��������� ���� ���������
	void CreateUnitOnCB();//�������� ����� �� ���� � � �����
	void CreateUnitOnCamp(); //�������� ����� � �������
	bool CheckMoveTo(int X, int Y);
	void UnitMoveOver(); //���� ���� ��������� �� �������, �� ����������
	void GetOutOfTheBorder();//���������, ����� �� ���� �� �������
	// ����� ��������
	void AllUnitFight();
	void ChoiceOfTheEnemy(int i, int j);
	///
	//��� �������� ������� ���������
	bool CheckScope(int x, int y , Scope s);
	bool UnitCheckPos(Designer un, Designer unEv);
	///
	//������ ������
	bool FortCheckPos(Fort *f, Designer un);
	///
	//������� ������ ������
	void EnemyHeadquarters();
	//*��������
	void ScriptOne(int numEl, int numCamps);
	void ScriptTwo(int numEl, int numCamps);
	void ScriptThree(int numEl, int numCamps);
	void ScriptFour(int numEl, int numCamps);
	///
public:

	void NewGame(int X, int Y);

	//��� ������ � logic
    static Logic * getInstanse();
    static void init(int width, int height);
    static void start();
    void onUpdate();
	void InitObjInRender(SDL_Renderer * ren);
	///
	//��� ������ � ��������� ���� ���������
	void setUnitMoveTo(int X, int Y);
	void UnitPushCheck(int X, int Y); //���������� � ����������� ������� ���� � ����������� �� ����� ������ ������
	void UnitDropPush(); //���������� � ���� ������ objPush
	void UnitCheckEnemy(); //�������� ������� �� ������� ������
	void AllUnitPush(); //�������� ���� ������
	void UnitCheckFort(bool Evil); //�������� ������� �� ������� ������
	void AllUnitMoveTo(); // ����������� ������� � �������� �����������
	///
	// ������� logic
	std::vector<Unit> mUnit;
	std::vector<Unit> mEvUnit;
	//����
	Base gBase;
	Base evBase;
	///
	//������
	std::vector<Camp> mCamp;
	///
	//�����
	Castle castle;
	
	///
	
	std::vector<Designer*> Obj;


	//��������� �����
	FightBufCell<Unit> FBuf[80]; //����������� ����, ���� ��� ���
	///
	//��������� ����
	int width;
	int height;
	bool endGame = false;
	Designer startGame;
	Designer exitGame;
	///
	//��������� ��� ������ ������� 
	unsigned int timeNow; //���� �����
	unsigned int timeRespUnBas; //����� �������� ����� � ���� � �����
	unsigned int timeRespUnCam; //����� �������� ����� � ������
};

#endif//LOGIC_H
