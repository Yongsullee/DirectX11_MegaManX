#include "stdafx.h"
#include "LandMine.h"
#include "Effects/Effect.h"
#include "Effects/CrashEffect.h"
#include "Effects/MakingHoleEffect.h"

LandMine::LandMine(D3DXVECTOR2 start)
	:Weapon()
{
	SetWeaponSpec();

	animation = new Animation;

	wstring shaderFile = Shaders + L"009_Sprite.fx";
	wstring spriteFile = L"";

	Clip* clip;
	// ReadyToMove = 0
	{
		clip = new Clip(PlayMode::Loop);
		spriteFile = Textures + L"MegaManX/Weapons/LandMineReady0.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.15f);
		spriteFile = Textures + L"MegaManX/Weapons/LandMineReady2.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.15f);
		spriteFile = Textures + L"MegaManX/Weapons/LandMineReady3.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.15f);
		spriteFile = Textures + L"MegaManX/Weapons/LandMineWeapon.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.15f);
		animation->AddClip(clip);
	}
	// Idle = 1
	{
		clip = new Clip(PlayMode::Loop);
		spriteFile = Textures + L"MegaManX/Weapons/LandMineWeapon.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.25f);
		animation->AddClip(clip);
	}

	animation->Scale(2.5f, 2.5f);
	animation->Position(start);
	animation->DrawBound(false);
	animation->Play(stateNumber);

	/*****************************************************************/ // Effect

	// 1. Crash Effect
	crashEffect = new CrashEffect(start);
	
	// 2. Hole Effect
	MakingHoleEffect* holeEffect;

	holeEffect = new MakingHoleEffect(start, 1);
	holeEffects.push_back(holeEffect);
	holeEffect = new MakingHoleEffect(start, 2);
	holeEffects.push_back(holeEffect);
	holeEffect = new MakingHoleEffect(start, 3);
	holeEffects.push_back(holeEffect);

}

LandMine::~LandMine()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(crashEffect);
	for (MakingHoleEffect* effect : holeEffects)
		SAFE_DELETE(effect);
}

void LandMine::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);
	D3DXVECTOR2 position = animation->Position();

	SetBMove(false);

	AdjustPosition();	// State Number = 0 일때,

	/******************************************************/ // State & Movement

	if (BRender() == true
		&& BDead() == false)
	{
		HandleStateNumber();
		HandleMovement();	// 추락
		HandleDestruction();
	}

	animation->Play(stateNumber);
	animation->Update(V, P);

	/******************************************************/ // Effect

	// 1. Crash Effect
	crashEffect->GetWeaponPos(position);
	crashEffect->Update(V, P);

	//CheckCrashEffectDead();

	// 2. Making Hole Effect
	for (MakingHoleEffect* effect : holeEffects)
	{
		effect->GetWeaponPos(position);
		effect->Update(V, P);	
	}

	if (crashEffect->BDead() == true)
	{
		for (MakingHoleEffect* effect : holeEffects)
			effect->SetBRender(true);
	}

}

void LandMine::Render()
{
	if (BRender() == true)
	{
		__super::Render();
		animation->Render();
	}

	/*****************************************************************/ //Effect

	// 1. Making Hole Effect
	for (MakingHoleEffect* effect : holeEffects)
		effect->Render();

	// 2. Crash Effect
	crashEffect->Render();

}

/*****************************************************************/ // Spec

void LandMine::SetWeaponSpec()
{
	SetAttackLevel(20);
	SetHealthLevel(150);
	SetCollideLevel(50);
}

/*****************************************************************/ // Position & Movement

void LandMine::AdjustPosition()
{
	D3DXVECTOR2 position = animation->Position();

	if (StateNumber() == 0)
	{
		position.x = enemyPos.x;
		position.y = enemyPos.y-28.f;
	}
	
	animation->Position(position);

}

void LandMine::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();

	if (StateNumber() == 1)
	{
		if (position.y < GroundPosition())
		{
			position.y = GroundPosition();
			SetBOnGround(true);
		}
		
		if(BOnGround() == false)
		{
			position = Move(position);
		}
	}

	animation->Position(position);
}

D3DXVECTOR2 LandMine::Move(D3DXVECTOR2 _position)
{
	velocity += gravity;
	_position.y += velocity * Timer->Elapsed();

	return _position;
}

/*****************************************************************/ // Attack

void LandMine::HandleAttack()
{
	__super::HandleAttack();
}

/*****************************************************************/ // Damage

void LandMine::HandleDamage(int damage)
{
	int presentHealthLevel = HealthLevel();

	presentHealthLevel -= damage;

	if (presentHealthLevel < 0)
	{
		presentHealthLevel = 0;
	}

	SetHealthLevel(presentHealthLevel);
}

/*****************************************************************/ // Handle

void LandMine::HandleDestruction()
{
	__super::HandleDestruction();

	if (BOnGround() == false)
	{
		if (HealthLevel() <= 0)
		{
			SetBRender(false);

			if (crashEffect->BDead() == false)
				crashEffect->SetBRender(true);

			SetCollideLevel(0);
			SetAttackLevel(0);
		}
	}
	else
	{
		SetBRender(false);

		if (crashEffect->BDead() == false)
			crashEffect->SetBRender(true);

		for (MakingHoleEffect* effect : holeEffects)
			effect->SetBRender(true);

		SetCollideLevel(0);
		SetAttackLevel(0);
	}
}

/*****************************************************************/ // State

void LandMine::HandleStateNumber()
{
	DelayReadyToMoveState();

	CheckStateNumberChange0to1Condition();
}

void LandMine::DelayReadyToMoveState()
{
	if (StateNumber() == 0
		&& frameCycle == 0
		&& animation->GetClip()->CurrentFrame() == 3)
	{
		frameCycle++;
	}
}

void LandMine::CheckStateNumberChange0to1Condition()
{
	if (StateNumber() == 0
		&& frameCycle == 1)
	{
		SetStateNumber(1);	//	Idle
		frameCycle = 0;
	}
}

/*****************************************************************/ // Effect

void LandMine::CheckCrashEffectDead()
{
	if (crashEffect->BDead() == true)
	{
		SetBDead(true);
	}
}
