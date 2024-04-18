#include "stdafx.h"
#include "Enemies/LandMineEnemy.h"
#include "Effects/Effect.h"
#include "Effects/CrashEffect.h"
#include "Weapons/Weapon.h"
#include "Weapons/LandMine.h"

LandMineEnemy::LandMineEnemy(D3DXVECTOR2 start, D3DXVECTOR2 target)
	:Enemy()
	, moveSpeed(200.f)
	, targetPosition(target)
{
	SetEnemySpec();

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Enemy/Enemies.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	Clip* clip;
	// StartToMove = 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 209, 955-13+8, 209 + 27, 955 + 35+13-8), 0.12f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 259, 955-13+8, 259 + 27, 955 + 35+13-8), 0.12f);
		animation->AddClip(clip);
	}
	// Idle = 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 0, 905-12+7, 0 + 45, 905 + 36+12-7), 0.10f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 58, 905-13+8, 58 + 29, 905 + 35+13-8), 0.10f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 108, 905-13+8, 108 + 29, 905 + 35+13-8), 0.10f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 158, 905-12+7, 158 + 30, 905 + 36+12-7), 0.10f);
		animation->AddClip(clip);
	}
	// Spawn - 0 = 2
	{
		clip = new Clip(PlayMode::Loop);
		spriteFile = Textures + L"MegaManX/Enemy/LandMineEnemy5_new.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile),0.10f);
		spriteFile = Textures + L"MegaManX/Enemy/LandMineEnemy6.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.10f);
		spriteFile = Textures + L"MegaManX/Enemy/LandMineEnemy7.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.10f);
		spriteFile = Textures + L"MegaManX/Enemy/LandMineEnemy8.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.10f);
		animation->AddClip(clip);
	}
	// EndOfSpawn - 0 = 3
	{
		clip = new Clip(PlayMode::Loop);
		spriteFile = Textures + L"MegaManX/Enemy/LandMineEnemy_30.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.10f);
		spriteFile = Textures + L"MegaManX/Enemy/LandMineEnemy_31.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.10f);
		animation->AddClip(clip);
	}

	animation->Scale(2.5f, 2.5f);
	animation->RotationDegree(0, 0, 0);
	animation->Position(start);
	animation->DrawBound(false);
	animation->Play(stateNumber);

	/*****************************************************************/	// Effect

	crashEffect = new CrashEffect(start);
	
	/*****************************************************************/	// Weapon

	landMine = new LandMine(start);

}

LandMineEnemy::~LandMineEnemy()
{
	SAFE_DELETE(animation);

	SAFE_DELETE(landMine);

	SAFE_DELETE(crashEffect);
}

void LandMineEnemy::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);
	__super::HandleDestruction();

	D3DXVECTOR2 position = animation->Position();

	SetBMove(false);

	HandleStateNumber();

	HandleMovement();
	HandleAttack();
	HandleDestruction();

	animation->Play(stateNumber);
	animation->Update(V, P);

	/*****************************************************************/	// Weapon

	if (StateNumber() == 2 && BReadyToSPawn() == true)	// Spawn 상태
		landMine->SetBRender(true);
	landMine->GetEnemyPos(position);

	landMine->Update(V, P);

	/*****************************************************************/	// Effect

	if (BRender() == true)
		crashEffect->GetEnemyPos(animation->Position());
	crashEffect->Update(V, P);

	CheckCrashEffectDead();

}

void LandMineEnemy::Render()
{
	__super::Render();

	/*****************************************************************/ //Weapon

	landMine->Render();
	
	if (BRender() == true)
		animation->Render();

	/*****************************************************************/ //Effect

	crashEffect->Render();

}

/*****************************************************************/ //ETC

Sprite * LandMineEnemy::GetSprite()
{
	return animation->GetSprite();
}

/*****************************************************************/ //Spec

void LandMineEnemy::SetEnemySpec()
{
	SetHealthLevel(80);
	SetCollideLevel(40);
	SetAttackLevel(32);
}

