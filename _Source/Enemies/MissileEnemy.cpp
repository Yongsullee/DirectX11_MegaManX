#include "stdafx.h"
#include "Enemies/MissileEnemy.h"
#include "Effects/CrashEffect.h"
#include "Weapons/Weapon.h"
#include "Weapons/Missile.h"

MissileEnemy::MissileEnemy(D3DXVECTOR2& _position)
	:Enemy()
	, lastDirection(0)
	, bOnGround(true), bSpawnMissile(false), bEndOfSpawnMissile(false)
	, groundPosition(0.f)
	, angle(0.f)
	, frameCycle(0)
	, missileNum(0)
{
	SetEnemySpec();

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Enemy/MissileEnemy.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	/*****************************************************************/ // Clip

	Clip* clip;
	//Idle, stateNumber = 0 
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 7, 4, 7 + 42, 4 + 57), 0.1f);
		animation->AddClip(clip);
	}
	//ReadyToSpawn, stateNumber = 1 
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 51, 0, 51 + 42, 61), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 97, 0, 97 + 44, 61), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 144, 0, 144 + 44, 61), 0.20f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 191, 0, 191 + 44, 61), 0.25f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 237, 0, 237 + 44, 61), 0.30f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 283, 0, 283 + 52, 61), 0.10f);

		clip->AddFrame(new Sprite(spriteFile, shaderFile, 12, 75, 12 + 44, 75 + 62), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 71, 75, 71 + 44, 75 + 62), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 132, 75, 132 + 44, 75 + 62), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 179, 75, 179 + 44, 75 + 62), 0.17f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 225, 75, 225 + 44, 75 + 62), 0.2f);
		animation->AddClip(clip);
	}
	// Spawn, stateNumber = 2
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 132, 75, 132 + 44, 75 + 62), 0.5f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 179, 75, 179 + 44, 75 + 62), 0.2f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 225, 75, 225 + 44, 75 + 62), 0.5f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 179, 75, 179 + 44, 75 + 62), 0.2f);
		animation->AddClip(clip);
	}
	// End Of Spawn, stateNumber = 3
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 225, 75, 225 + 44, 75 + 62), 0.2f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 179, 75, 179 + 44, 75 + 62), 0.22f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 132, 75, 132 + 44, 75 + 62), 0.25f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 71, 75, 71 + 44, 75 + 62), 0.25f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 12, 75, 12 + 44, 75 + 62), 0.25f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 283, 0, 283 + 52, 61), 0.25f);

		clip->AddFrame(new Sprite(spriteFile, shaderFile, 237, 0, 237 + 44, 61), 0.30f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 191, 0, 191 + 44, 61), 0.17f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 144, 0, 144 + 44, 61), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 97, 0, 97 + 44, 61), 0.12f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 51, 0, 51 + 42, 61), 0.1f);
		animation->AddClip(clip);
	}

	/*****************************************************************/

	animation->Scale(2.5f, 2.5f);
	animation->Position(_position);
	animation->DrawBound(false);
	animation->Play(stateNumber);
	
	/*****************************************************************/ //Weapons

	for (int i = 0; i < maxMissileNum; i++)
	{
		Missile* weapon = new Missile(shaderFile, _position, DirectionInDegree::Right);
		missiles.push_back(weapon);
	}

	/*****************************************************************/ //Effect

	crashEffect = new CrashEffect(_position);

}

MissileEnemy::~MissileEnemy()
{
	SAFE_DELETE(animation);

	for (Missile* weapon : missiles)
		SAFE_DELETE(weapon);

	SAFE_DELETE(crashEffect);
}

