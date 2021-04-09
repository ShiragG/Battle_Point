#include <iostream>
#include "render.h"

Render * Render::render;
Render::Render() 
{
}

Render * Render::getInstanse() {
    return render;
}
void Render::start(Logic * logic) {
	SDL_Delay(5); //������� ���, ������, ��� ���������������� ������� 
	while (!logic->exitGame.objPush) {
		if (logic->startGame.objPush) {
			logic->InitObjInRender(render->renderer); //�������������� ��� ������� logic (��� �������� � ������)
			render->fonTex->setTexture("img/Fon.png");
			////////////
			while (!(logic->endGame)) {
				render->onUpdate(logic);
			}

			SDL_Delay(3000);
			return;
		}
	}
}
void Render::init(int width, int height) {
	
	render = new Render();

	//������������� �������� 
	render->fonTex = new Designer();
	

	//������������� �������� ����
	SDL_Init(SDL_INIT_EVERYTHING);
	render->window = SDL_CreateWindow("Battle Points", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	render->renderer = SDL_CreateRenderer(render->window, -1, 0);
	//
	//������ ������� ����� � ����� ��� 
	SDL_RenderClear(render->renderer);
	render->fonTex->setTextureAndRender("img/BattlePoints.png", render->renderer);
	render->fonTex->setSizeObj(height, width);
	render->fonTex->setPos(0, 0);
	render->fonTex->Renderer();
	//
	SDL_RenderPresent(render->renderer);
}
void Render::onUpdate(Logic * logic) {
	
	SDL_RenderClear(renderer); //������� render
	CheckTex(logic);//�������� ������� �� ���������
	/*	������� ���������
	1)������� �����
	2)����
	3)�����
	*/

	//������ ���
	fonTex->Renderer();
	
	///
	//������ �����
	if(logic->gBase.getLife())
	logic->Obj[0]->Renderer();
	if(logic->evBase.getLife())
	logic->Obj[1]->Renderer();
	
	//������ ������
	//��������. ������� ������ ������� ��� ���.....
	///
	int _size = logic->mCamp.size();
	for (int i = 3; i < _size + 3; i++) {
		logic->Obj[i]->Renderer();
	}
	//������ �����
	
	logic->Obj[2]->Renderer();
	
	///
	//������ ������
	_size = logic->mUnit.size();
	
	for (int i = 0; i < _size; i++) {
		//���� true, �� ����� (��������������)
		
		if (logic->mUnit[i].getLife()) 
			logic->mUnit[i].Renderer();
	
		if (logic->mEvUnit[i].getLife())
		logic->mEvUnit[i].Renderer();
		
	}

	///
	SDL_RenderPresent(render->renderer);

}
void Render::CheckTex(Logic * l)
{
	//�������� ��������� ������
	//�������� �����
	if (l->castle.Update) //���� � ������� ��������� ���������, �� �����������
		if (l->castle.getSide()) {//��������� �� ����� ������� (�������, ������)
			l->castle.setTexture("img/Castle.png"); //����� ��������
			l->castle.Update = false; //���������, ��� ��������� ���������
		}
		else {
			l->castle.setTexture("img/CastleEv.png");
			l->castle.Update = false;
		}
		///
		//�������� �������
		int _size = l->mCamp.size();
		for (int i = 0; i < _size; i++) {
			if (l->mCamp[i].Update) { //���� ��������� ���������
				if (l->mCamp[i].getSide()) {
					l->mCamp[i].setTexture("img/Camp.png");
					l->mCamp[i].Update = false;
				}
				else {
					l->mCamp[i].setTexture("img/CampEv.png");
					l->mCamp[i].Update = false;
				}
			}
		}
		///
		//�������� ������
		_size = l->mUnit.size();
		for (int i = 0; i < _size; i++) {
			if (l->mUnit[i].Update) {
				if (l->mUnit[i].objPush) { //���� �� ���� ������, �� ������� ��� 
					
					l->mUnit[i].setTexture("img/UnitUp.png");
					l->mUnit[i].Update = false;
				}
				else {	//���� ����� ������� ��� ���� ����, �� ������� ���������
					l->mUnit[i].setTexture("img/Unit.png");
					l->mUnit[i].Update = false;
				}
			}
		}
		///
}
