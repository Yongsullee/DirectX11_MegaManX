#include "stdafx.h"
#include "NormalCar.h"

constexpr float NORMAL_CAR_1_POS_Y = 2.f;
constexpr float NORMAL_CAR_2_POS_Y = 15.f;

NormalCar::NormalCar(float xPos, int carNum, bool bCollidable, bool bMovable, bool bDeadable)
	:Car()
	, clipNum(carNum)
{
	SetBCollide(bCollidable);	// BackgroundObject class �Ӽ�
	SetBMove(bMovable);			// Car class �Ӽ�
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
	*	����:
	*		Player ��ġ�� �����Ͽ� �������� �̵��մϴ�.
	*	����:
	*		BackgroundObject::GetPlayerPos() �� ���� �޾ƿ� Player ��ġ ������
	*		BackgroundObjecT::playerPosition�� �����ϰ�,
	*		�Ļ� Ŭ�������� playerPosition ������ ���� ���� �帧�� ������ �� �ֽ��ϴ�.
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
