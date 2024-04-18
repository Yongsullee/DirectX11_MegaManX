#pragma once
#include "Enemy.h"

class SuicideBombEnemy : public Enemy
{
public:
	SuicideBombEnemy(D3DXVECTOR2 start, D3DXVECTOR2 target);
	virtual ~SuicideBombEnemy();

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

	/***************************************/ // Movement

private:
	void AngleBetweenPlayer(D3DXVECTOR2& _position);
	void SlowDownSpeed();

	/***************************************/ // Crash Effect

public:
	void CheckCrashEffectDead();

	/***************************************/ // Getter & Setter

public:

	void SetMoveSpeed(float f) { moveSpeed = f; }
	float MoveSpeed() { return moveSpeed; }

	void SetAngle(int i) { angle = i; }
	int Angle() { return angle; }

private:
	class CrashEffect* crashEffect;

private:
	enum DirectionInDegree { Right = 180, Left = 0 };
	enum AngleDirection { _Right = 1, _Left = -1 };

	int lastDirection = 0;

private:
	float moveSpeed = 300.f;
	float angle = 0.f;
	float tmpAngle = 0.f;

private:
	D3DXVECTOR2 targetPosition;

private:
	int frameCycle = 0;
};