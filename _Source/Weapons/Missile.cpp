#include "stdafx.h"
#include "Weapons/Missile.h"
#include "Effects/Effect.h"
#include "Effects/FlyingDustEffect.h"

Missile::Missile(wstring shaderFile, D3DXVECTOR2 start, float angle)
	:Weapon()
	,velocity(0.f, 0.f), speed(500.f)
{
	SetWeaponSpec();

	wstring spriteFile = Textures + L"MegaManX/Enemy/Enemies.png";

	/*****************************************************************/ // Sprite

	sprite = new Sprite(spriteFile, shaderFile, 360, 751, 360 + 16, 751 + 11);

	float radian = Math::ToRadian(angle);
	/*
		������ = 1
		x ��ǥ = sinf(radian)
		y ��ǥ = cosf(radian)
								*/
	velocity.x = cosf(radian);
	velocity.y = sinf(radian);
	velocity *= speed;

	sprite->Scale(2.5f, 2.5f);
	sprite->RotationDegree(0, 0, 0);
	sprite->Position(start);
	sprite->DrawBound(false);

	/*****************************************************************/ // Effect

	flyingDustEffect = new FlyingDustEffect(start);

}

Missile::~Missile()
{
	SAFE_DELETE(sprite)
	SAFE_DELETE(flyingDustEffect);
}

void Missile::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);
	D3DXVECTOR2 position = sprite->Position();

	SetBMove(false);
	
	if (BDead() == false
		&& BRender() == true)
	{
		position = Move();
		flyingDustEffect->SetBRender(true);
	}

	sprite->Position(position);
	sprite->Update(V, P);

	/*****************************************************************/ // Effect

	flyingDustEffect->GetWeaponPos(position);
	flyingDustEffect->Update(V, P);

}
void Missile::Render()
{

	if (BRender() == true)
	{
		__super::Render();
		sprite->Render();
	}

	/*****************************************************************/ // Effect

	flyingDustEffect->Render();

}

/*****************************************************************/ // Spec

/*
	����:
		Weapon�� Spec�� �����մϴ�.
	����:
		1. Attack Level ����
		2. Move Speed ����
*/
void Missile::SetWeaponSpec()
{
	SetAttackLevel(20);
	SetMoveSpeed(200.f);
}

/*****************************************************************/ // Movement

D3DXVECTOR2 Missile::Move()
{
	__super::Move();
	D3DXVECTOR2 position = sprite->Position();

	position += velocity * Timer->Elapsed();

	return position;
}

void Missile::AdjustPosition(D3DXVECTOR2 _pos)
{
	D3DXVECTOR2 missilePos = sprite->Position();

	missilePos = _pos;

	sprite->Position(missilePos);
}

/*****************************************************************/ // Attack

void Missile::HandleAttack()
{
	__super::HandleAttack();
}

/*****************************************************************/ // Destroy

/*

	����:
		�ִ� �ݰ��� �Ѿ Missile�� ȭ�鿡�� �����մϴ�.
	����:
		Player X��ǥ�� �������� 1.5f * Width�� �Ѿ��,
		
		1. SetBDead(true)
		2. SetBRender(false)
		3. effect->SetBRender(false);

*/
void Missile::HandleDestruction()
{
	__super::HandleDestruction();	//	SetBAttack(true)
	D3DXVECTOR2 position = sprite->Position();

	if (position.x < enemyPos.x - Width * 1.5f
		|| BAttack() == true)
	{
		SetBDead(true);
		SetBRender(false);
		flyingDustEffect->SetBDead(true);
		flyingDustEffect->SetBRender(false);
	}

}


