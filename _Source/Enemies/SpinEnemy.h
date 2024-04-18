#pragma once
#include "Enemies/Enemy.h"

class SpinEnemy : public Enemy
{
public:
	SpinEnemy(D3DXVECTOR2 position, float _targetPos);
	virtual ~SpinEnemy();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	Sprite* GetSprite();

public:
	void SetBOnGround(bool b) { bOnGround = b; }
	bool BOnGround() { return bOnGround; }

public:
	void SetGroundPosition(float i) { groundPosition = i; }
	float GroundPosition() { return groundPosition; }

	void SetAngle(float f) { angle = f; }
	float Angle() { return angle; }

public:
	virtual void SetEnemySpec();

public:
	virtual void HandleMovement() override;
	virtual void HandleDamage(int damage) override;
	virtual void HandleDestruction() override;

	void CheckCrashEffectDead();

public:
	void Move();

private:
	Animation* animation;
	class CrashEffect* crashEffect;

private:
	enum DirectionInDegree { Right = 180, Left = 0 };
	enum AngleDirection { _Right = 1, _Left = -1 };

	int lastDirection = 0;

private:
	bool bOnGround = true;
	float groundPosition = 0.f;

private:
	int moveSpeed = 150;
	float angle = 0.f;

private:
	float targetPos = 0.f;
};