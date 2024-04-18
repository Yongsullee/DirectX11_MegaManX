#pragma once
#include "Enemies/Enemy.h"

class LandMineEnemy : public Enemy
{
public:
	LandMineEnemy(D3DXVECTOR2 start, D3DXVECTOR2 target);
	virtual ~LandMineEnemy();


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

	/***************************************/ // StateNumber 

public:
	void HandleStateNumber();

	void CheckStateNumberChange0to1Condition();
	void DelayIdleState();

	void CheckStateNumberChange1to2Condition();
	void DelaySpawnState();

	void CheckStateNumberChange2to0Condition();

	/***************************************/ // Movement

public:
	void Move();
	void StopMove();

	/***************************************/ // Crash Effect

public:
	void CheckCrashEffectDead();

	/***************************************/ // Getter & Setter

public:
	void SetBReadyToSpawn(bool b) { bReadyToSpawn = b; }
	bool BReadyToSPawn() { return bReadyToSpawn; }
	
	void SetBEndOfSpawn(bool b) { bEndOfSpawn = b; }
	bool BEndOfSpawn() { return bEndOfSpawn; }

private:
	class CrashEffect* crashEffect;

public:
	class LandMine* landMine;

private:
	bool bReadyToSpawn = false;
	bool bEndOfSpawn = false;

private:
	float moveSpeed;

private:
	D3DXVECTOR2 targetPosition;

private:
	int frameCycle = 0;

};