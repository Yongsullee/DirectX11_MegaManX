#include "stdafx.h"
#include "Level3Bullet.h"

Level3Bullet::Level3Bullet(wstring shaderFile, D3DXVECTOR2 start, float angle)
	:Bullet()
{
	SetAttackLevel(100);

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Weapons/Bullets.png";

	Clip* clip;
	//Level 3 Start, clipNumber = 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 62, 46, 62 + 14, 46 + 20), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 90, 49, 90 + 23, 49 + 16), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 120, 41, 120 + 32, 41 + 32), 0.2f);
		animation->AddClip(clip);
	}
	//Level 3 Idle, clipNumber = 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 156, 41, 156 + 40, 41 + 32), 0.3f);
		animation->AddClip(clip);
	}
	//Level 3 bAttack, clipNumber = 2
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 202, 44, 202 + 27, 44 + 24), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 241, 44, 241 + 28, 44 + 28), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 273, 45, 273 + 26, 45 + 26), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 306, 44, 306 + 30, 44 + 30), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 340, 44, 340 + 32, 44 + 32), 0.01f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 377, 44, 377 + 32, 44 + 32), 0.15f);
		animation->AddClip(clip);
	}

	animation->Scale(2.5f, 2.5f);
	animation->RotationDegree(0, angle, 0);
	animation->Position(start);
	animation->DrawBound(false);
	animation->Play(clipNumber);
}

Level3Bullet::~Level3Bullet()
{
	SAFE_DELETE(animation);
}

void Level3Bullet::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	D3DXVECTOR2 position = animation->Position();

	if(StartMove() == false)
		position = AdjustPosition();

	if (StartMove() == true && clipNumber != 3)
		position = Move();	

	HandleStateNumber();

	animation->RotationDegree(0, angle, 0);
	animation->Position(position);
	animation->Play(clipNumber);
	animation->Update(V, P);
}

void Level3Bullet::Render()
{
	__super::Render();

	animation->Render();
}

/***************************************/ // Handle

void Level3Bullet::HandleStateNumber()
{
	// 0 to 1
	DelayStartState();
	CheckStateNumberChange0to1Condition();

	// 2 to Dead
	DelayDeadState();
}

void Level3Bullet::HandleDestruction()
{
	__super::HandleDestruction();	//SetBAttack(true)

	if (EnemyHealthLevel() < AttackLevel())
	{
		clipNumber = 1;
		SetBAttack(false);
	}
	else
		clipNumber = 2;
}

/***************************************/ // State

void Level3Bullet::DelayStartState()
{
	if (clipNumber == 0
		&& animation->GetClip()->CurrentFrame() == 2)
	{
		frameCycle++;
	}
}

void Level3Bullet::CheckStateNumberChange0to1Condition()
{
	if (clipNumber == 0
		&& frameCycle >= 1)
	{
		clipNumber = 1;
		frameCycle = 0;
	}
}

void Level3Bullet::DelayDeadState()
{
	if (clipNumber == 2 && animation->GetClip()->CurrentFrame() == 5)
		SetBDead(true);
}

/***************************************/ // Adjust Position

D3DXVECTOR2 Level3Bullet::AdjustPosition()
{
	D3DXVECTOR2 playerPos = GetPlayerPos();

	playerPos.x += offsetPositionX;
	playerPos.y += offsetPositionY;

	return playerPos;
}

/***************************************/ // Velocity

void Level3Bullet::SetVelocity(int i)
{
	float radian = Math::ToRadian(i);

	velocity.x = cosf(radian);
	velocity.y = sinf(radian);
	velocity *= speed;
}
