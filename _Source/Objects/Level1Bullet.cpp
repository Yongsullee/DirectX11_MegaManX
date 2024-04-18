#include "stdafx.h"
#include "Level1Bullet.h"

Level1Bullet::Level1Bullet(wstring shaderFile, D3DXVECTOR2 start, float angle)
	:Bullet()
{
	SetAttackLevel(20);

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Weapons/Bullets.png";

	Clip* clip;
	//Level 1, clipNumber = 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 62, 4, 62 + 8, 4 + 6), 0.3f);
		animation->AddClip(clip);
	}
	//Level 1 bAttack, clipNumber = 1  
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 78, 2, 78 + 12, 2 + 12), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 96, 2, 96 + 13, 2 + 13), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 114, 1, 114 + 15, 1 + 15), 0.01f);
		animation->AddClip(clip);
	}

	animation->Scale(2.5f, 2.5f);
	animation->RotationDegree(0, angle, 0);
	animation->Position(start);
	animation->DrawBound(false);
	animation->Play(clipNumber);
}

Level1Bullet::~Level1Bullet()
{
	SAFE_DELETE(animation);
}

void Level1Bullet::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	D3DXVECTOR2 position = animation->Position();

	position = AdjustPosition();

	if(StartMove() == true)
		position = Move();

	HandleStateNumber();

	animation->RotationDegree(0, angle, 0);
	animation->Position(position);
	animation->Play(clipNumber);
	animation->Update(V, P);
}

void Level1Bullet::Render()
{
	__super::Render();

	animation->Render();
}

/***************************************/ // Handle

void Level1Bullet::HandleStateNumber()
{
	// 1 to Dead
	DelayDeadState();
}

void Level1Bullet::HandleDestruction()
{
	__super::HandleDestruction();	//SetBAttack(true)

	if (EnemyHealthLevel() < AttackLevel())
	{
		clipNumber = 0;
		SetBAttack(false);
	}
	else
	{
		clipNumber = 1;
	}
}

/*
	목적:
		clipNumber 가 bHit 상태가 되었을 때, 모든 Frame을 화면에 충분히 나타낸 뒤,
		화면에서 지워버리기 위한 Delay를 걸어둡니다.
	설명:

		1. clipNubmer == 2, 그리고 CurrentFrame이 5로 모두 Play
		2. SetBDead(true)를 통해 죽음 상태로 전환합니다.
*/

/***************************************/ // State

void Level1Bullet::DelayDeadState()
{
	if (clipNumber == 1 && animation->GetClip()->CurrentFrame() == 2)
	{
		SetBDead(true);
	}
}

/***************************************/ // Adjust Poisition w/ Offset Vals

D3DXVECTOR2 Level1Bullet::AdjustPosition()
{

	D3DXVECTOR2 playerPos = GetPlayerPos();

	playerPos.x += offsetPositionX;
	playerPos.y += offsetPositionY;

	return playerPos;
}

/***************************************/ // Physics

void Level1Bullet::SetVelocity(int i)
{
	float radian = Math::ToRadian(i);

	velocity.x = cosf(radian);
	velocity.y = sinf(radian);
	velocity *= speed;
}
