#pragma once
#include "Viewer/IFollowing.h"

class Player : public IFollowing
{
public:
	Player(D3DXVECTOR2 position, D3DXVECTOR2 scale);
	~Player();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void Focus(D3DXVECTOR2* position, D3DXVECTOR2* size);

	Sprite* GetSprite();

	/*****************************************************************/ // Handle

public:
	void HandleStateNumber();
	void HandleMovement();
	void HandleAttack();
	void HandleJump();
	void HandleCollisionDamage(int damage);
	void HandleAttackDamage(int damage);
	void HandleElectricDamage(int damage);	// Boss의 공격을 받는 경우
	void HandleGroggyState();

	/*****************************************************************/ // Effects

	void HandleChargingEffectRender();
	void HandleSparkEffectRender();

	/********************************************/ // Getter & Setter

public:

	void SetBMove(bool b) { bMove = b; }
	bool BMove() { return bMove; }

	void SetBOnGround(bool b) { bOnGround = b; }
	bool BOnGround() { return bOnGround; }

	void SetBOnWall(bool b) { bOnWall = b; }
	bool BOnWall() { return bOnWall; }

	void SetBWallJump (bool b) { bWallJump = b; }
	bool BWallJump() { return bWallJump; }

	void SetBFalling(bool b) { bFalling = b; }
	bool BFalling() { return bFalling; }

	void SetBAttack(bool b) { bAttack = b; }
	bool BAttack() { return bAttack; }

	void SetBCharging(bool b) { bCharging = b; }
	bool BCharging() { return bCharging; }

	void SetBPhysicalHit(bool b) { bPhysicalHit = b; }
	bool BPhysicalHit() { return bPhysicalHit; }

	void SetBElectricalHit(bool b) { bElectricalHit = b; }
	bool BElectricalHit() { return bElectricalHit; }

	void SetBHit(bool b) { bHit = b; }
	bool BHit() { return bHit; }

	void SetBInvincible(bool b) { bInvincible = b; }
	bool BInvincible() { return bInvincible; }

	void SetBGroggy(bool b) { bGroggy = b; }
	bool BGroggy() { return bGroggy; }

	void SetBDead(bool b) { bDead = b; }
	bool BDead() { return bDead; }

	void SetBCollideWithHole(bool b) { bCollideWithHole = b; }
	bool BCollideWithHole() { return bCollideWithHole; }

public:

	void SetMoveSpeed(float f) { moveSpeed = f; }
	float MoveSpeed() { return moveSpeed; }

	void SetGroundPosition(float f) { groundPosition = f; }
	float GroundPosition() { return groundPosition; }

	void SetAngle(float f) { angle = f; }
	float Angle() { return angle; }

	void SetLimitFlag(int i) { limitFlag = i; }
	int LimitFlag() { return limitFlag; }

	void SetLimitLeftMove(int i) { limitLeftMove = i; }
	int LimitLeftMove() { return limitLeftMove; }

	void SetLimitRightMove(int i) { limitRightMove = i; }
	int LimitRightMove() { return limitRightMove; }

	void SetPlayerHealthLevel(int i) { playerHealthLevel = i; }
	int PlayerHealthLevel() { return playerHealthLevel; }

	void SetLastDirection(int i) { lastDirection = i; }
	int LastDirection() { return lastDirection; }

	void SetMeetSubBoss1(bool b) { meetSubBoss1 = b; }
	bool MeetSubBoss1() { return meetSubBoss1; }

	void SetMeetSubBoss2(bool b) { meetSubBoss2 = b; }
	bool MeetSubBoss2() { return meetSubBoss2; }

	void SetSubBoss1Dead(bool b) { subBoss1Dead = b; }
	bool SubBoss1Dead() { return subBoss1Dead; }

	void SetSubBoss2Dead(bool b) { subBoss2Dead = b; }
	bool SubBoss2Dead() { return subBoss2Dead; }

	void SetMeetBoss(bool b) { meetBoss = b; }
	bool MeetBoss() { return meetBoss; }

	void SetBossDead(bool b) { bossDead = b; }
	bool BossDead() { return bossDead; }

	/*********************************************/ // Bullet Getter & Setter
	
