#include "stdafx.h"
#include "SubBossPawn.h"
#include "Enemies/SubBossEnemy.h"
#include "Effects/Effect.h"
#include "Effects/CrashEffect.h"

SubBossPawn::SubBossPawn(D3DXVECTOR2 position)
	:Enemy()
{
	SetEnemySpec();

	animation = new Animation;

	wstring spriteFile = L"";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	/*****************************************************************/ // Clip

	Clip* clip;
	// Init , 0
	{
		clip = new Clip(PlayMode::Loop);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-0.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.1f);
		animation->AddClip(clip);
	}
	// ReadyToStand , 1
	{
		clip = new Clip(PlayMode::Loop);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn0-0.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn0-1.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn0-2.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn0-3.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		animation->AddClip(clip);
	}
	// Stand Up, 2
	{
		clip = new Clip(PlayMode::Loop);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-0.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-1.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-2.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-3.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-4.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-5.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-6.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-7.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-8.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-9.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-10.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-11.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn1-12.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		animation->AddClip(clip);
	}
	// Idle, 3
	{
		clip = new Clip(PlayMode::Loop);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn3-0.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn3-1.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn3-2.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn3-3.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.05f);
		animation->AddClip(clip);
	}
	// Move, 4
	{
		clip = new Clip(PlayMode::Loop);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn4-0.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.1f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn4-1.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.1f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn4-2.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.1f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn4-3.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.1f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn4-4.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.1f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn4-5.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.1f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn4-6.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.1f);
		spriteFile = Textures + L"MegaManX/Enemy/SubBossPawn4-7.png";
		clip->AddFrame(new Sprite(spriteFile, shaderFile), 0.1f);
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

SubBossPawn::~SubBossPawn()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(crashEffect);
}

void SubBossPawn::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	D3DXVECTOR2 position = animation->Position();

	__super::Update(V, P);
	__super::HandleDestruction();

	SetBMove(false);

	HandleStateNumber();
	HandleMovement();
	HandleAttack();
	HandleDestruction();

	if (StateNumber() == 4)
		animation->RotationDegree(0, DirectionInDegree::Right, 0);
	animation->Play(stateNumber);
	animation->Update(V, P);
	
	/*****************************************************************/ //Effect

	if (BRender() == true)
		crashEffect->GetEnemyPos(position);
	crashEffect->Update(V, P);

	CheckCrashEffectDead();
}

void SubBossPawn::Render()
{
	__super::Render();

	if (BRender() == true)
		animation->Render();
	
	/*****************************************************************/ //Effect

	crashEffect->Render();

}

/*****************************************************************/ // ETC

Sprite * SubBossPawn::GetSprite()
{
	return animation->GetSprite();
}

void SubBossPawn::GetSubBossEnemyPos(SubBossEnemy & subBoss)
{
	subBossPos = subBoss.GetSprite()->Position();
}

/*****************************************************************/ //Spec

void SubBossPawn::SetEnemySpec()
{
	SetHealthLevel(60);
	SetCollideLevel(10);
	SetAttackLevel(0);
}

/*****************************************************************/ //Handle

void SubBossPawn::HandleStateNumber()
{
	// 0 to 1
	CheckStateNumberChange0to1Condition();

	// 1 to 2
	DelayReadyToStandState();
	CheckStateNumberChange1to2Condition();

	// 2 to 3
	DelayStandUpState();
	CheckStateNumberChange2to3Condition();

	// 3 to 4
	DelayIdleState();
	CheckStateNumberChange3to4Condition();
}

void SubBossPawn::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();

	if (BRender() == true)
	{
		if (StateNumber() < 3
			&& StateNumber() != 1)
		{
			SetBMove(false);
		}
		else
		{
			if (playerPos.x > targetPos)
				Move(position);
		}

		Fall(position);

		animation->Position(position);
	}
	else
	{
		animation->Position(subBossPos.x + 50.f, subBossPos.y);
	}
}

void SubBossPawn::HandleDamage(int damage)
{
	__super::HandleDamage(damage);
}

void SubBossPawn::HandleDestruction()
{
	__super::HandleDestruction();

	if (HealthLevel() <= 0
		|| SubBossBDead() == true)
	{
		SetBRender(false);

		SetCollideLevel(0);
		SetAttackLevel(0);

		// 폭발 효과 Render
		crashEffect->SetBRender(true);

	}
}

/*****************************************************************/ //State

void SubBossPawn::CheckStateNumberChange0to1Condition()
{
	if (StateNumber() == 0)
	{
		if (BOnGround() == true && rebound == 0)
		{
			SetStateNumber(1);
		}
	}
}

void SubBossPawn::DelayReadyToStandState()
{
	if (StateNumber() == 1
		&& animation->GetClip()->CurrentFrame() == 3)
	{
		frameCycle++;
	}
}

void SubBossPawn::CheckStateNumberChange1to2Condition()
{
	if (StateNumber() == 1
		&& frameCycle == 1)
	{
		SetStateNumber(2);
		frameCycle = 0;
	}
}

void SubBossPawn::DelayStandUpState()
{
	if (StateNumber() == 2
		&& animation->GetClip()->CurrentFrame() == 12)
	{
		frameCycle++;
	}
}

void SubBossPawn::CheckStateNumberChange2to3Condition()
{
	if (StateNumber() == 2
		&& frameCycle == 1)
	{
		SetStateNumber(3);
		frameCycle = 0;
	}
}

void SubBossPawn::DelayIdleState()
{
	if (StateNumber() == 3
		&& animation->GetClip()->CurrentFrame() == 3)
	{
		frameCycle++;
	}
}

void SubBossPawn::CheckStateNumberChange3to4Condition()
{
	if (StateNumber() == 3
		&& frameCycle == 1)
	{
		SetStateNumber(4);
		frameCycle = 0;
	}
}

/*****************************************************************/ //Movement

void SubBossPawn::Move(D3DXVECTOR2& _pos)
{
	float radian = Math::ToRadian(angle);
	_pos.x -= cosf(radian) * moveSpeed * Timer->Elapsed();
	_pos.y -= sinf(radian) * moveSpeed * Timer->Elapsed();

	lastDirection = DirectionInDegree::Left;
	animation->RotationDegree(0, DirectionInDegree::Left, 0);

	SetBMove(true);
}

void SubBossPawn::Fall(D3DXVECTOR2 & _pos)
{
	velocity += gravity * Timer->Elapsed();
	_pos.y += velocity;

	if (LandCondition(_pos.y) == true)
	{
		// 반동
		if (rebound <= 0)
		{
			_pos.y = GroundPosition();
			SetBOnGround(true);
		}
		else
		{
			_pos.y += rebound;
			velocity = 0.f;

			rebound -= 30.f;
		}
	}
}

bool SubBossPawn::LandCondition(float f)
{
	if (f <= GroundPosition())
		return true;
	else
		return false;
}

/*****************************************************************/ //Effects

void SubBossPawn::CheckCrashEffectDead()
{
	if (crashEffect->BDead() == true)
	{
		SetBDead(true);
	}
}
