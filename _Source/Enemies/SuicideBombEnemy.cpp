#include "stdafx.h"
#include "SuicideBombEnemy.h"
#include "Effects/Effect.h"
#include "Effects/CrashEffect.h"

SuicideBombEnemy::SuicideBombEnemy(D3DXVECTOR2 start, D3DXVECTOR2 target)
	:Enemy(),
	targetPosition(target)
{
	SetEnemySpec();

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Enemy/BombEnemy.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	Clip* clip;
	// Move = 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 6, 5, 6 + 29, 5 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 36, 5, 36 + 29, 5 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 65, 5, 65 + 31, 5 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 96, 5, 96 + 29, 5 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 126, 5, 126 + 29, 5 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 155, 5, 155 + 31, 5 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 186, 5, 186 + 29, 5 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 216, 5, 216 + 29, 5 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 246, 5, 246 + 29, 5 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 246, 5, 246 + 29, 5 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 276, 5, 276 + 29, 5 + 32), 0.05f);
		animation->AddClip(clip);
	}
	
	animation->Scale(2.5f, 2.5f);
	animation->RotationDegree(0, 0, 0);
	animation->Position(start);
	animation->DrawBound(false);
	animation->Play(stateNumber);

	/*****************************************************************/	// Effect

	crashEffect = new CrashEffect(start);
}

SuicideBombEnemy::~SuicideBombEnemy()
{
	SAFE_DELETE(animation);

	SAFE_DELETE(crashEffect);
}


void SuicideBombEnemy::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);
	__super::HandleDestruction();

	D3DXVECTOR2 position = animation->Position();

	SetBMove(false);

	HandleMovement();
	HandleAttack();
	HandleDestruction();

	animation->Play(stateNumber);
	animation->Update(V, P);

	/*****************************************************************/	// Effect

	if (BRender() == true)
		crashEffect->GetEnemyPos(animation->Position());
	crashEffect->Update(V, P);

	CheckCrashEffectDead();

}

void SuicideBombEnemy::Render()
{
	__super::Render();

	if (BRender() == true)
		animation->Render();

	/*****************************************************************/ //Effect

	crashEffect->Render();
}

/*****************************************************************/ //ETC

Sprite * SuicideBombEnemy::GetSprite()
{
	return animation->GetSprite();
}

/*****************************************************************/ //Spec

void SuicideBombEnemy::SetEnemySpec()
{
	SetHealthLevel(80);
	SetCollideLevel(40);
	SetAttackLevel(32);
}

/*****************************************************************/ //Movement

/*
	목적:
		Player를 따라 움직이는 행동패턴을 구현합니다.
	설명:
		1.	삼각 함수를 통해 Enemy ------ Player의 각도를 구합니다.
		
			a) Enemy의 X좌표 - Player의 X좌표				== AC 직선 길이
			b) Enemy의 (X, Y) 좌표 - Player의 (X, Y) 좌표	== AB 빗변 길이
			c) 각도 A = 아크코사인( AC / AB )가 됩니다.

		2.	일정 거리 이하로 Enemy와 Player간의 거리가 좁혀졌을때,
			위에서 구한 각도를 통해 방향 벡터를 설정합니다.

		3.	만약, Player가 Enemy를 지나쳐 갈 경우, 
			Enemy는 Player의 뒤를 쫒습니다. 
			더불어, 위에서 구한 각도를 통해 방향 벡터를 지속적으로 변경합니다.

		4.	결과적으로, 유도 미사일 처럼 Player를 쫒는 형태를 구현합니다.
			
*/

void SuicideBombEnemy::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();

	AngleBetweenPlayer(position);

	if (playerPos.x >= targetPosition.x)
	{
		float radian = Math::ToRadian(angle);
		position.x -= cosf(radian) * moveSpeed * Timer->Elapsed();
		position.y -= sinf(radian) * moveSpeed * Timer->Elapsed();

		SetBMove(true);
		animation->Position(position);
	}
}

void SuicideBombEnemy::AngleBetweenPlayer(D3DXVECTOR2& _position)
{
	float ab = sqrtf(pow(_position.x - playerPos.x, 2) + pow(_position.y - playerPos.y, 2));
	float ac = fabs(_position.x - playerPos.x);

	tmpAngle = Math::ToDegree(acosf(ac / ab));

	// 1. Enemy 기준 Player가 왼쪽에 위치할 때
	if (playerPos.x < _position.x)
	{
		if (playerPos.y > _position.y)
		{
			tmpAngle = -1.f * tmpAngle;
		}
	}
	// 2. Enemy 기준 Player가 오른쪽에 위치할 때
	else if (playerPos.x > _position.x)
	{
		if (playerPos.y < _position.y)
		{
			tmpAngle = 180.f + (-1.f * tmpAngle);
		}
		else
		{
			tmpAngle += 180.f;
		}
		SlowDownSpeed();
	}

	SetAngle(tmpAngle);

}

void SuicideBombEnemy::SlowDownSpeed()
{
	SetMoveSpeed(200.f);
}

/*****************************************************************/ //Attack

void SuicideBombEnemy::HandleAttack()
{
	__super::HandleAttack();
}

/*****************************************************************/ //Damage

void SuicideBombEnemy::HandleDamage(int damage)
{
	__super::HandleDamage(damage);
}

/*****************************************************************/ //Destruction

void SuicideBombEnemy::HandleDestruction()
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

/*****************************************************************/ //Effect Dead Check

void SuicideBombEnemy::CheckCrashEffectDead()
{
	if (crashEffect->BDead() == true)
	{
		SetBDead(true);
		crashEffect->SetBRender(false);
	}
}
