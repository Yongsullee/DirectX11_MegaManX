#pragma once
#include "Enemies/Enemy.h"

class SubBossPawn : public Enemy
{
public:
	SubBossPawn(D3DXVECTOR2 position);
	virtual ~SubBossPawn();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	Sprite* GetSprite();

	void GetSubBossEnemyPos(class SubBossEnemy& subBoss);

	/***************************************/ // Spec

public:
	virtual void SetEnemySpec();

	/***************************************/ // Handle

public:
	void HandleStateNumber();
	virtual void HandleMovement() override;
	virtual void HandleDamage(int damage) override;
	virtual void HandleDestruction() override;

	/***************************************/ // State Number

public:
	// 0 to 1
	void CheckStateNumberChange0to1Condition();
	// 1 to 2
	void DelayReadyToStandState();
	void CheckStateNumberChange1to2Condition();
	// 2 to 3
	void DelayStandUpState();
	void CheckStateNumberChange2to3Condition();
	// 3 to 4
	void DelayIdleState();	
	void CheckStateNumberChange3to4Condition();

	/***************************************/ // Physics

public:
	void Move(D3DXVECTOR2& _pos);
	void Fall(D3DXVECTOR2& _pos);
	bool LandCondition(float f);

public:
	void CheckCrashEffectDead();

	/***************************************/ // Getter & Setter

public:
	void SetBOnGround(bool b) { bOnGround = b; }
	bool BOnGround() { return bOnGround; }

public:
	void SetGroundPosition(float i) { groundPosition = i; }
	float GroundPosition() { return groundPosition; }

	void SetAngle(float f) { angle = f; }
	float Angle() { return angle; }

	void SetSubBossBDead(bool b) { subBossBDead = b; }
	bool SubBossBDead() { return subBossBDead; }

	void SetTargetPos(float f) { targetPos = f; }
	float TargetPos() { return targetPos; }

	/***************************************/ // Effects

private:
	Animation* animation;
	class CrashEffect* crashEffect;

	int frameCycle = 0;

private:
	enum DirectionInDegree { Right = 180, Left = 0 };
	enum AngleDirection { _Right = 1, _Left = -1 };

	int lastDirection = 0;

private:
	bool bOnGround = false;
	float groundPosition = 0.f;

private:
	int moveSpeed = 120.f;
	float angle = 0.f;
	float velocity = 0.f;
	float gravity = -10.5f;
	float rebound = 30.f;

private:
	D3DXVECTOR2 subBossPos = D3DXVECTOR2(0.f, 0.f);
	bool subBossBDead = false;

	float targetPos = 0.f;
};