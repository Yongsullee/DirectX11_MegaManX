#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet()
	:clipNumber(0)
	, velocity(0, 0), speed(800.0f)
	, bAttack(false)
	, attackLevel(0), enemyHealthLevel(0)
	, playerPos(0.f, 0.f)
{}

Bullet::~Bullet()
{}

void Bullet::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{}

void Bullet::Render()
{}

D3DXVECTOR2 Bullet::Move()
{
	D3DXVECTOR2 position = animation->Position();

	position += velocity * Timer->Elapsed();

	return position;
}

void Bullet::HandleDestruction()
{
	SetBAttack(true);
}

void Bullet::DelayDeadState()
{}

