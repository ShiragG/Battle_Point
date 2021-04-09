
#include "logic.h"
#include <iostream>

//������ ��� ��������� ������ ������, ���� ������ ������������ ������ �������

///
Logic * Logic::logic;

Logic::Logic() 
{
}

void Logic::NewGame(int X, int Y)
{//��������� ������� �� ������
	startGame.pushChek(X, Y,true);
	exitGame.pushChek(X, Y,true);
}
//��� ������ � logic
Logic * Logic::getInstanse()
{
	return logic;
}
void Logic::init(int Width, int Height)
{
	logic = new Logic;

	logic->timeRespUnBas = 0;
	logic->timeRespUnCam = 0;

	//�������������� ��������� ������
	logic->startGame.setPos(276,331);
	logic->startGame.setSizeObj(44,253);

	logic->exitGame.setPos(274,400);
	logic->exitGame.setSizeObj(44,253);
	///


	//�������������� ���������
	logic->width = Width;
	logic->height = Height;


	//�������������� ����� (��������, ������)
	//�������������� ������� ����
	logic->evBase.init(1500, false, 5); //��������, �������, ����
	logic->evBase.setSizeObj(80, 80);
	logic->evBase.setPos(0,0);
	logic->evBase.setLife(true);


//	logic->gBase = new Base;
	logic->gBase.init(1500, true, 5);
	logic->gBase.setSizeObj(80, 80);
	logic->gBase.setPos(Width - 80, Height - 80);
	logic->gBase.setLife(true);
	///
	//�������������� ������
	Camp cap;
	cap.init(1000, NULL);
	for (int i = 0; i < 8; i++) {
		if (i < 4) {
			logic->mCamp.push_back(cap);
			logic->mCamp[i].setSide(true);	//������ ��� ������� ������
			logic->mCamp[i].setSizeObj(40, 80);
		}else {
			logic->mCamp.push_back(cap);
			logic->mCamp[i].setSide(false); // � ��� ������
			logic->mCamp[i].setSizeObj(40, 80);
		}
	}
	logic->mCamp[0].setPos(50, 220);
	logic->mCamp[1].setPos(280,20 );
	logic->mCamp[2].setPos(480, 60);
	logic->mCamp[3].setPos(80, 360);
	logic->mCamp[4].setPos(250,480);
	logic->mCamp[5].setPos(650,180);
	logic->mCamp[6].setPos(430,520 );
	logic->mCamp[7].setPos(620,350 );
	///
	//�������������� �����
	logic->castle.init(500,150, false, 5);
	logic->castle.setSizeObj(130, 130);
	logic->castle.setPos(Width / 2 - logic->castle.getWidth()/2,
		Height / 2 - logic->castle.getHeight()/2);//�������� ����� � �����

	//�������������� ������ (�������, ������)
	Unit un;
	un.init(100, 2);
	for (int i = 0; i < 30; i++) {
		logic->mUnit.push_back(un);
		logic->mEvUnit.push_back(un);
		//����������� �������� false
		logic->mUnit[i].setLife(false);
		logic->mEvUnit[i].setLife(false);
	}

	//��������� ������ obj
	
	logic->Obj.push_back(&(logic->gBase));
	logic->Obj.push_back(&(logic->evBase));
	logic->Obj.push_back(&(logic->castle));
	
	int _size = logic->mCamp.size();
	for (int i = 0; i < _size; i++) {
		logic->Obj.push_back(&(logic->mCamp[i]));
	}
	///
}
void Logic::start() {
	
		while (!logic->exitGame.objPush) { //���� ������ exit �� ������
			if (logic->startGame.objPush) { //���������, ������ �� ������ start
				
				while (!(logic->endGame)) {//��������� ���� ��������� ������
					logic->onUpdate();
				}
				return;
			}
		}
}
void Logic::onUpdate()
{
	
	SDL_Delay(30); //����������� �������� ���������� ������ ����

	//���� ���� ������� ��� ������ ����������, �� ���� ��������
	if (!gBase.getLife() || !evBase.getLife()) 
		endGame = true;
	
	EnemyHeadquarters(); //������� ��� ������
	UnitCheckEnemy(); //�������� ������� � ������� �����
	AllUnitFight(); //������������� ��������
	UnitCheckFort(false); //�������� ��� ������� ������ (��������� �� �� � �����)
	UnitCheckFort(true); //�������� ��� ������
	AllUnitMoveTo(); //������������� � �������� ����� 
	GetOutOfTheBorder();//��������, ����� �� ���� �� ������� �������

	//������� ������ �� �������
	timeNow = SDL_GetTicks();
	if (timeNow > timeRespUnBas + 5000) { 
		//C������� ����� � ����� � �� ����
		CreateUnitOnCB();
		timeRespUnBas = timeNow; //��������� ������ ������
	}
	if (timeNow > timeRespUnCam + 8000) {
		//�������� ����� � ������
		CreateUnitOnCamp();
		timeRespUnCam = timeNow;
	}
	///
}
void Logic::InitObjInRender(SDL_Renderer * ren)
{//�������������� ������ � ����� ��� ������ � render
	//������� �����
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {

		mUnit[i].setTextureAndRender("img/Unit.png", ren);
	}
	//������ �����
	_size = mEvUnit.size();
	for (int i = 0; i < _size; i++) {

		mEvUnit[i].setTextureAndRender("img/UnitEvil.png", ren);
	}

	//����. �����
	gBase.setTextureAndRender("img/Fort.png", ren);
	evBase.setTextureAndRender("img/FortEvil.png", ren);
	//����. ������
	_size = mCamp.size();
	for (int i = 0; i < _size; i++) {
			mCamp[i].setTextureAndRender("img/CampDead.png", ren);
	}
	//����. �����
	castle.setTextureAndRender("img/CastleDead.png", ren);
	
}
///
// ��� ������ � ��������� ���� ���������
void Logic::CreateUnitOnCB()
{
	//�������� ����� �� ���� �������
	for (int i = 0; i < 5; i++) {
		if (!mUnit[i].getLife()) {
			mUnit[i].setLife(true);
			mUnit[i].setPos(gBase.getX() + 20, gBase.getY() + 20);
			mUnit[i].setHp(100);
		
			break;
		}
	}
	///
	//�������� ����� �� ���� ������
	for (int i = 0; i < 5; i++) {
		if (!mEvUnit[i].getLife()) {
			mEvUnit[i].setLife(true);
			mEvUnit[i].setPos(evBase.getX() + 20, evBase.getY() + 20);
			mEvUnit[i].setHp(100);
	
			break;
		}
	}
	///

	//�������� ������ � �����
	if(castle.getLife()) //���� ����� ��� ��� �� ��������, �� ��������� ���
	if (castle.getSide()) {
		for (int i = 5; i < 14; i++) {
			if (!mUnit[i].getLife()) {
				mUnit[i].setLife(true);
				mUnit[i].setPos(castle.getX() + castle.getWidth()/2, castle.getY() + castle.getHeight()/2);
				mUnit[i].setHp(100);
				break;
			}
		}
	}
	else {
		for (int i = 5; i < 14; i++) {
			if (!mEvUnit[i].getLife()) {
				mEvUnit[i].setLife(true);
				mEvUnit[i].setPos(castle.getX() + castle.getWidth()/2, castle.getY() + castle.getHeight()/2);
				mEvUnit[i].setHp(100);
				break;
			}
		}
	}
	///
}
void Logic::CreateUnitOnCamp()
{
	int _size = mCamp.size();
	for (int i = 0; i < _size; i++) {
		if (mCamp[i].getLife()) {  //�������� ��� ��� ���� ������
			if (mCamp[i].getSide()) { //���� ��������
				int s = mUnit.size();
				for (int j = 14; j < s; j++) {
					if (!mUnit[j].getLife()) {
						if (i + 14 + i == j || i + 15 + i == j) { //��������, ����� � ������ ����� ������������ ������ 2 �����
							mUnit[j].setLife(true);
							mUnit[j].setPos(mCamp[i].getX() + mCamp[i].getWidth()/2, mCamp[i].getY() + mCamp[i].getHeight()/2);
							mUnit[j].setHp(100);
							mUnit[j].setScope();
							break;
						}
					}
				}
			}
			else {	//���� �������
				int s = mEvUnit.size();
				for (int j = 14; j < s; j++) {
					if (!mEvUnit[j].getLife()) {
						if (i + 14 + i == j || i + 15 + i == j) { //��������, ����� � ������ ����� ������������ ������ 2 �����
							mEvUnit[j].setLife(true);
							mEvUnit[j].setPos(mCamp[i].getX() + 20, mCamp[i].getY() + 20);
							mEvUnit[j].setHp(100);
							mEvUnit[j].setScope();
							break;
						}
					}
				}
			}
		}
	}
}
bool Logic::CheckMoveTo(int X, int Y)
{
	int _size = mUnit.size();
	for(int i = 0; i < _size; i++){
		if (mUnit[i].getMoveTo().x == X
			&& mUnit[i].getMoveTo().y == Y)
			return true;
	}
	return false;
}
void Logic::UnitMoveOver()
{
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		
	}
}
void Logic::GetOutOfTheBorder()
{
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mUnit[i].getLife()) {
			if (mUnit[i].getX() < 0)
				mUnit[i].setX(0);
			if (mUnit[i].getX() + mUnit[i].getWidth() > width)
				mUnit[i].setX(width - mUnit[i].getWidth());
			if (mUnit[i].getY() < 0)
				mUnit[i].setY(0);
			if (mUnit[i].getY() + mUnit[i].getHeight() > height)
				mUnit[i].setY(height - mUnit[i].getHeight());
		}
	}
	 _size = mEvUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mEvUnit[i].getLife()) {
			if (mEvUnit[i].getX() < 0)
				mEvUnit[i].setX(0);
			if (mEvUnit[i].getX() + mEvUnit[i].getWidth() > width)
				mEvUnit[i].setX(width - mEvUnit[i].getWidth());
			if (mEvUnit[i].getY() < 0)
				mEvUnit[i].setY(0);
			if (mEvUnit[i].getY() + mEvUnit[i].getHeight() > height)
				mEvUnit[i].setY(height - mEvUnit[i].getHeight());
		}
	}
}
void Logic::AllUnitMoveTo()
{
		//������������� � �������� ����������� (������� ����)
		int _size = mUnit.size();
		for (int i = 0; i < _size; i++)
			if (mUnit[i].getLife() && !(mUnit[i].getFight())) {
				if ((mUnit[i].getX() != mUnit[i].getMoveTo().x)//�������� �� ������ ���� ����� ��� �������� �������� ��������� 
					|| (mUnit[i].getY() != mUnit[i].getMoveTo().y)) {
					mUnit[i].MoveTo();
				}
			}
		
		// ������������� � �������� ����������� (������ ����)
		_size = logic->mEvUnit.size();
		for (int i = 0; i < _size; i++) {
			if (mEvUnit[i].getLife() && !(mEvUnit[i].getFight())) {

				if ((mEvUnit[i].getX() != mEvUnit[i].getMoveTo().x) 		//�������� �� ������ ���� ����� ��� �������� �������� ���������
					|| (mEvUnit[i].getY() != mEvUnit[i].getMoveTo().y)) {

					mEvUnit[i].MoveTo();

				}
			}
		}
}
bool Logic::CheckScope(int x, int y, Scope s)
{//�������� ������� �����
	//���� �������� � �������� �������, �� true
	if ((s.x1y1.x <= x && x <= s.x2y1.x)
		&& (s.x1y1.y <= y && y <= s.x1y2.y)) {
		return  true;
	}
	return false;
}
//*����� ��������
void Logic::AllUnitFight()
{//����� ��������

	for (int i = 0; i < 80; i++) {
		if (FBuf[i].lock) {
			switch (FBuf[i].whoAt)
			{
			case 1: //���� 1, �� ���� ������� ������� ����
				if (FBuf[i].un->getLife() && FBuf[i].evUn->getFight()) {
					
					FBuf[i].evUn->setDamage(FBuf[i].un->getAt());

					FBuf[i].whoAt = 2; // � ��������� ���, ���� ������ ����

					if (FBuf[i].evUn->getHp() <= 0) {
						FBuf[i].evUn->setFight(false); //����� ����� ��� �� �� ���������
						FBuf[i].un->setFight(false);//����� ����� ��� �� �� ���������
						FBuf[i].lock = false; //��������� ������ ��� ������

					}
				}
				else {
					FBuf[i].evUn->setFight(false);
					FBuf[i].un->setFight(false);
					FBuf[i].lock = false;
				}
				break;
			case 2: //���� 2, �� ���� ������� ������ ����
				if (FBuf[i].evUn->getLife() && FBuf[i].un->getFight()) {
					FBuf[i].un->setDamage(FBuf[i].evUn->getAt());

					FBuf[i].whoAt = 1; // � ��������� ���, ���� ������ ����

					if (FBuf[i].un->getHp() <= 0) {
						FBuf[i].evUn->setFight(false);//����� ����� ��� �� �� ���������
						FBuf[i].un->setFight(false);//����� ����� ��� �� �� ���������
						FBuf[i].lock = false; //��������� ������ ��� ������
					}
				}
				else {
					FBuf[i].un->setFight(false);
					FBuf[i].evUn->setFight(false);
					FBuf[i].lock = false;

				}
				break;
			}
		}
	}
}
void Logic::ChoiceOfTheEnemy(int i, int j)
{	//
	int x1 = mEvUnit[j].getX();
	int y1 = mEvUnit[j].getY();
	//���� ����� ������ � ���� ��������� ���� �����, �� ���������� �����
	if (mUnit[i].pushChek(x1 + mEvUnit[j].getWidth() / 2, y1 + mEvUnit[j].getHeight() / 2 , false)) {//���������� �������� ���������� ����� 
		for (int num = 0; num < 80; num++) {
			if (!FBuf[num].lock) {  //���� ������ ������ �� �������, �� ���������� ������
				if (mUnit[i].getLife() && mEvUnit[j].getLife()) { //���� ��� ����� ����
					std::srand(SDL_GetTicks()); //*�������� �������� ��� ���� ������

					FBuf[num].un = &mUnit[i]; //������ ����� � �����				mUnit[i].setFight(true); //����������, ��� ���� ���������
					mUnit[i].setFight(true);

					FBuf[num].evUn = &mEvUnit[j];
					mEvUnit[j].setFight(true);

					FBuf[num].lock = true; //�������� ������

					FBuf[num].whoAt = 1 + rand() % 2; //�������� �����������, ��� ������ ����� ���������
				}
				break;
			}
		}

	}
}
///
//��� �������� ������� ���������
bool Logic::UnitCheckPos(Designer un, Designer unEv)
{//�������� ����������� ������� ���������
	Scope sv = unEv.getScope(); //������� ��������� ������� �����		 
	//���� ������ ���� ����� ������� �������� � ������ ������� ���������� true 
	if (
		CheckScope(sv.x1y1.x, sv.x1y1.y, un.getScope()) ||  //����������� 1 ���������� �������
		CheckScope(sv.x2y1.x, sv.x2y1.y, un.getScope()) ||  //2
		CheckScope(sv.x2y2.x, sv.x2y2.y, un.getScope()) ||  //3
		CheckScope(sv.x1y2.x, sv.x1y2.y, un.getScope())     //4
		)
		return true;
	else
		return false;
}
bool Logic::FortCheckPos(Fort *f, Designer un)
{//��������, ��� ���� ��������� � ���� �����
	if (f->pushChek(un.getX(), un.getY() , false) ||
		f->pushChek(un.getX(), un.getY() + un.getHeight() , false) ||
		f->pushChek(un.getX() + un.getWidth(), un.getY() + un.getHeight() , false) ||
		f->pushChek(un.getX(), un.getY() + un.getHeight() , false)
		)
		return true;// ���� ���������
	else
		return false; //���� �� ���������
}
void Logic::UnitCheckEnemy()
{//��������� ��������� �����
	//�������� ������ (���� �) ���� ��������� ���� ������
	//�������� ����� � ����� (�����������), ���� ����� �������� ���� �����
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) { //������ �� ������� ������� ������
		if (mUnit[i].getLife()) {  //��������, ��� ���� �����
			for (int j = 0; j < _size; j++) {//������ �� ������� ������ ������
				if (mEvUnit[j].getLife()) { //���� ��������� ���� �����
					if (UnitCheckPos(mUnit[i], mEvUnit[j])) { //��������, ��� ������� ����� �����
						
						if (!(mUnit[i].getFight())) { // ���� (������) ���� �� ������, �� ��� � �������
							mUnit[i].setPosMoveTo(mEvUnit[j].getX() + 5, mEvUnit[j].getY() + 5);
						}
						if (!(mEvUnit[j].getFight())) { // ���� (����) ���� �� ������, �� ��� � �������
							mEvUnit[j].setPosMoveTo(mUnit[i].getX() - 5, mUnit[i].getY() - 5);
						}
						ChoiceOfTheEnemy(i, j); //���������, ����� �� � ����� � ���������� � ����� ��������
					}
				}
			}
		}
	}
}
void Logic::UnitCheckFort(bool Evil)
{//�������� ������
	std::vector<Unit*> mUn; //���������� ������ � ��������
	int _size = mEvUnit.size();
	if (Evil) {			//��������, � ����� �������� ��������
		//mUn = &mEvUnit;
		for (int i = 0; i < _size; i++) { //�������� ������ �� ������
			mUn.push_back(&mEvUnit[i]);
		}
	}
	else {
		for (int i = 0; i < _size; i++) { //�������� ������ �� ������
			mUn.push_back(&mUnit[i]);
		}
	}
	///

	//��� ������� ����� ����������� �������, �������� �� �� � ���� �����
	 _size = mUn.size();
	for (int i = 0; i < _size; i++) {
		if (mUn[i]->getLife() && !(mUn[i]->getFight())) { //���� ��� � �� ���������
			//�������� ����
			if (Evil) { //���� ���� ������
				if (FortCheckPos(&gBase, *mUn[i])) { //���� �� ��������� ����
					if (gBase.getLife()) {
						gBase.setDamage(mUn[i]->getAt()); //������� ����
						mUn[i]->setDamage(gBase.getDamage()); //�������� ����
					}
				}
			}else if (FortCheckPos(&evBase, *mUn[i])) {
				if (evBase.getLife()) {
					evBase.setDamage(mUn[i]->getAt());
					mUn[i]->setDamage(evBase.getDamage());
					
				}
			}
			///
			//�������� �����
			if (FortCheckPos(&castle, *mUn[i])) {
				//����������� ����
				if (!(castle.getLife())) {
					castle.setSide(Evil); //���� �����������, �� ��������� ������� �����
					castle.setLife(true); //���������� ��������
					castle.Update = true; //����������, ��� ����� ������� ��������
					castle.setDamage(mUn[i]->getAt());
					mUn[i]->setDamage(castle.getDamage());
				}
				else if (Evil == castle.getSide()) {
					castle.setDamage(mUn[i]->getAt());
					mUn[i]->setDamage(castle.getDamage());
				}
			}
			///
			//�������� ������
			int size = mCamp.size();
			for (int j = 0; j < size; j++) {
				if (FortCheckPos(&mCamp[j], *mUn[i])) {
					//����������� ������
					if (!(mCamp[j].getLife())) { //���� ������ ������������� ������ ���
						mCamp[j].setSide(Evil); //���� ������ �����������, �� �� ���������� �� ������� ����� ���������
						mCamp[j].setLife(true);
						mCamp[j].Update = true; //����������, ��� ����� ������� �������� 
						mCamp[j].setDamage(mUn[i]->getAt()); //��� ������ �����
					}else if (Evil == mCamp[j].getSide()) { //��������, ��� ������
						mCamp[j].setDamage(mUn[i]->getAt());//���� ���������, �� �������
					}
				}
			}
			///
		}
	}

}
///
//��� ������ � ��������� ���� ���������
void Logic::setUnitMoveTo(int X, int Y)
{//����� �������� ��� ����� ���� ����� ���������
	int sch = 1;
	int maxUn = 6;

	X -= mUnit[0].getWidth() / 2;
	Y -= mUnit[0].getHeight() / 2;

	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mUnit[i].objPush && mUnit[i].getLife() && !(mUnit[i].getFight())) {
			
			while (CheckMoveTo(X,Y)) { //���� ������� ������� ���������� � �����, �� ������� ����������
				if (sch == maxUn) {
					X = X - 8 * 6;
					Y = Y - 8 * 5;
					Y += mUnit[i].getHeight();
					maxUn += 6;
				}
				else {
					X += 8;					//����� ����� �� �������� ���� �� �����
					Y += 8;

					sch++;
				}
				
			}
			mUnit[i].setPosMoveTo(X, Y);
		}
	}
}
void Logic::UnitPushCheck(int X, int Y)
{
	//���������� � ����������� ������� ���� � ����������� �� ����� ������ ������
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mUnit[i].getLife() && !(mUnit[i].objPush)) { //���� ��� � ��� �� �����
			mUnit[i].pushChek(X, Y , true); //����������� ����� �� � ������� �������
			if (mUnit[i].objPush)//���� �� ���� ������, �� ����� �������� ��������
				mUnit[i].Update = true;
		}
	}
}
void Logic::UnitDropPush()
{
	//���������� � ���� ������ objPush
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mUnit[i].objPush && mUnit[i].getLife()) {
			mUnit[i].pushDrop();
			mUnit[i].Update = true;
		}
	}

	/*mUnit[0].setLife(true); //��������� �������� �����(�������)
	mUnit[0].setPos(650, 450);
	mUnit[0].setHp(2);
	mUnit[1].setLife(true);
	mUnit[1].setPos(650, 450);
	mUnit[1].setHp(2);

	mEvUnit[0].setLife(true);
	mEvUnit[0].setPos(350, 450);
	mEvUnit[0].setHp(2);
	mEvUnit[1].setLife(true);
	mEvUnit[1].setPos(350, 450);
	mEvUnit[1].setHp(2);*/

}
void Logic::AllUnitPush()
{
	int _size = mUnit.size();
	for (int i = 0; i < _size; i++) {
		if (mUnit[i].getLife() && !(mUnit[i].objPush)) { //���� ��� � ��� �� �����
			mUnit[i].objPush = true;
		mUnit[i].Update = true;
		}
	}
}
///
//������� ��������� ��� �����
void Logic::EnemyHeadquarters()
{
	//����� ����� ���� 
	Position centerWin;
	centerWin.x = width / 2;
	centerWin.y = height / 2;
	///
	//����� ��������� ���������
	int NumCamps = 0; //���������� ����������� �������
	int _size = mCamp.size();
	for (int i = 0; i < _size; i++) {
		if (mCamp[i].getLife() && !mCamp[i].getSide()) {
			NumCamps++;
		}
	}


	 _size = mEvUnit.size();
	for (int num = 0; num < _size; num++) {
		if (mEvUnit[num].getLife() && !(mEvUnit[num].getFight())) {//��������, ��� ��� � �� ������
			//���������� � ����� �������� ���������
			
			if (mEvUnit[num].getPos().x <= centerWin.x && mEvUnit[num].getPos().y <= centerWin.y) { 
				//��������� � ����� ������� ����
				ScriptOne(num, NumCamps); //��������� ������� ������
			}
			else if (mEvUnit[num].getPos().x > centerWin.x && mEvUnit[num].getPos().y < centerWin.y) {
				//��������� � ������ ������� ����
				ScriptTwo(num, NumCamps);
			}
			else if (mEvUnit[num].getPos().x < centerWin.x && mEvUnit[num].getPos().y > centerWin.y) {
				//��������� � ����� ������ ����
				ScriptThree(num, NumCamps);
			}else {
				//��������� � ������ ������ ����
				ScriptFour(num, NumCamps);
			}
		}
	}
}
//�������
void Logic::ScriptOne(int numEl, const int NumCamps)
{
	//����������� ������ ���� ��������� 0 � 1 ������ 
	if (!mCamp[0].getLife() || mCamp[0].getSide()) {
		mEvUnit[numEl].setPosMoveTo(mCamp[0].getCen().x, mCamp[0].getCen().y);
	}else if (!mCamp[1].getLife() || mCamp[1].getSide()) {
		mEvUnit[numEl].setPosMoveTo(mCamp[1].getCen().x, mCamp[1].getCen().y);
	}else {
		if (NumCamps >= 4) { //���� ���������� ������� 4 ��� �����, �� ���� ����������� �����
			if (!castle.getLife() || castle.getSide()) { //���� ����� ��������� �� �����������
				mEvUnit[numEl].setPosMoveTo(castle.getCen().x, castle.getCen().y);

			}else { //���� ��� ��������, �� ��� ��������� ��������� ����
				mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
			}
		}
		else { //����� ��� ����������� 2 ������
			if (!mCamp[2].getLife() || mCamp[2].getSide()) {//���� �� ��������
				mEvUnit[numEl].setPosMoveTo(mCamp[2].getCen().x, mCamp[2].getCen().y);
			}
			else {//����� ��� � 3
				if (!mCamp[3].getLife() || mCamp[3].getSide()) {//���� �� ��������
					mEvUnit[numEl].setPosMoveTo(mCamp[3].getCen().x, mCamp[3].getCen().y);
				}
			}
		}
	}
}
void Logic::ScriptTwo(int numEl, const int NumCamps)
{
	if (NumCamps >= 4) {//����������� ����� ��� ����
		if (!castle.getLife() || castle.getSide()) { //���� ����� ��������� �� �����������
			mEvUnit[numEl].setPosMoveTo(castle.getCen().x, castle.getCen().y);
		}
		else { //���� ��� ��������, �� ��� ��������� ��������� ����
			mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
		}
	}
	else if (!mCamp[2].getLife() || mCamp[2].getSide()) {//��� � 2
		//���� �� ��������
		mEvUnit[numEl].setPosMoveTo(mCamp[2].getCen().x, mCamp[2].getCen().y);

	}else if (!mCamp[5].getLife() || mCamp[5].getSide()) {//����� ��� � 5
		//���� �� �������� 
		mEvUnit[numEl].setPosMoveTo(mCamp[5].getCen().x, mCamp[5].getCen().y);
	}
	else if (!mCamp[6].getLife() || mCamp[6].getSide()) {//��� � 6
														 //���� �� �������� 
		mEvUnit[numEl].setPosMoveTo(mCamp[6].getCen().x, mCamp[6].getCen().y);
	}
	else {//��� �� ����
		mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
	}
}
void Logic::ScriptThree(int numEl, const int NumCamps)
{
	if (NumCamps >= 4) {
		if (!castle.getLife() || castle.getSide()) { //���� ����� ��������� �� �����������
			mEvUnit[numEl].setPosMoveTo(castle.getCen().x, castle.getCen().y);
		}
		else { //���� ��� ��������, �� ��� ��������� ��������� ����
			mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
		}
	}else if (!mCamp[3].getLife() || mCamp[3].getSide()) {
		mEvUnit[numEl].setPosMoveTo(mCamp[3].getCen().x, mCamp[3].getCen().y);

	}else if (!mCamp[4].getLife() || mCamp[4].getSide()) {
		mEvUnit[numEl].setPosMoveTo(mCamp[4].getCen().x, mCamp[4].getCen().y);

	}else if (!mCamp[7].getLife() || mCamp[7].getSide()) {
		mEvUnit[numEl].setPosMoveTo(mCamp[7].getCen().x, mCamp[7].getCen().y);
	
	}else {
		mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
	}
}
void Logic::ScriptFour(int numEl, const int NumCamps)
{//������ ������� ����
	mEvUnit[numEl].setPosMoveTo(gBase.getCen().x, gBase.getCen().y);
}
///