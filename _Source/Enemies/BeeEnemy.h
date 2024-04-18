#pragma once
#include "Enemy.h"

class BeeEnemy : public Enemy
{
public:
	BeeEnemy(D3DXVECTOR2 start, D3DXVECTOR2 target);
	virtual ~BeeEnemy();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	Sprite* GetSprite();

	/***************************************/ // Spec

public:
	virtual void SetEnemySpec();

	/***************************************/ // Handle

public:
	virtual void HandleMovement() override;
	virtual void HandleAttack() override;
	virtual void HandleDamage(int damage) override;
	virtual void HandleDestruction() override;

	/***************************************/ // Crash Effect

public:
	void CheckCrashEffectDead();

private:
	class CrashEffect* crashEffect;

private:
	float moveSpeed = 220.f;

private:
	D3DXVECTOR2 targetPosition;

private:
	int frameCycle = 0;
};