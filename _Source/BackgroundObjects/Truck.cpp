#include "stdafx.h"
#include "Truck.h"

constexpr float TRUCK_POS_Y = 45.f;

Truck::Truck(float xPos, int truckNum, bool bCollidable, bool bMovable, bool bDeadable)
	:Car()
	, clipNum(truckNum)
{
	SetBCollide(bCollidable);	// BackgroundObject class 속성
	SetBMove(bMovable);			// Car class 속성
	SetBDead(bDeadable);

	SetMoveSpeed(Math::Random(300.f, 400.f));

	wstring textureFile = L"/MegaManX/Objects/Objects2.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	animation = new Animation;

	Clip* clip;
	if (clipNum == 1)
	{
		//Idle
		{
			clip = new Clip(PlayMode::Loop);
			clip->AddFrame(new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 134, 26, 134 + 96, 26 + 40), 0.3f);
			animation->AddClip(clip);
		}
	}

	if (clipNum == 2)
	{
		//Idle
		{
			clip = new Clip(PlayMode::Loop);
			clip->AddFrame(new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 15, 23, 15 + 101, 23 + 43), 0.3f);
			animation->AddClip(clip);
		}
	}

	animation->Scale(3.0f, 3.0f);
	animation->Position(xPos,  TRUCK_POS_Y);
	animation->Play(0);

	if (BCollide() == true)
	{
		animation->DrawBound(true);
	}
	else 
	{
		animation->DrawBound(false);
	}
}

Truck::~Truck()
{}

void Truck::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);
	/*
	*	목적:
	*		Player 위치를 감지하여 왼쪽으로 이동합니다.
	*	설명:
	*		BackgroundObject::GetPlayerPos() 를 통해 받아온 Player 위치 정보를
	*		BackgroundObjecT::playerPosition에 저장하고,
	*		파생 클래스에서 playerPosition 정보를 통해 게임 흐름을 설정할 수 있습니다.
	*/
	if (BMove() == true)
	{
		if(playerPosition.x < 0.f && playerPosition.x >= -10600.f)
			MoveLeft();
	}
}

void Truck::Render()
{
	__super::Render();
}

void Truck::MoveLeft()
{
	D3DXVECTOR2 position = animation->Position();
	float speed = MoveSpeed();

	position.x -= speed * Timer->Elapsed();

	animation->Position(position);
}
