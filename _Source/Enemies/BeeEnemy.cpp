#include "stdafx.h"
#include "BeeEnemy.h"
#include "Effects/Effect.h"
#include "Effects/CrashEffect.h"

BeeEnemy::BeeEnemy(D3DXVECTOR2 start, D3DXVECTOR2 target)
	:Enemy()
	, targetPosition(target)
{
	SetEnemySpec();

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Enemy/BeeEnemy.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	Clip* clip;
	// Move = 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 8, 11, 8 + 35, 11 + 36), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 51, 10, 51 + 36, 10 + 38), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 95, 8, 95 + 25, 8 + 40), 0.05f);
		animation->AddClip(clip);
	}
	// Spawn = 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 127, 9, 127 + 36, 9 + 39), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 167, 7, 167 + 25, 7 + 41), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 198, 11, 198 + 35, 11 + 37), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 238, 8, 238 + 36, 8 + 40), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 281, 6, 281 + 25, 6 + 42), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 313, 10, 313 + 33, 10 + 38), 0.05f);
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

BeeEnemy::~BeeEnemy()
{
	SAFE_DELETE(animation);

	SAFE_DELETE(crashEffect);
}

void BeeEnemy::Update(D3DXMATRIX & V, D3DXMATRIX & P)
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

void BeeEnemy::Render()
{
	__super::Render();

	if (BRender() == true)
		animation->Render();

	/*****************************************************************/ //Effect

	crashEffect->Render();
}

/*****************************************************************/ //ETC

Sprite * BeeEnemy::GetSprite()
{
	return animation->GetSprite();
}

/*****************************************************************/ //Spec

void BeeEnemy::SetEnemySpec()
{
	SetHealthLevel(80);
	SetCollideLevel(40);
	SetAttackLevel(32);
}

/*****************************************************************/ //Handle

void BeeEnemy::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();

	if (playerPos.x >= targetPosition.x)
	{
		position.x -= moveSpeed * Timer->Elapsed();

		SetBMove(true);
		animation->Position(position);
	}
}

void BeeEnemy::HandleAttack()
{
	__super::HandleAttack();
}

void BeeEnemy::HandleDamage(int damage)
{
	__super::HandleDamage(damage);
}

void BeeEnemy::HandleDestruction()
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

void BeeEnemy::CheckCrashEffectDead()
{
	if (crashEffect->BDead() == true)
	{
		SetBDead(true);
		crashEffect->SetBRender(false);
	}
}
