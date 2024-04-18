#include "stdafx.h"
#include "Enemies/Enemy.h"
#include "Objects/Player.h"
#include "Effects/CrashEffect.h"

Enemy::Enemy()
	:stateNumber(0)
	, playerPos(0.f, 0.f)
	, bMove(false), bAttack(false), bHit(false), bDead(false), bRender(true)
	, healthLevel(100), collideLevel(0), attackLevel(0)
{}

Enemy::~Enemy()
{}

void Enemy::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{}

void Enemy::Render()
{}

void Enemy::GetPlayerPos(Player & _player)
{
	playerPos = _player.GetSprite()->Position();
}

/*************************************************************************/ // Movement

void Enemy::HandleMovement()
{}

/*************************************************************************/ // Attack

void Enemy::HandleAttack()
{
	SetBAttack(true);
}

/*************************************************************************/ // Damage

/*
	����:
		�ǰ� ���� ���, ���ڷ� ���� Damage ��ŭ Health Level�� �����մϴ�.
	
*/
void Enemy::HandleDamage(int damage)
{	
	int presentHealthLevel = HealthLevel();

	presentHealthLevel -= damage;

	if (presentHealthLevel < 0)
	{
		presentHealthLevel = 0;
	}

	SetHealthLevel(presentHealthLevel);
	SetBHit(true);
}

/*************************************************************************/ // Destruction

void Enemy::HandleDestruction()
{}
