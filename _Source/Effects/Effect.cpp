#include "stdafx.h"
#include "Objects/Player.h"
#include "Effect.h"

Effect::Effect()
	:playerPos(0.f, 0.f), enemyPos(0.f, 0.f), weaponPos(0.f, 0.f)
{}

Effect::~Effect()
{}

void Effect::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{}

void Effect::Render()
{}

void Effect::GetPlayerPos(Player & _player)
{
	playerPos = _player.GetSprite()->Position();
}

void Effect::GetEnemyPos(D3DXVECTOR2& _pos)
{
	enemyPos = _pos;
}

void Effect::GetWeaponPos(D3DXVECTOR2& _position)
{
	weaponPos = _position;
}

