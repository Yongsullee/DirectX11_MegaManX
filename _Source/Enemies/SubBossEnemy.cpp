#include "stdafx.h"
#include "SubBossEnemy.h"
#include "Enemies/SubBossPawn.h"
#include "Effects/Effect.h"
#include "Effects/CrashEffect.h"
#include "Effects/DustEffect.h"

SubBossEnemy::SubBossEnemy(D3DXVECTOR2 _pos, float _targetPos)
	:Enemy()
	, targetPos(_targetPos)
{
	SetEnemySpec();

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Enemy/Enemies.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	Clip* clip;
	// Idle, 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 10, 205, 10 + 125, 205 + 85), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 167-25, 204, 167 + 125, 204 + 87), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 317-25, 204, 317 + 125, 204 + 87), 0.03f);
		animation->AddClip(clip);
	}
	// Spawn, 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 10, 330, 10 + 125, 330 + 98), 0.015f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 10, 455, 10 + 125, 455 + 98), 0.015f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 167-25, 329, 167 + 125, 329 + 98), 0.015f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 167-25, 454, 167 + 125, 454 + 98), 0.015f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 317-25, 329, 317 + 125, 329 + 98), 0.015f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 317-25, 454, 317 + 125, 454 + 98), 0.015f);
		animation->AddClip(clip);
	}
	// Dead, 2
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 17, 599, 17+111, 599+48), 0.025f);
		animation->AddClip(clip);
	}

	animation->Scale(2.5f, 2.5f);
	animation->RotationDegree(0, 0, 0);
	animation->Position(_pos);
	animation->DrawBound(false);
	animation->Play(stateNumber);

	/***************************************/ // Pawns

	SubBossPawn* pawn;

	for (int i = 0; i < subBossNum; i++)
	{
		pawn = new SubBossPawn(_pos);
		pawn->SetTargetPos(_targetPos);
		pawn->SetBRender(false);
		subBossPawns.push_back(pawn);
	}

	/***************************************/ // Crash Effect & Dust Effect

	CrashEffect* effect1;
	DustEffect* effect2;

	for (int i = 0; i < crashEffectsNum; i++)
	{
		effect1 = new CrashEffect(_pos);
		effect1->SetBAdjustable(true);
		crashEffects.push_back(effect1);

	}

	for (int i = 0; i < dustEffectsNum; i++)
	{
		effect2 = new DustEffect(_pos);
		effect2->SetBAdjustable(true);
		dustEffects.push_back(effect2);
	}
}

SubBossEnemy::~SubBossEnemy()
{
	SAFE_DELETE(animation);

	/***************************************/ // Pawns

	for (SubBossPawn* pawn : subBossPawns)
		SAFE_DELETE(pawn);

	/***************************************/ // Crash Effect & Dust Effect

	for (CrashEffect* effect : crashEffects)
		SAFE_DELETE(effect);
	for (DustEffect* effect : dustEffects)
		SAFE_DELETE(effect);
}

void SubBossEnemy::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);
	__super::HandleDestruction();

	SetBMove(false);

	HandleStateNumber();
	HandleMovement();
	HandleAttack();
	HandleDestruction();

	animation->Play(stateNumber);
	animation->Update(V, P);

	/*****************************************************************/	// Pawns
	
	if(StartMove() == true)
		HandlePawnsRender(animation->Position());

	for (SubBossPawn* pawn : subBossPawns)
	{
		pawn->GetSubBossEnemyPos(*this);

		pawn->Update(V, P);
	}

	/*****************************************************************/	// Effects
	
	HandleCrashEffectsRender();
	HandleDustEffectsRender();

	/*
		목적:
			각 Effect의 위치 Offset을 랜덤으로 설정합니다.
	*/

	if (BRender() == true)
	{
		// Crash Effects Udpate
		for (CrashEffect* effect : crashEffects)
		{
			effect->GetEnemyPos(animation->Position());

			effect->SetOffsetPositionX(Math::Random(CRASH_OFFSET_MIN, CRASH_OFFSET_MAX));
			effect->SetOffsetPositionY(Math::Random(CRASH_OFFSET_MIN, CRASH_OFFSET_MAX));

			effect->Update(V, P);
		}

		// Dust Effects Udpate
		for (int i=0; i< dustEffects.size(); i++)
		{
			dustEffects.at(i)->GetEnemyPos(animation->Position());

			dustEffects.at(i)->SetOffsetPositionX(Math::Random(DUST_OFFSET_MIN, DUST_OFFSET_MAX));
			dustEffects.at(i)->SetOffsetPositionY(-60.f);

			dustEffects.at(i)->Update(V, P);
		}
	}
}

void SubBossEnemy::Render()
{

	/*****************************************************************/ //Pawns

	for (SubBossPawn* pawn : subBossPawns)
		pawn->Render();

	/*****************************************************************/ //Sub Boss

	__super::Render();
	if (BRender() == true)
		animation->Render();
	

	/*****************************************************************/ //Effects

	for (CrashEffect* effect : crashEffects)
		effect->Render();
	for (DustEffect* effect : dustEffects)
		effect->Render();
}

/*****************************************************************/ //ETC

Sprite * SubBossEnemy::GetSprite()
{
	return animation->GetSprite();
}

