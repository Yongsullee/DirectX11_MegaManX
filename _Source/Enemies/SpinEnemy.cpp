#include "stdafx.h"
#include "SpinEnemy.h"
#include "Effects/CrashEffect.h"

SpinEnemy::SpinEnemy(D3DXVECTOR2 position, float _targetPos)
	:Enemy()
	, lastDirection(0)
	, bOnGround(true)
	, groundPosition(0.f)
	, moveSpeed(250.0f), angle(0.f)
	, targetPos(_targetPos)
{
	SetEnemySpec();

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Enemy/SpinEnemy.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	/*****************************************************************/ // Clip

	Clip* clip;
	//Idle, stateNumber = 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 4, 4, 4 + 37, 4 + 37), 0.05f);
		animation->AddClip(clip);
	}
	// Run, stateNumber = 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 4, 4, 4 + 37, 4 + 37), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 42, 5, 42 + 35, 5 + 35), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 78, 5, 78 + 35, 5 + 35), 0.05f);
		animation->AddClip(clip);
	}

	/*****************************************************************/

	animation->Scale(2.5f, 2.5f);
	animation->Position(position);
	animation->DrawBound(false);
	animation->Play(stateNumber);

	/*****************************************************************/ //Effect

	crashEffect = new CrashEffect(position);

}

SpinEnemy::~SpinEnemy()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(crashEffect);
}

void SpinEnemy::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	D3DXVECTOR2 position = animation->Position();
	__super::Update(V, P);
	__super::HandleDestruction();
	
	SetBMove(false);

	HandleMovement();
	HandleAttack();
	HandleDestruction();

	if (position.y < GroundPosition())
	{
		position.y = GroundPosition();
		SetBOnGround(true);
	}

	if (BMove() == true)
	{
		stateNumber = 1;
	}

	animation->Play(stateNumber);
	animation->Update(V, P);

	/*****************************************************************/ //Effect

	if(BRender() == true)
		crashEffect->GetEnemyPos(animation->Position());
	crashEffect->Update(V, P);

	CheckCrashEffectDead();

}

void SpinEnemy::Render()
{
	__super::Render();

	if(BRender() == true)
		animation->Render();

	/*****************************************************************/ //Effect

	crashEffect->Render();

}

/*****************************************************************/ //ETC

Sprite * SpinEnemy::GetSprite()
{
	return animation->GetSprite();
}

/*****************************************************************/ //Spec

/*
	����:
		�ʼ�������, ��� Enemy�� �Ļ� ��ü���� Spec�� �����ؾ� �մϴ�.
	����:
		1. Health Level(ü��) ����
		2. Collide Level(�浹 �� ���ط�) ����
		3. Attack Level (���ݷ�) ����
*/

void SpinEnemy::SetEnemySpec()
{
	SetHealthLevel(60);
	SetCollideLevel(20);
	SetAttackLevel(0);
}

/*****************************************************************/ //Handle

/*
	����:
		Player�� ���������� �������, �������� �����մϴ�.
*/

void SpinEnemy::HandleMovement()
{
	if (playerPos.x > targetPos
		&& BDead() == false)
		Move();
}

void SpinEnemy::HandleDamage(int damage)
{
	__super::HandleDamage(damage);
}

/*
	����:
		Health Level�� 0 ���ϰ� �Ǹ�, Crash Effect�� �������մϴ�.
	����:
		Enemy�� �������� ���߰�, Crash Effect�� �������� �����մϴ�.
*/

void SpinEnemy::HandleDestruction()
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

/*****************************************************************/ //Movement

void SpinEnemy::Move()
{
	D3DXVECTOR2 position = animation->Position();

	float radian = Math::ToRadian(angle);
	position.x -= cosf(radian) * moveSpeed * Timer->Elapsed();
	position.y -= sinf(radian) * moveSpeed * Timer->Elapsed();

	lastDirection = DirectionInDegree::Left;
	animation->RotationDegree(0, DirectionInDegree::Left, 0);

	SetBMove(true);
	animation->Position(position);
}

/*****************************************************************/ //Dead State

/*
	����:
		���������� Crash Effect���� ��� Play�ǰ� ���� ���·� ��ȯ�մϴ�.
	����:
		
		1.	Crash Effect�� ��� Clip�� Play�ǰ� '����' ���·� ��ȯ�˴ϴ�.
		2.	Enemy�� ��� Crash Effect�� ��� Play�ǰ� ���� ���·� ��ȯ�Ǿ�� �ϱ� ������,
			Crash Effect�� '����' ���¸� üũ�ϰ�, ���������� '����' ���·� ��ȯ�˴ϴ�.
*/

void SpinEnemy::CheckCrashEffectDead()
{
	if(crashEffect->BDead() == true)
	{
		SetBDead(true);
	}
}

