#include "stdafx.h"
#include "CarEnemy.h"
#include "Effects/CrashEffect.h"

CarEnemy::CarEnemy(D3DXVECTOR2 position, float _targetPos, float _leftLimit, float _rightLimit)
	:Enemy()
	, targetPos(_targetPos)
	, leftLimit(_leftLimit), rightLimit(_rightLimit)
{
	SetEnemySpec();

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Enemy/Enemies.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	/*****************************************************************/ // Clip

	Clip* clip;
	//Move, stateNumber = 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 89, 655, 89 + 78, 655 + 36), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 4, 655, 4 + 68, 655 + 36), 0.15f);
		animation->AddClip(clip);
	}
	//Move, stateNumber = 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 179, 655, 179 + 78, 655 + 36), 0.5f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 274, 655, 274 + 68, 655 + 36), 0.5f);
		animation->AddClip(clip);
	}
	//Turn, stateNumber = 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 360, 655, 360 + 56, 655 + 35), 0.3f);
		animation->AddClip(clip);
	}

	/*****************************************************************/

	animation->Scale(2.5f, 2.5f);
	animation->RotationDegree(0, DirectionInDegree::Left, 0);
	animation->Position(position);
	animation->DrawBound(false);
	animation->Play(stateNumber);

	/*****************************************************************/ //Effect

	crashEffect = new CrashEffect(position);

}

CarEnemy::~CarEnemy()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(crashEffect);
}

void CarEnemy::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	D3DXVECTOR2 position = animation->Position();
	__super::Update(V, P);
	__super::HandleDestruction();

	SetBMove(false);

	HandleStateNumber();
	HandleMovement();
	HandleAttack();
	HandleDestruction();

	animation->Play(stateNumber);
	animation->Update(V, P);

	/*****************************************************************/ //Effect

	if (BRender() == true)
		crashEffect->GetEnemyPos(animation->Position());
	crashEffect->Update(V, P);

	CheckCrashEffectDead();
}

void CarEnemy::Render()
{
	__super::Render();

	if (BRender() == true)
		animation->Render();

	/*****************************************************************/ //Effect

	crashEffect->Render();
}

/*****************************************************************/ //ETC

Sprite * CarEnemy::GetSprite()
{
	return animation->GetSprite();
}

/*****************************************************************/ //Spec

void CarEnemy::SetEnemySpec()
{
	SetHealthLevel(80);
	SetCollideLevel(40);
	SetAttackLevel(0);
}

/*****************************************************************/ //Handle

void CarEnemy::HandleStateNumber()
{
	DelayIdleState();
	CheckStateNumber0to1Condition();
	
	DelayTurnState();
	CheckStateNumber2to0Condition();
}

void CarEnemy::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();


	// 착지 조건 
	position.y += gravity * Timer->Elapsed();

	if (position.y < GroundPosition())
	{
		position.y = GroundPosition();
		SetBOnGround(true);
	}

	if (position.x <= leftLimit)
	{
		// 1.회전 Animation
		TurnRight();
	}
	else if (position.x >= rightLimit)
	{
		// 1.회전 Animation
		TurnLeft();
	}

	if (playerPos.x >= targetPos)
	{
		if (lastDirection == DirectionInDegree::Left)
		{
			position = MoveLeft(position);
		}
		if (lastDirection == DirectionInDegree::Right)
		{
			position = MoveRight(position);
		}

		SetBMove(true);
	}
	
	animation->RotationDegree(0, lastDirection, 0);
	animation->Position(position);

}

void CarEnemy::HandleAttack()
{
	__super::HandleAttack();
}

void CarEnemy::HandleDamage(int damage)
{
	__super::HandleDamage(damage);
}

void CarEnemy::HandleDestruction()
{
	__super::HandleDestruction();

	if (HealthLevel() <= 0)
	{
		SetBRender(false);
		crashEffect->SetBRender(true);

		SetCollideLevel(0);
		SetAttackLevel(0);
	}
}

/*****************************************************************/ //State

/*
	목적:
		State Number를 관리합니다.
*/


void CarEnemy::DelayIdleState()
{
	if (StateNumber() == 0
		&& animation->GetClip()->CurrentFrame() == 1)
	{
		frameCycle++;
	}
}

void CarEnemy::CheckStateNumber0to1Condition()
{
	if (StateNumber() == 0
		&& frameCycle == 25)
	{
		SetStateNumber(1);

		frameCycle = 0;
	}
}

void CarEnemy::DelayTurnState()
{
	if (StateNumber() == 2)
	{
		frameCycle++;
	}
}

void CarEnemy::CheckStateNumber2to0Condition()
{
	if (StateNumber() == 2
		&& frameCycle == 5)
	{
		SetStateNumber(0);
		frameCycle = 0;
	}
}

/*****************************************************************/ //Movement


D3DXVECTOR2 CarEnemy::MoveLeft(D3DXVECTOR2 & _pos)
{
	velocity -= friction * Timer->Elapsed();

	float radian = Math::ToRadian(angle);
	_pos.x -= cosf(radian) /** moveSpeed*/ * velocity * Timer->Elapsed();
	_pos.y -= sinf(radian) /** moveSpeed*/ * velocity * Timer->Elapsed();

	SetBMove(true);

	return _pos;
}

D3DXVECTOR2 CarEnemy::MoveRight(D3DXVECTOR2& _pos)
{
	velocity -= friction * Timer->Elapsed();

	float radian = Math::ToRadian(angle);
	_pos.x += cosf(radian) /** moveSpeed*/ * velocity * Timer->Elapsed();
	_pos.y += sinf(radian) /** moveSpeed*/ * velocity * Timer->Elapsed();

	SetBMove(true);

	return _pos;
}

void CarEnemy::TurnRight()
{
	lastDirection = DirectionInDegree::Right;

	SetStateNumber(2);

	Boost();
}

void CarEnemy::TurnLeft()
{
	lastDirection = DirectionInDegree::Left;
	
	SetStateNumber(2);

	Boost();
}

void CarEnemy::Boost()
{
	velocity = 260.5f;
}

/*****************************************************************/ //Effect

void CarEnemy::CheckCrashEffectDead()
{
	if (crashEffect->BDead() == true)
	{
		SetBDead(true);
	}
}
