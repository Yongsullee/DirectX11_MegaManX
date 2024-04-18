#include "stdafx.h"
#include "NormalCar.h"

constexpr float NORMAL_CAR_1_POS_Y = 2.f;
constexpr float NORMAL_CAR_2_POS_Y = 15.f;

NormalCar::NormalCar(float xPos, int carNum, bool bCollidable, bool bMovable, bool bDeadable)
	:Car()
	, clipNum(carNum)
{
	SetBCollide(bCollidable);	// BackgroundObject class 속성
	SetBMove(bMovable);			// Car class 속성
	SetBDead(bDeadable);

	SetMoveSpeed(Math::Random(450.f, 550.f));

	wstring textureFile = L"/MegaManX/Objects/Objects2.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	animation = new Animation;

	Clip* clip;
	if (clipNum == 1)
	{
		//Idle
		{
			clip = new Clip(PlayMode::Loop);
			clip->AddFrame(new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 32, 131, 32 + 78, 131 + 32), 0.3f);
			animation->AddClip(clip);
			animation->Scale(3.0f, 3.0f);
			animation->Position(xPos, NORMAL_CAR_1_POS_Y);
		}
	}

	if (clipNum == 2)
	{
		//Idle
		{
			clip = new Clip(PlayMode::Loop);
			clip->AddFrame(new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 253, 37, 253 + 98, 37 + 46), 0.15f);
			clip->AddFrame(new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 253, 99, 253 + 104, 99 + 51), 0.15f);
			clip->AddFrame(new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 253, 164, 253 + 104, 164 + 46), 0.15f);
			animation->AddClip(clip);
			animation->Scale(2.5f, 2.5f);
			animation->Position(xPos, NORMAL_CAR_2_POS_Y);
		}
	}

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

NormalCar::~NormalCar()
{
}

void NormalCar::Update(D3DXMATRIX & V, D3DXMATRIX & P)
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
		if (playerPosition.x < 0.f && playerPosition.x >= -10600.f)
			MoveLeft();
	}
}

void NormalCar::Render()
{
	__super::Render();
}

void NormalCar::MoveLeft()
{
	D3DXVECTOR2 position = animation->Position();
	float speed = MoveSpeed();

	position.x -= speed * Timer->Elapsed();

	animation->Position(position);
}