/*****************************************************************/ //State Number

/*
	목적:
		State Number 변경을 관리합니다.
	설명:
		1. 먼저, State Number의 Frame Cycle을 체크합니다									-> Delay Function
		2. 다음으로, 적절한 Frame Cycle이 다 돌았는지 체크하고, State Number를 변경합니다	-> Check Condition Function
*/

void LandMineEnemy::HandleStateNumber()
{
	// 0(Move) to 1(Idle)
	CheckStateNumberChange0to1Condition();
	// 1's delay
	DelayIdleState();
	
	// 1(Idle) to 2(Spawn)
	CheckStateNumberChange1to2Condition();
	//// 2's delay
	DelaySpawnState();

	//// 2(Spawn) to 0(Idle 혹은 Move상태)
	CheckStateNumberChange2to0Condition();

}

void LandMineEnemy::CheckStateNumberChange0to1Condition()
{
	D3DXVECTOR2 position = animation->Position();

	if (BReadyToSPawn() == true
		&& StateNumber() == 0)
	{
		SetStateNumber(1);	//Idle
	}
}

void LandMineEnemy::DelayIdleState()
{
	if (StateNumber() == 1
		&& animation->GetClip()->CurrentFrame() == 3)
	{
		frameCycle++;
	}
}

void LandMineEnemy::CheckStateNumberChange1to2Condition()
{
	if (frameCycle == 3
		&& StateNumber() == 1)
	{
		SetStateNumber(2);	// Spawn
		frameCycle = 0;

		//Bool Setup
		SetBReadyToSpawn(false);
	}
}

void LandMineEnemy::DelaySpawnState()
{
	if (StateNumber() == 2 
		&& BEndOfSpawn() == false)
	{
		frameCycle++;
	}
}

void LandMineEnemy::CheckStateNumberChange2to0Condition()
{
	if (frameCycle == 20
		&& StateNumber() == 2)
	{
		SetStateNumber(3);	// End Of Spawn
		frameCycle = 0;

		// Bool Setup
		SetBEndOfSpawn(true);
	}
}

/*****************************************************************/ //Movement

/*
	목적:
		Movement를 관리합니다.
	설명:
		Player의 위치를 먼저 감지하고,
		Target 위치까지 이동합니다.

		* End Of Spawn 이 참일 경우, 모든 조건을 무시하고 이동을 시작합니다.
*/

void LandMineEnemy::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();

	if (BEndOfSpawn() == false)
	{
		if (playerPos.x > targetPosition.x)
		{		
			if (StateNumber() == 0 && fabs(position.x - playerPos.x) >= 150.f)
				Move();
			else
				StopMove();
		}
	}
	else
	{
		Move();
	}
}

void LandMineEnemy::Move()
{
	D3DXVECTOR2 position = animation->Position();

	position.x -= moveSpeed * Timer->Elapsed();

	SetBMove(true);
	animation->Position(position);
}

/*
	목적:
		이동을 멈추고, Spawn 준비에 들어갑니다.
	설명:
		1. bMove = false
		2. bReadyToSpawn = true
*/

void LandMineEnemy::StopMove()
{
	SetBMove(false);
	SetBReadyToSpawn(true);
}

/*****************************************************************/ //Handle

void LandMineEnemy::HandleAttack()
{
	__super::HandleAttack();	//SetBAttack(true)
}

void LandMineEnemy::HandleDamage(int damage)
{
	__super::HandleDamage(damage);
}

void LandMineEnemy::HandleDestruction()
{
	__super::HandleDestruction();

	if (HealthLevel() <= 0)
	{
		SetBRender(false);
		crashEffect->SetBRender(true);
		landMine->SetBRender(true);

		SetCollideLevel(0);
		SetAttackLevel(0);
	}
}

/*****************************************************************/ //Effect Dead Check

void LandMineEnemy::CheckCrashEffectDead()
{
	if (crashEffect->BDead() == true
		&& landMine->crashEffect->BDead() == true)
	{
		crashEffect->SetBRender(false);
	}
}
