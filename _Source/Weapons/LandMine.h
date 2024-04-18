#pragma once
#include "Weapon.h"

class LandMine : public Weapon
{
public:
	LandMine(D3DXVECTOR2 start);
	virtual ~LandMine();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	virtual void SetWeaponSpec() override;

	Sprite* GetSprite() { return animation->GetSprite(); }

	/*****************************************************************/ // Movement

public:
	void AdjustPosition();

	void HandleMovement();
	D3DXVECTOR2 Move(D3DXVECTOR2 _position);

	/*****************************************************************/ // Handle

public:
	virtual void HandleAttack() override;
	void HandleDamage(int damage);
	virtual void HandleDestruction() override;

	/*****************************************************************/ // State

public:
	void HandleStateNumber();

	void DelayReadyToMoveState();
	void CheckStateNumberChange0to1Condition();

	/*****************************************************************/ // Effect

public:
	void CheckCrashEffectDead();

	/*****************************************************************/ // Getter & Setter

	void SetHealthLevel(int i) { healthLevel = i; }
	int HealthLevel() { return healthLevel; }

	void SetCollideLevel(int i) { collideLevel = i; }
	int CollideLevel() { return collideLevel; }

	void SetGroundPosition(int i) { groundPosition = i; }
	int GroundPosition() { return groundPosition; }

	void SetBOnGround(bool b) { bOnGround = b; }
	bool BOnGround() { return bOnGround; }

private:
	Animation* animation;
	int frameCycle = 0;

public:
	class CrashEffect* crashEffect;

	vector<string>DepthOfHoles;
	vector<class MakingHoleEffect*> holeEffects;

private:
	bool bCollideWithLine = false;

public:
	int healthLevel = 0;
	int collideLevel = 0;

private:
	float velocity = 0.f;
	float gravity = -20.0f;
	float speed = 800.f;

private:
	float groundPosition = 0.f;

	bool bOnGround = false;

};