#pragma once
#include "Enemy.h"

class BossEnemy : public Enemy
{
public:
	BossEnemy(D3DXVECTOR2 _pos, float _targetPos);
	virtual ~BossEnemy();

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
	void HandlePhase(D3DXVECTOR2& _pos);

	/***************************************/ // State
	// * to Idle
	void CheckStateNumberAllto0Condition();
	// Idle to Attack
	void CheckStateNumber0to1Condition();
	// Idle to Jump
	void CheckStateNumber0to2Condition();
	// * to Punch
	void CheckStateNumberAllto3Condition();

	/***************************************/ // Phase

	/*

		목적:
			보스 객체의 행동패턴을 정의합니다.
		설명:
			1. Phase_1 : 펀치 및 끝에서 끝으로 이동
			2. Phase_2 : Player의 지근 거리에서 연속 펀치
			3. Phase_3 : Player의 지근 거리에서 Bullet 발사 및 백점프

	*/

	void CheckPhaseStart();
	void Phase_1(D3DXVECTOR2& _pos);
	void Phase_2(D3DXVECTOR2& _pos);
	void Phase_3(D3DXVECTOR2& _pos);


	/***************************************/ // Physics

	void Move(D3DXVECTOR2& _pos);
	void StartJump();
	void EndJump();

	void Fall(D3DXVECTOR2& _pos);
	bool LandCondition(float f);

	/***************************************/ // Bullet Handle

	void AngleBetweenPlayer(class BossEnemyBullet& _bullet, D3DXVECTOR2& _pos);
	void SpawnBullets();

	/***************************************/ // Effect Handle
	
	void HandleCrashEffectsRender();
	bool CheckCrashEffectDead(class CrashEffect& _crashEffect);

	void HandleDustEffectsRender();
	bool CheckDustEffectDead(class DustEffect& _dustEffect);

	/***************************************/ // Getter & Setter

	void SetLastDirection(int i) { lastDirection = i; }
	int LastDirection() { return lastDirection; }

	void SetRightMove(bool b) { rightMove = b; }
	bool RightMove() { return rightMove; }

	void SetLeftMove(bool b) { leftMove = b; }
	bool LeftMove() { return leftMove; }
	
	void SetGroundPosition(float f) { groundPosition = f; }
	float GroundPosition() { return groundPosition; }

	void SetBOnGround(bool b) { bOnGround = b; }
	bool BOnGround() { return bOnGround; }

	void SetAngle(float f) { angle = f; }
	float Angle() { return angle; }

	void SetMoveSpeed(float f) { moveSpeed = f; }
	float MoveSpeed() { return moveSpeed; }

	void SetPunch(bool b) { punch = b; }
	bool Punch() { return punch; }

	void SetBSpawnBullet(bool b) { bSpawnBullet = b; }
	bool BSpawnBullet() { return bSpawnBullet; }

	void SetBEndOfSpawnBullet(bool b) { bEndOfSpawnBullet = b; }
	bool BEndOfSpawnBullet() { return bEndOfSpawnBullet; }

	void SetPhaseStart(bool b) { phaseStart = b; }
	bool PhaseStart() { return phaseStart; }

private:
	float originalPos;
	float targetPos;

private:
	enum DirectionInDegree { Right = 180, Left = 0 };
	enum AngleDirection { _Right = 1, _Left = -1 };

	int lastDirection = 0;

	bool rightMove = false;
	bool leftMove = false;

private:
	float groundPosition = -200.f;

private:
	float angle = 0.f;
	const float DEFAULT_MOVE_SPEED = 30.f;
	float moveSpeed = 0.f;
	float velocity = 0.f;
	float backJump = 0.f;
	float gravity = -20.5f;
	
private:
	bool bOnGround = true;

private:
	bool punch = false;
	bool bSpawnBullet = false;
	bool bEndOfSpawnBullet = false;

	/***********************************/ //Bullet

public:
	vector<class BossEnemyBullet*> bullets;
	const int bulletOffsetX = 33.f;
	const int bulletOffsetY = 82.f;

	/***********************************/ //Behavior

private:
	const int PHASE_START_DELAY = 100;
	int phaseStartDelay = 0;
	bool phaseStart = false;
	int phase = 1;

	// Phase Delay
	const int STOP_DELAY = 20;
	int delayCnt = 0;

	// Phase 1
	const int PHASE_1_MAX = 30;
	int phase1Num = 0;
	bool phase1End = false;
	bool phase1_step1 = true;
	bool phase1_step2 = false;
	bool phase1_step3 = false;
	bool phase1_step4 = false;
	bool phase1_step5 = false;
	bool phase1_step6 = false;

	// Phase 2
	const int PHASE_2_MAX = 30;			// 최대 Cycle 횟수
	const int PHASE_2_MAX_PUNCH = 5;	// 최대 펀치 per Cycle
	int phase2Num = 0;
	int phase2PunchNum = 0;
	bool phase2End = false;
	bool phase2_step1 = true;
	bool phase2_step2 = false;
	bool phase2_step3 = false;

	// Phase 3
	const int PHASE_3_MAX = 30;
	const int PHASE_3_MAX_BULLET = 5;
	bool phase3Jump = false;
	int phase3Num = 0;
	int phase3BulletNum = 0;
	bool phase3End = false;
	bool phase3_step1 = true;
	bool phase3_step2 = false;
	bool phase3_step3 = false;
	bool phase3_step4 = false;
	bool phase3_step5 = false;


	/***********************************/ //Effects

private:
	const float CRASH_OFFSET_MIN = -50.f;
	const float CRASH_OFFSET_MAX = +50.f;

	const float DUST_OFFSET_X = 80.f;
	const float DUST_OFFSET_Y = -80.f;

	const int crashEffectNum = 10;
	vector <class CrashEffect*> crashEffects;

	const int dustEffectNum = 3;
	vector <class DustEffect*> dustEffects;

};