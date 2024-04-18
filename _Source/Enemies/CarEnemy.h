#pragma once
#include "Enemies/Enemy.h"

class CarEnemy : public Enemy
{
public:
	CarEnemy(D3DXVECTOR2 position, float _targetPos, float _leftLimit, float _rightLimit);
	virtual ~CarEnemy();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	Sprite* GetSprite();

	/*****************************************************************/ //Getter & Setter

public:
	void SetBOnGround(bool b) { bOnGround = b; }
	bool BOnGround() { return bOnGround; }

public:
	void SetGroundPosition(float i) { groundPosition = i; }
	float GroundPosition() { return groundPosition; }

	void SetAngle(float f) { angle = f; }
	float Angle() { return angle; }

	/*****************************************************************/ //Spec

public:
	virtual void SetEnemySpec();

	/*****************************************************************/ //Handle

public:
	virtual void HandleMovement() override;
	virtual void HandleAttack() override;
	virtual void HandleDamage(int damage) override;
	virtual void HandleDestruction() override;

	/*****************************************************************/ //State

public:
	void HandleStateNumber();

	void DelayIdleState();
	void CheckStateNumber0to1Condition();

	void DelayTurnState();
	void CheckStateNumber2to0Condition();

	/*****************************************************************/ //Movement

public:
	D3DXVECTOR2 MoveLeft(D3DXVECTOR2& _pos);
	D3DXVECTOR2 MoveRight(D3DXVECTOR2& _pos);

	void TurnRight();
	void TurnLeft();

	void Boost();

	/*****************************************************************/ //Effect

	void CheckCrashEffectDead();

private:
	Animation* animation;
	class CrashEffect* crashEffect;

private:
	enum DirectionInDegree { Right = 180, Left = 0 };
	enum AngleDirection { _Right = 1, _Left = -1 };

	int lastDirection = 0;

private:
	bool bOnGround = false;
	float groundPosition = -200.f;
	float angle = 0;

private:
	int moveSpeed = 250;
	float velocity = 0.f;
	float gravity = -10.f;
	float friction = -50.f;

private:
	float targetPos;

	float leftLimit;
	float rightLimit;

private:
	int frameCycle = 0;

};