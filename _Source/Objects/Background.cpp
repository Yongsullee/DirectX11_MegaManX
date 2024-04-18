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
	*	목적:
	*		뒷 배경 생성
	*/
	bg1 = new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 9, 490, 9 + 3545, 490 + 352);

	bg1->Scale(BG1_SCALE, BG1_SCALE);
	bg1->Position(BG1_POS_X, BG1_POS_Y);

	/*
	*	목적: 
	*		배경 Bridge 생성
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
*		목적:
*			Player 움직임을 감지하여, bg1의 움직임을 결정합니다.
*		설명:
*			Player와 뒷 배경간의 원근감을 묘사하기 위해
*			Player의 속도와 방향을 감지하고,
*			이를 통해 bg1의 Position().x를 Player moveSpeed의 0.5 만큼을 같은 방향으로 이동합니다.
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