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
	����:
		�ʼ�������, ��� Enemy�� �Ļ� ��ü���� Spec�� �����ؾ� �մϴ�.
	����:
		1. Health Level(ü��) ����
		2. Collide Level(�浹 �� ���ط�) ����
		3. Attack Level (���ݷ�) ����
*/

void MissileEnemy::SetEnemySpec()
{
	SetHealthLevel(320);
	SetCollideLevel(24);
	SetAttackLevel(20);
}

/*****************************************************************/ //State

/*
	����:
		Player�� ��ġ�� ���� ������ �Ѿ��, ������ �غ� ���մϴ�.
	����:
		1. StateNumber�� 0 ���� 1�� ��ȯ�˴ϴ�.
		2. StateNumber�� 1 �̸�, FrameNumber�� 1 Cycle�� ���� ����, FrameCycle�� �����մϴ�.
		3. StateNumber�� 1 �̸�, FrameCycle�� 1�̵Ǹ�, StateNumber�� 2�� ������ŵ�ϴ�.
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

	����:
		StateNumber�� 0�� ���¿���, Player�� �ݰ� ������ ������ StateNumber�� 1�� ��ȯ�մϴ�.

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

	����:
		StateNumber�� 1�� ���¿���, Frame Cycle�� ������ �� ����,
		FrameCycle�� 1�� ������ŵ�ϴ�.

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

	����:
		StateNumber�� 1�� ���¿���, FrameCycle�� 1�� �Ǹ�,
		StateNumber�� 2�� ��ȯ��ŵ�ϴ�.

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

	����:
		StaetNumber�� 3�� ���¿���, Frmae Cycle�� ������ �� ����,
		FrameCycle�� 1�� ������ŵ�ϴ�.

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

	����:
		StateNumber�� 3�� ���¿���, FrameCycle�� 1�� �Ǹ�,
		StateNumber�� 0�� ��ȯ��ŵ�ϴ�.

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
	����:
		���� �غ� ����, ���̾� ������ �����մϴ�.
	����:
		1. StateNumber�� 2�� �Ǹ�, �� ���� �غ� �Ϸ�Ǹ� ȣ��˴ϴ�.
		2. BAttack() == true �� �����˴ϴ�.
		3. Missile�� Spawn�մϴ�. 
*/

void MissileEnemy::HandleAttack()
{
	__super::HandleAttack();	// SetBAttack(true)
	D3DXVECTOR2 position = animation->Position();

	if(BSpawnMissile() == true
		&& bEndOfSpawnMissile == false)
		SpawnMissile(position);		// 2���� �̻��� �߻�
}

/*****************************************************************/ //Weapons

/*
	����:
		�� 2���� �̻��ϵ��� ��� �߻�Ȱ��� üũ�մϴ�.
	����:
		1. missileNum�� 2���� �Ȱ��� üũ�մϴ�.
		2. State Number�� 2(Spawn)���� 3(EndOfSpawn)���� �����մϴ�.
		3. bAttack �� false�� ��ȯ�մϴ�.
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
	����:
		State Number�� ���� ���·� ��ȯ�Ǹ�,
		�� 2���� �̻����� �߻��մϴ�.
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
	����:
		Health Level�� 0 ���ϰ� �Ǹ�, Crash Effect�� �������մϴ�.
	����:
		Enemy�� �������� ���߰�, Crash Effect�� �������� �����մϴ�.
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
	����:
		���������� Crash Effect���� ��� Play�ǰ� ���� ���·� ��ȯ�մϴ�.
	����:

		1.	Crash Effect�� ��� Clip�� Play�ǰ� '����' ���·� ��ȯ�˴ϴ�.
		2.	Enemy�� ��� Crash Effect�� ��� Play�ǰ� ���� ���·� ��ȯ�Ǿ�� �ϱ� ������,
			Crash Effect�� '����' ���¸� üũ�ϰ�, ���������� '����' ���·� ��ȯ�˴ϴ�.
*/

void MissileEnemy::CheckCrashEffectDead()
{
	if (crashEffect->BDead() == true)
	{
		SetBDead(true);
	}
}