/*****************************************************************/ //Spec

void SubBossEnemy::SetEnemySpec()
{
	SetHealthLevel(500);
	SetCollideLevel(20);
	SetAttackLevel(20);
}

/*****************************************************************/ //Handle

void SubBossEnemy::HandleStateNumber()
{
	CheckStateNumberChange0to1Condition();

	CheckStateNumberChang1to2Conditon();
}

void SubBossEnemy::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();

	if (stateNumber == 2)
	{
		Fall(position);
	}
	else
	{
		if (playerPos.x >= targetPos)
		{
			Move(position);
		}
	}

	if (StateNumber() == 1
		&& bAdjustable == true)
	{
		position.y -= 20.f;
		bAdjustable = false;
	}

	animation->Position(position);
}

void SubBossEnemy::HandleAttack()
{
	__super::HandleAttack();
}

void SubBossEnemy::HandleDamage(int damage)
{
	__super::HandleDamage(damage);
}

void SubBossEnemy::HandleDestruction()
{
	__super::HandleDestruction();

	if (HealthLevel() <= 0)
	{
		SetBDead(true);

		crashEffects.at(0)->SetBRender(true);

		SetCollideLevel(0);
		SetAttackLevel(0);

		for (SubBossPawn* pawn : subBossPawns)
			pawn->SetSubBossBDead(true);
	}
}

/*****************************************************************/ //State

void SubBossEnemy::CheckStateNumberChange0to1Condition()
{
	D3DXVECTOR2 position = animation->Position();

	if (StateNumber() == 0
		&& position.x <= targetPos+600.f)
	{
		SetStateNumber(1);
	}
}

void SubBossEnemy::CheckStateNumberChang1to2Conditon()
{
	if (HealthLevel() <= 0)
	{
		SetStateNumber(2);
	}
}

/*****************************************************************/ //Movement

/*
	목적:
		Dead State로 전환되면 추락합니다.
	설명:
		1. State Number가 2가 되면 추락합니다.
		2. 지면에 충돌하면, 반동으로 튀어올라 곧 지면에 정지합니다.
*/

void SubBossEnemy::Move(D3DXVECTOR2 & _pos)
{
	_pos.x -= moveSpeed * Timer->Elapsed();

	SetStartMove(true);
}

void SubBossEnemy::Fall(D3DXVECTOR2 & _pos)
{
	// Dead State
	if (StateNumber() == 2)
	{
		velocity += gravity * Timer->Elapsed();
		_pos.y += velocity;

		if (LandCondition(_pos.y) == true)
		{
			// 반동
			if (rebound <= 0)
			{
				_pos.y = GroundPosition();
			}
			else
			{
				_pos.y += rebound;
				velocity = 0.f;

				rebound -= 50.f;
			}
				
		}
	}
}

bool SubBossEnemy::LandCondition(float f)
{
	if (f <= GroundPosition())
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************************/ //Pawns

/*
	목적:
		Sub Boss Pawn의 Render를 핸들링합니다.
	설명:
		1. 먼저 StateNumber가 1이되면 첫 번째 병사를 Render합니다.
		2. 그 후, 이전 병사의 Clip이 "Move"로 Play되면 다음 병사를 Render합니다.
*/

void SubBossEnemy::HandlePawnsRender(D3DXVECTOR2 _pos)
{

	if (_pos.x <= targetPos + 750.f)
	{
		subBossPawns.at(0)->SetBRender(true);
	}

	for (int i = 0; i < subBossPawns.size()-1; i++)
	{
		// 이전 병사의 Clip이 "Move"로 Play되면 다음 병사를 Render합니다.
		if (subBossPawns.at(i)->StateNumber() == 4)
			subBossPawns.at(i + 1)->SetBRender(true);
	}

}

/*****************************************************************/ //Crash Effect

/*

	목적:
		Crash Effect의 Render를 핸들링합니다.
	설명:
		1. Crash Effect의 Dead 여부 체크
		2. Dead == true ? 다음 Crash Effect Render

*/

void SubBossEnemy::HandleCrashEffectsRender()
{
	for (int i = 0; i < crashEffects.size()-1; i++)
	{
		if (CheckCrashEffectDead(*crashEffects.at(i)) == true)
			if (crashEffects.at(i + 1)->BRender() == false)
				crashEffects.at(i + 1)->SetBRender(true);
	}
}

bool SubBossEnemy::CheckCrashEffectDead(class CrashEffect& _crashEffect)
{
	if (_crashEffect.BDead() == true)
		return true;
	else
		return false;
}

/*****************************************************************/ //Dust Effect

/*

	목적:
		Dust Effect의 Render를 핸들링합니다.
	설명:
		1. Dust Effect의 Dead 여부 체크
		2. Dead == true ? 다음 Dust Effect Render

*/

void SubBossEnemy::HandleDustEffectsRender()
{
	if (BCollideWithPartiableBridge() == true && rebound == 0)
	{
		for (DustEffect* effect : dustEffects)
			effect->SetBRender(true);
	}
}

bool SubBossEnemy::CheckDustEffectDead(DustEffect & _dustEffect)
{
	if (_dustEffect.BDead() == true)
		return true;
	else
		return false;
}
