#include "stdafx.h"
#include "Background.h"
#include "BackgroundObjects/BigBridge.h"
#include "BackgroundObjects/Car.h"
#include "Objects/Player.h"
#include "Line.h"

constexpr float BG1_SCALE = 3.5f;
constexpr float BG1_POS_X = -6069.f;
constexpr float BG1_POS_Y = -250.f;

Background::Background(wstring shaderFile)
{
	wstring textureFile = L"";
	textureFile = L"/MegaManX/Background/Stage1.png";

	/*
	*	����:
	*		�� ��� ����
	*/
	bg1 = new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 9, 490, 9 + 3545, 490 + 352);

	bg1->Scale(BG1_SCALE, BG1_SCALE);
	bg1->Position(BG1_POS_X, BG1_POS_Y);

	/*
	*	����: 
	*		��� Bridge ����
	*/
	bgBridge = new BigBridge();
}

Background::~Background()
{
	SAFE_DELETE(bg1);
	SAFE_DELETE(bgBridge);
}

void Background::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	bg1->Update(V, P);
	bgBridge->Update(V, P);
}

void Background::Render()
{
	bg1->Render();
	bgBridge->Render();
}

/*
*		����:
*			Player �������� �����Ͽ�, bg1�� �������� �����մϴ�.
*		����:
*			Player�� �� ��氣�� ���ٰ��� �����ϱ� ����
*			Player�� �ӵ��� ������ �����ϰ�,
*			�̸� ���� bg1�� Position().x�� Player moveSpeed�� 0.5 ��ŭ�� ���� �������� �̵��մϴ�.
*/
void Background::BackgroundMove(Player& inPlayerInfo)
{
	D3DXVECTOR2 position = bg1->Position();
	D3DXVECTOR2 playerPos = inPlayerInfo.GetSprite()->Position();

	float speed = inPlayerInfo.MoveSpeed();

	if (maxPlayerPos < inPlayerInfo.GetSprite()->Position().x
		&& inPlayerInfo.BMove() == true
		&& inPlayerInfo.LastDirection() == 0)
	{
		maxPlayerPos = inPlayerInfo.GetSprite()->Position().x;

		position.x += speed * 0.55f * Timer->Elapsed();
	}

	bg1->Position(position);
}