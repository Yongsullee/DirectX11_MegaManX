#include "stdafx.h"
#include "Weapons/Weapon.h"

Weapon::Weapon()
	:stateNumber(0)
	, enemyPos(0.f, 0.f)
	, moveSpeed(0.f), attackLevel(0)
	, bMove(false), bAttack(false), bDead(false), bRender(false)
{}

Weapon::~Weapon()
{}

void Weapon::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{}

void Weapon::Render()
{}

D3DXVECTOR2 Weapon::Move()
{
	return D3DXVECTOR2(0.f, 0.f);
}

void Weapon::HandleAttack()
{}

void Weapon::HandleDestruction()
{
	SetBAttack(true);
}