	void SetBSpawnLevel1Bullet(bool b) { bSpawnLevel1Bullet = b; }
	bool BSpawnLevel1Bullet() { return bSpawnLevel1Bullet; }

	void SetBSpawnLevel2Bullet(bool b) { bSpawnLevel2Bullet = b; }
	bool BSpawnLevel2Bullet() { return bSpawnLevel2Bullet; }

	void SetBSpawnLevel3Bullet(bool b) { bSpawnLevel3Bullet = b; }
	bool BSpawnLevel3Bullet() { return bSpawnLevel3Bullet; }

	/*****************************************************************/ // State

	void CheckStateChangeAttackToNotAttackState();

	/*****************************************************************/ // Physics

public:
	void Move(D3DXVECTOR2& _pos);
	void NormalFall(D3DXVECTOR2& _pos);
	void WallFall(D3DXVECTOR2& _pos);
	void WallJump(D3DXVECTOR2& _pos);
	void Land(D3DXVECTOR2& _pos);
	void StartJump();
	void EndJump();

	/*****************************************************************/ // Movement

public:
	void LimitMovement();
	void CheckLimitMovementCondition();
	void StartWallFriciton();
	void KnockBack();

	/*****************************************************************/ // Damage

public:

	void DamageTaken(int& damage, const std::string& dmaageType);
	void CheckInvincibleCondition();

	/*****************************************************************/ // Camera Setting

	bool CheckMeetSubBoss1();
	bool CheckMeetSubBoss2();

	bool CheckMeetBoss();

private:
	int stateNumber;

private:
	bool bMove = false;
	bool bOnGround = false;
	bool bOnWall = false;
	bool bWallJump = false;
	bool bFalling = false;
	bool bAttack = false;
	bool bCharging = false;
	bool bPhysicalHit = false;
	bool bElectricalHit = false;
	bool bHit = false;
	bool bInvincible = false;
	bool bGroggy = false;
	bool bDead = false;
	bool bCollideWithHole = false;
	bool bReadyToAttack = false;

	/*****************************************************************/ // Values

private:
	float moveSpeed = 300.f;
	float groundPosition = 0.f;

	/*****************************************************************/ // Action

private:
	float velocity = 0.0f;
	float gravity = -23.f;
	float friction = 1.2f;
	float tension = 0.f;
	float angle = 0.f;
	const float knockBackDist = 40.f;
	

	/*****************************************************************/ // Angles
	
private:
	enum DirectionInDegree { Right = 0, Left = 180 };
	enum AngleDirection { _Right = 1, _Left = -1 };

	int lastDirection = 0;

	int limitFlag = 0;
	int limitLeftMove = 1;
	int limitRightMove = 1;

	/*****************************************************************/ // Damage

private:
	
	const int maxInvincibleTime = 30;
	int invincibleTime = 0;

	/*****************************************************************/ // Effect*

private:
	class ChargingEffect* chargingEffect;
	class SparkEffect* sparkEffect;

	/*****************************************************************/ // Ineterface

private:
	class HealthLevel* healthLevelInterface;
	int playerHealthLevel = 100;

	/*****************************************************************/ // Sub Boss Info

private:
	bool meetSubBoss1 = false;
	bool subBoss1Dead = false;

	bool meetSubBoss2 = false;
	bool subBoss2Dead = false;

	bool meetBoss = false;
	bool bossDead = false;

	/*****************************************************************/ // etc

private:
	Animation* animation;
	D3DXVECTOR2 focusOffset;

	int frameCycle = 0;

	/*****************************************************************/ // Bullet

public:
	int chargingTime = 0;

private:
	bool bSpawnLevel1Bullet = false;
	bool bSpawnLevel2Bullet = false;
	bool bSpawnLevel3Bullet = false;

	/*****************************************************************/ // Camera

private:
	float cameraFocus = 0.f;
	float decreaseInFocus = -200.f;
	const float cameraFocusMin = -530.f;

	const float CAMERA_SET_TO_SUBBOSS1 = -3109.f;
	const float CAMERA_SET_TO_SUBBOSS2 = -1197.f;
	const float CAMERA_SET_TO_BOSS = 11500.f;

};