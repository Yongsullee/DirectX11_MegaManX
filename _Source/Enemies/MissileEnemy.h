#pragma once
#include "Enemies/Enemy.h"

class MissileEnemy : public Enemy
{
public:
	MissileEnemy(D3DXVECTOR2& _position);
	virtual ~MissileEnemy();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	Sprite* GetSprite();

public:
	void SetBOnGround(bool b) { bOnGround = b; }
	bool BOnGround() { return bOnGround; }

	void SetBSpawnMissile(bool b) { bSpawnMissile = b; }
	bool BSpawnMissile() { return bSpawnMissile; }

public:
	void SetGroundPosition(float i) { groundPosition = i; }
	float GroundPosition() { return groundPosition; }

	void SetAngle(float f) { angle = f; }
	float Angle() { return angle; }

	/***************************************/ // Spec

public:
	virtual void SetEnemySpec();

	/***************************************/ // Handle

public:
	virtual void HandleAttack() override;
	virtual void HandleDamage(int damage) override;
	virtual void HandleDestruction() override;
	
	/***************************************/ // State

	void HandleStateNumber();

	void CheckStateNumber0to1Condition();
	void DelayReadyToAttackState();
	void CheckStateNumber1to2Condition();
	void DelayReturnToIdleState();
	void CheckStateNumber3to1Condition();

	/***************************************/ // Effect

	void CheckCrashEffectDead();

public:
	void CheckEndOfSpawnMissileCondition();
	void SpawnMissile(D3DXVECTOR2& _position);
	

private:
	Animation* animation;

	/***************************************/ // Effect

private:
	class CrashEffect* crashEffect;

	/***************************************/ // Weapons

public:
	const int maxMissileNum = 10;
	int missileNum;
	vector<class Missile*> missiles;

private:
	enum DirectionInDegree { Right = 180, Left = 0 };
	enum AngleDirection { _Right = 1, _Left = -1 };

	int lastDirection;

private:
	bool bOnGround;
	bool bSpawnMissile;
	bool bEndOfSpawnMissile;

private:
	float groundPosition;

private:
	float angle;

private:
	int frameCycle;
};