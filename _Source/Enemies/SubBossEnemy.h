#pragma once
#include "Enemy.h"

class SubBossEnemy : public Enemy
{
public:
	SubBossEnemy(D3DXVECTOR2 _pos, float _targetPos);
	virtual ~SubBossEnemy();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	Sprite* GetSprite();

	/***************************************/ // Spec

public:
	virtual void SetEnemySpec();
	
	/***************************************/ // Handle

public:
	void HandleStateNumber();
	virtual void HandleMovement() override;
	virtual void HandleAttack() override;
	virtual void HandleDamage(int damage) override;
	virtual void HandleDestruction() override;

	/***************************************/ // State

	void CheckStateNumberChange0to1Condition();
	void CheckStateNumberChang1to2Conditon();

	/***************************************/ // Physics

	void Move(D3DXVECTOR2& _pos);
	void Fall(D3DXVECTOR2& _pos);
	bool LandCondition(float f);

	/***************************************/ // Pawns

	void HandlePawnsRender(D3DXVECTOR2 _pos);

	/***************************************/ // Effect Handle

public:
	void HandleCrashEffectsRender();
	bool CheckCrashEffectDead(class CrashEffect& _crashEffect);

	void HandleDustEffectsRender();
	bool CheckDustEffectDead(class DustEffect& _dustEffect);

	/***************************************/ // Getter & Setter

	void SetGroundPosition(float f) { groundPosition = f; }
	float GroundPosition() { return groundPosition; }

	void SetStartMove(bool b) { startMove = b; }
	bool StartMove() { return startMove; }

	void SetBUnderUnderPlayer(bool b) { bUnderPlayer = b; }
	bool BUnderPlayer() { return bUnderPlayer; }

	void SetBCollideWithPartiableBridge(bool b) { bCollideWithPartiableBridge = b; }
	bool BCollideWithPartiableBridge() { return bCollideWithPartiableBridge; }

	void SetCollideNum(int i) { collideNum = i; }
	int CollideNum() { return collideNum; }

	/***************************************/ // Effects

private:
	float targetPos;

public:
	const int subBossNum = 5;
	vector <class SubBossPawn*> subBossPawns;

private:

	const float CRASH_OFFSET_MIN = -50.f;
	const float CRASH_OFFSET_MAX = 50.f;

	const float DUST_OFFSET_MIN = -120.f;
	const float DUST_OFFSET_MAX = 120.f;

	const int crashEffectsNum = 10;
	vector <class CrashEffect*> crashEffects;

	const int dustEffectsNum = 5;
	vector <class DustEffect*> dustEffects;

private:
	float groundPosition = 0.f;

private:
	bool startMove = false;

private:
	float moveSpeed = 30.f;
	float velocity = 0.f;
	float gravity = -10.5f;
	float rebound = 50.f;

	const float offsetY = 20.f;

private:
	bool bUnderPlayer = false;
	bool bCollideWithPartiableBridge = false;
	bool bAdjustable = true;

private:
	int collideNum = 0;
};