bool test = false;
void MissileEnemy::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);
	__super::HandleDestruction();

	D3DXVECTOR2 position = animation->Position();

	SetBMove(false); 

	HandleStateNumber();

	if (StateNumber() == 2
		&& BDead() == false);
		HandleAttack();

	HandleDestruction();

	if (position.y < GroundPosition())
	{
		position.y = GroundPosition();
		SetBOnGround(true);
	}

	CheckEndOfSpawnMissileCondition();

	animation->Play(stateNumber);
	animation->Update(V, P);

	/*****************************************************************/ // Weapon

	for (Missile* weapon : missiles)
	{
		weapon->GetEnemyPos(position);
		weapon->Update(V, P);
	}

	/*****************************************************************/ //Effect

	if (BRender() == true)
		crashEffect->GetEnemyPos(animation->Position());
	crashEffect->Update(V, P);

	CheckCrashEffectDead();
}

void MissileEnemy::Render()
{
	__super::Render();

	if (BRender() == true)
		animation->Render();

	/*****************************************************************/ //Weapons

	for (Missile* weapon : missiles)
		weapon->Render();

	/*****************************************************************/ //Effect

	crashEffect->Render();

}

/*****************************************************************/ //ETC

Sprite * MissileEnemy::GetSprite()
{
	return animation->GetSprite();
}

/*****************************************************************/ //Spec

/*
	목적:
		필수적으로, 모든 Enemy의 파생 객체들은 Spec을 설정해야 합니다.
	설명:
		1. Health Level(체력) 설정
		2. Collide Level(충돌 시 피해량) 설정
		3. Attack Level (공격력) 설정
*/

void MissileEnemy::SetEnemySpec()
{
	SetHealthLevel(320);
	SetCollideLevel(24);
	SetAttackLevel(20);
}

/*****************************************************************/ //State

/*
	목적:
		Player의 위치가 일정 범위를 넘어서면, 공격할 준비를 취합니다.
	설명:
		1. StateNumber가 0 에서 1로 전환됩니다.
		2. StateNumber가 1 이며, FrameNumber가 1 Cycle을 돌고 나면, FrameCycle이 증가합니다.
		3. StateNumber가 1 이며, FrameCycle이 1이되면, StateNumber를 2로 증가시킵니다.
*/

void MissileEnemy::HandleStateNumber()
{
	D3DXVECTOR2 position = animation->Position();
	
	CheckStateNumber0to1Condition();

	DelayReadyToAttackState();

	CheckStateNumber1to2Condition();

	DelayReturnToIdleState();

	CheckStateNumber3to1Condition();

}

/*

	목적:
		StateNumber가 0인 상태에서, Player가 반경 범위에 들어오면 StateNumber를 1로 전환합니다.

*/

void MissileEnemy::CheckStateNumber0to1Condition()
{
	D3DXVECTOR2 position = animation->Position();

	if (playerPos.x > position.x - 0.6f * Width
		&& StateNumber() == 0)
	{
		frameCycle++;
		if (frameCycle > 20)
		{
			frameCycle = 0;
			SetStateNumber(1);
		}
	}
}

/*

	목적:
		StateNumber가 1인 상태에서, Frame Cycle을 완전히 다 돌면,
		FrameCycle을 1로 증가시킵니다.

*/

void MissileEnemy::DelayReadyToAttackState()
{
	if (StateNumber() == 1
		&& animation->GetClip()->CurrentFrame() == 10)
	{
		frameCycle++;
	}
}

/*

	목적:
		StateNumber가 1인 상태에서, FrameCycle이 1이 되면,
		StateNumber를 2로 전환시킵니다.

*/

void MissileEnemy::CheckStateNumber1to2Condition()
{
	if (StateNumber() == 1 
		&& frameCycle == 1)
	{
		frameCycle = 0;
		SetStateNumber(2);
		SetBSpawnMissile(true);
	}
}

/*

	목적:
		StaetNumber가 3인 상태에서, Frmae Cycle을 완전히 다 돌면,
		FrameCycle을 1로 증가시킵니다.

*/

void MissileEnemy::DelayReturnToIdleState()
{
	if (StateNumber() == 3
		&& animation->GetClip()->CurrentFrame() == 10)
	{
		frameCycle++;
	}
}

/*

	목적:
		StateNumber가 3인 상태에서, FrameCycle이 1이 되면,
		StateNumber를 0로 전환시킵니다.

*/

