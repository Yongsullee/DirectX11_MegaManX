#include "stdafx.h"
#include "BossEnemyBullet.h"

BossEnemyBullet::BossEnemyBullet(D3DXVECTOR2 _pos, float _angle)
	:Bullet()
{
	SetSpeed(100.f);
	SetAttackLevel(20);

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Enemy/BossEnemy.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	Clip* clip;

	// Start, stateNumber = 0
	{		
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile,149, 596, 149+11, 596+11), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 161, 594, 161+15, 594+15), 0.05f);
		animation->AddClip(clip); 
	}
	// Idle, stateNumber = 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 183, 595, 183+13, 595+13), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 203, 597, 203+9, 597+9), 0.15f);
		animation->AddClip(clip);
	}

	animation->Scale(2.5f, 2.5f);
	animation->RotationDegree(0, _angle, 0);
	animation->Position(_pos);
	animation->DrawBound(false);
	animation->Play(clipNumber);
}

BossEnemyBullet::~BossEnemyBullet()
{
	SAFE_DELETE(animation);
}

void BossEnemyBullet::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	HandleStateNumber();
	HandleMovement();

	animation->Play(clipNumber);
	animation->Update(V, P);
}

void BossEnemyBullet::Render()
{
	__super::Render();

	if(BRender() == true)
		animation->Render();
}

/*****************************************/ // Handle

void BossEnemyBullet::HandleStateNumber()
{
	// 0 to 1
	DelayStartState();
	CheckStateNumberChange0to1Condition();
}

void BossEnemyBullet::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();

	// 각도 계산 + 속도 계산
	SetVelocity(angle);

	if (BRender() == false)
	{
		AdjustPosition(position);
	}
	else if (BRender() == true)
	{
		Move(position);
	}

	animation->Position(position);
}

void BossEnemyBullet::HandleDestruction()
{
	__super::HandleDestruction();
}

/*****************************************/ // State

void BossEnemyBullet::DelayStartState()
{
	if (clipNumber == 0
		&& animation->GetClip()->CurrentFrame() == 1)
	{
		frameCycle++;
	}
}

void BossEnemyBullet::CheckStateNumberChange0to1Condition()
{
	if (clipNumber == 0
		&& frameCycle >= 1)
	{
		clipNumber = 1;
	}
}

/*****************************************/ // Adjust Position

void BossEnemyBullet::AdjustPosition(D3DXVECTOR2& _pos)
{
	D3DXVECTOR2 enemyPosition = enemyPos;

	_pos.x = enemyPosition.x + offsetPositionX;
	_pos.y = enemyPosition.y + offsetPositionY;
}

/*****************************************/ // Movement

void BossEnemyBullet::SetVelocity(int i)
{
	float radian = Math::ToRadian(i);

	velocity.x = cosf(radian);
	velocity.y = sinf(radian);
	velocity *= speed;
}

void BossEnemyBullet::Move(D3DXVECTOR2 & _pos)
{
	_pos += velocity * Timer->Elapsed();
}
