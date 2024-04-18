#include "stdafx.h"
#include "Truck.h"

constexpr float TRUCK_POS_Y = 45.f;

Truck::Truck(float xPos, int truckNum, bool bCollidable, bool bMovable, bool bDeadable)
	:Car()
	, clipNum(truckNum)
{
	SetBCollide(bCollidable);	// BackgroundObject class �Ӽ�
	SetBMove(bMovable);			// Car class �Ӽ�
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
	*	����:
	*		Player ��ġ�� �����Ͽ� �������� �̵��մϴ�.
	*	����:
	*		BackgroundObject::GetPlayerPos() �� ���� �޾ƿ� Player ��ġ ������
	*		BackgroundObjecT::playerPosition�� �����ϰ�,
	*		�Ļ� Ŭ�������� playerPosition ������ ���� ���� �帧�� ������ �� �ֽ��ϴ�.
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