void MissileEnemy::CheckStateNumber3to1Condition()
{
	if (StateNumber() == 3
		&& frameCycle == 1)
	{
		SetStateNumber(0);
		frameCycle = 0;
	}
}

/*****************************************************************/ //Attack

/*
	목적:
		공격 준비에 들어가고, 곧이어 공격을 시작합니다.
	설명:
		1. StateNumber가 2가 되면, 즉 공격 준비가 완료되면 호출됩니다.
		2. BAttack() == true 로 설정됩니다.
		3. Missile을 Spawn합니다. 
*/

void MissileEnemy::HandleAttack()
{
	__super::HandleAttack();	// SetBAttack(true)
	D3DXVECTOR2 position = animation->Position();

	if(BSpawnMissile() == true
		&& bEndOfSpawnMissile == false)
		SpawnMissile(position);		// 2개의 미사일 발사
}

/*****************************************************************/ //Weapons

/*
	목적:
		총 2개의 미사일들이 모두 발사된것을 체크합니다.
	설명:
		1. missileNum이 2개가 된것을 체크합니다.
		2. State Number를 2(Spawn)에서 3(EndOfSpawn)으로 변경합니다.
		3. bAttack 을 false로 전환합니다.
*/
void MissileEnemy::CheckEndOfSpawnMissileCondition()
{
	if (BSpawnMissile() == true
		&& bEndOfSpawnMissile == true)
	{
		SetStateNumber(3);
		missileNum = 0;

		bEndOfSpawnMissile = false;
		SetBSpawnMissile(false);
		SetBAttack(false);
	}
}

/*
	목적:
		State Number가 공격 상태로 전환되면,
		총 2개의 미사일을 발사합니다.
*/

void MissileEnemy::SpawnMissile(D3DXVECTOR2 & _position)
{
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	float missilePosX = 0.f;
	float missilePosY = 0.f;
	float firstMissilePosX = 0.f;
	float distanceFromEnemy = 0.f;

		if (missileNum % 2 == 0)
		{
			missilePosX = _position.x - 35.f;
			missilePosY = _position.y - 5.f;
			missiles.at(missileNum)->AdjustPosition(D3DXVECTOR2(missilePosX, missilePosY));
			missiles.at(missileNum)->SetBRender(true);
			missileNum++;
		}

		if (missileNum % 2 == 1)
		{
			float firstMissilePosX = missiles.at(missileNum-1)->GetSprite()->Position().x;
			float distanceFromEnemy = abs(_position.x - firstMissilePosX);
			if (distanceFromEnemy > 500.f)
			{
				missilePosX = _position.x - 35.f;
				missilePosY = _position.y - 50.f;
				missiles.at(missileNum)->AdjustPosition(D3DXVECTOR2(missilePosX, missilePosY));
				missiles.at(missileNum)->SetBRender(true);

				bEndOfSpawnMissile = true;
				missileNum++;
			}
		}
}

/*****************************************************************/ //Damage Taken

void MissileEnemy::HandleDamage(int damage)
{
	__super::HandleDamage(damage);
}

/*****************************************************************/ //Destruction

/*
	목적:
		Health Level이 0 이하가 되면, Crash Effect를 렌더링합니다.
	설명:
		Enemy의 렌더링을 멈추고, Crash Effect의 렌더링을 시작합니다.
*/

void MissileEnemy::HandleDestruction()
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

/*****************************************************************/ //Dead State

/*
	목적:
		최종적으로 Crash Effect까지 모두 Play되고 죽은 상태로 전환합니다.
	설명:

		1.	Crash Effect는 모든 Clip이 Play되고 '죽음' 상태로 전환됩니다.
		2.	Enemy의 경우 Crash Effect가 모두 Play되고 죽음 상태로 전환되어야 하기 때문에,
			Crash Effect의 '죽음' 상태를 체크하고, 최종적으로 '죽음' 상태로 전환됩니다.
*/

void MissileEnemy::CheckCrashEffectDead()
{
	if (crashEffect->BDead() == true)
	{
		SetBDead(true);
	}
}
