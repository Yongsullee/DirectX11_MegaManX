#include "stdafx.h"
#include "Level2Bullet.h"

Level2Bullet::Level2Bullet(wstring shaderFile, D3DXVECTOR2 start, float angle)
	:Bullet()
{
	SetAttackLevel(50);

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Weapons/Bullets.png";

	Clip* clip;
	//Level 2 Start, clipNumber = 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 60, 21, 60 + 16, 21 + 14), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 87, 16, 87 + 24, 16 + 24), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 118, 21, 118 + 27, 21 + 12), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 157, 21, 157 + 32, 23 + 8), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 198, 16, 198 + 40, 16 + 19), 0.05f);
		animation->AddClip(clip);
	}
	//Level 2 Idle, clipNumber = 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 246, 16, 246 + 36, 16 + 24), 0.3f);
		animation->AddClip(clip);
	}
	//Level 2 bAttack, clipNumber = 2
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 289, 20, 289 + 38, 20 + 12), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 340, 19, 340 + 16, 19 + 16), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 361, 15, 361 + 24, 15 + 24), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 388, 16, 388 + 22, 16 + 22), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 413, 15, 413 + 24, 15 + 24), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 442, 15, 442 + 24, 15 + 24), 0.02f);
		animation->AddClip(clip);
	}

	animation->Scale(2.5f, 2.5f);
	animation->RotationDegree(0, angle, 0);
	animation->Position(start);
	animation->DrawBound(false);
	animation->Play(clipNumber);
}

Level2Bullet::~Level2Bullet()
{
	SAFE_DELETE(animation);
}

void Level2Bullet::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	D3DXVECTOR2 position = animation->Position();

	if(StartMove() == false)
		position = AdjustPosition();

	if (StartMove() == true && clipNumber != 2 )
		position = Move();		

	HandleStateNumber();

	animation->RotationDegree(0, angle, 0);
	animation->Position(position);
	animation->Play(clipNumber);
	animation->Update(V, P);
}

void Level2Bullet::Render()
{
	__super::Render();

	animation->Render();
}

/***************************************/ // Handle

void Level2Bullet::HandleStateNumber()
{
	// 0 to 1
	DelayStartState();
	ChangeStateNumber0to1Conditon();

	// 2 to Dead
	DelayDeadState();
}

void Level2Bullet::HandleDestruction()
{
	__super::HandleDestruction();	//SetBAttack(true)

	/*
		목적:
			충돌한 Enemy의 체력이 Bullet의 공격력보다 적다면, 관통합니다.
	*/

	if (EnemyHealthLevel() < AttackLevel())
	{
		clipNumber = 1;
		SetBAttack(false);
	}
	else
		clipNumber = 2;
}

/***************************************/ // State

void Level2Bullet::DelayStartState()
{
	if (clipNumber == 0
		&& animation->GetClip()->CurrentFrame() == 4)
	{
		frameCycle++;
	}
}

void Level2Bullet::ChangeStateNumber0to1Conditon()
{
	if (clipNumber == 0
		&& frameCycle == 1)
	{
		clipNumber = 1;
		frameCycle = 0;
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


void Level2Bullet::DelayDeadState()
{
	if (clipNumber == 2 && animation->GetClip()->CurrentFrame() == 5)
		SetBDead(true);
}

/***************************************/ // Adjust Position w/ Offset Vals

D3DXVECTOR2 Level2Bullet::AdjustPosition()
{
	D3DXVECTOR2 playerPos = GetPlayerPos();

	playerPos.x += offsetPositionX;
	playerPos.y += offsetPositionY;

	return playerPos;
}

/***************************************/ // Physics

void Level2Bullet::SetVelocity(int i)
{
	float radian = Math::ToRadian(i);

	velocity.x = cosf(radian);
	velocity.y = sinf(radian);
	velocity *= speed;
}
