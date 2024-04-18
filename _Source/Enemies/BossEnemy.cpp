#include "stdafx.h"
#include "BossEnemy.h"
#include "Effects/Effect.h"
#include "Effects/CrashEffect.h"
#include "Effects/DustEffect.h"
#include "Objects/Bullet.h"
#include "Objects/BossEnemyBullet.h"

BossEnemy::BossEnemy(D3DXVECTOR2 _pos, float _targetPos)
	:Enemy()
	, originalPos(_pos.x)
	, targetPos(_targetPos)
{
	SetMoveSpeed(DEFAULT_MOVE_SPEED);
	SetEnemySpec();

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Enemy/BossEnemy.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	Clip* clip;

	// Idle, 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 16, 106, 16 + 55, 106 + 70), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 72, 105, 72 + 55, 105 + 71), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 128, 104, 128 + 55, 104 + 72), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 185, 103, 185 + 55, 103 + 73), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 242, 101, 242 + 55, 101 + 75), 0.03f);
		animation->AddClip(clip);
	}
	// Move, 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 61, 11, 61 + 55, 11 + 73), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 118, 10, 118 + 55, 10 + 74), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 175, 8, 175 + 55, 8 + 76), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 232, 7, 232 + 55, 7 + 77), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 290, 8, 290 + 55, 8 + 76), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 348, 10, 348 + 55, 10 + 74), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 405, 8, 405 + 55, 8 + 76), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 463, 7, 463 + 55, 7 + 77), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 520, 8, 520 + 55, 8 + 76), 0.03f);
		animation->AddClip(clip);
	}
	// Jump, 2
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 518, 92, 518 + 55, 92 + 84), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 575, 92, 575 + 61, 92 + 84), 1.5f);
		animation->AddClip(clip);
	}
	// Long Attack, 3
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 299, 107, 299 + 55, 107 + 69), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 356, 107, 356 + 75, 107 + 69), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 433, 107, 433 + 82, 107 + 69), 2.5f);
		animation->AddClip(clip);
	}
	// Short Attack, 4
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 299, 107, 299 + 55, 107 + 69), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 356, 107, 356 + 75, 107 + 69), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 433, 107, 433 + 82, 107 + 69), 1.0f);
		animation->AddClip(clip);
	}
	// Grab, 5
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 50, 207, 50 + 47, 207 + 73), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 99, 207, 99 + 75, 207 + 73), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 176, 207, 176 + 82, 207 + 73), 0.03f);
		animation->AddClip(clip);
	}

	animation->Scale(3.0f, 3.0f);
	animation->RotationDegree(0, 180.f, 0);
	animation->Position(_pos);
	animation->DrawBound(false);
	animation->Play(stateNumber);

	/***************************************/ // Phase Num

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dis1(20, PHASE_1_MAX);
	phase1Num = dis1(gen);

	std::uniform_int_distribution<int> dis2(20, PHASE_2_MAX);
	phase2Num = dis2(gen);
	phase2PunchNum = PHASE_2_MAX_PUNCH;

	std::uniform_int_distribution<int> dis3(20, PHASE_3_MAX);
	phase3Num = dis3(gen);
	phase3BulletNum = PHASE_3_MAX_BULLET;
	
	/***************************************/ // Bullet

	BossEnemyBullet* bullet;

	for (int i = 0; i < 50; i++)
	{
		bullet = new BossEnemyBullet(_pos, 0);
		bullets.push_back(bullet);
	}

	/***************************************/ // Crash Effect & Dust Effect

	CrashEffect* crash;
	DustEffect* dust;

	for (int i = 0; i < crashEffectNum; i++)
	{
		crash = new CrashEffect(_pos);
		crash->SetBAdjustable(true);
		crashEffects.push_back(crash);
	}

	for (int i = 0; i < dustEffectNum; i++)
	{
		dust = new DustEffect(_pos);
		dust->SetBAdjustable(true);
		dustEffects.push_back(dust);
	}

}

BossEnemy::~BossEnemy()
{
	SAFE_DELETE(animation);

	/***************************************/ // Bullet

	for (BossEnemyBullet* bullet : bullets)
		SAFE_DELETE(bullet);

	/***************************************/ // Crash Effect & Dust Effect

	for (CrashEffect* effect : crashEffects)
		SAFE_DELETE(effect);
	for (DustEffect* effect : dustEffects)
		SAFE_DELETE(effect);
	
}

void BossEnemy::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);
	__super::HandleDestruction();

	SetBMove(false);

	HandleStateNumber();
	HandleMovement();
	HandleAttack();
	HandleDestruction();

	animation->Play(stateNumber);
	animation->Update(V, P);

	/*****************************************************************/	// Bulelts

	for (BossEnemyBullet* bullet : bullets)
	{
		bullet->GetEnemyPos(animation->Position());
		bullet->GetPlayerPos(playerPos);

		if (lastDirection == DirectionInDegree::Right)
		{
			bullet->SetOffsetX(bulletOffsetX * -1.f);
		}
		else
		{
			bullet->SetOffsetX(bulletOffsetX);
		}

		bullet->SetOffsetY(bulletOffsetY);
		bullet->Update(V, P);
	}

	/*****************************************************************/	// Effects

	HandleCrashEffectsRender();
	HandleDustEffectsRender();

	if (BRender() == true)
	{
		// Crash Effects Udpate
		for (CrashEffect* effect : crashEffects)
		{
			effect->GetEnemyPos(animation->Position());

			effect->SetOffsetPositionX(Math::Random(CRASH_OFFSET_MIN, CRASH_OFFSET_MAX));
			effect->SetOffsetPositionY(Math::Random(CRASH_OFFSET_MIN, CRASH_OFFSET_MAX));

			effect->Update(V, P);
		}

		// Dust Effects Udpate
		for (DustEffect* effect : dustEffects)
		{
			effect->GetEnemyPos(animation->Position());

			if (lastDirection == DirectionInDegree::Right)
				effect->SetOffsetPositionX(DUST_OFFSET_X);
			else if (lastDirection == DirectionInDegree::Left)
				effect->SetOffsetPositionX(-1.f * DUST_OFFSET_X);

			effect->SetOffsetPositionY(DUST_OFFSET_Y);

			effect->Update(V, P);
		}
	}

}

void BossEnemy::Render()
{

	__super::Render();
	if (BRender() == true)
		animation->Render();

	/******************************************/ //Effects

	for (BossEnemyBullet* bullet : bullets)
		bullet->Render();

	/******************************************/ //Effects

	for (CrashEffect* effect : crashEffects)
		effect->Render();
	for (DustEffect* effect : dustEffects)
		effect->Render();

}

Sprite * BossEnemy::GetSprite()
{
	return animation->GetSprite();
}

/*****************************************************************/	// Spec

void BossEnemy::SetEnemySpec()
{
	SetHealthLevel(1000);
	SetCollideLevel(20);
	SetAttackLevel(20);
}

/*****************************************************************/	// Handle

void BossEnemy::HandleStateNumber()
{
	// * to Idle
	CheckStateNumberAllto0Condition();
	// Idle to Move
	CheckStateNumber0to1Condition();
	// Idle to Jump
	CheckStateNumber0to2Condition();
	// * to Punch
	CheckStateNumberAllto3Condition();

}

void BossEnemy::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();

	CheckPhaseStart();

	if(PhaseStart() == true)
		HandlePhase(position);

	/*****************************************************************/	// Physics

	Move(position);

	Fall(position);

	if (LandCondition(position.y) == true)
	{
		backJump = 0.f;
		position.y = GroundPosition();
		SetBOnGround(true);
	}

	animation->Position(position);
}

void BossEnemy::HandleAttack()
{
	__super::HandleAttack();

	if (BSpawnBullet() == true)
	{
		SpawnBullets();
	}
}

void BossEnemy::HandleDamage(int damage)
{
	__super::HandleDamage(damage);
}

void BossEnemy::HandleDestruction()
{
	__super::HandleDestruction();

	if (HealthLevel() <= 0)
	{
		SetBMove(false);
		SetBDead(true);

		crashEffects.at(0)->SetBRender(true);

		SetCollideLevel(0);
		SetAttackLevel(0);
	}
}

void BossEnemy::HandlePhase(D3DXVECTOR2& _pos)
{

	if (phase == 1)
	{
		Phase_1(_pos);
		if (phase1End == true)
		{
			phase1Num--;

			phase1End = false;

			phase++;
		}
	}
	if (phase == 2)
	{
		Phase_2(_pos);
		if (phase2End == true)
		{
			phase2Num--;

			phase2End = false;

			phase++;
		}
	}
	if (phase == 3)
	{
		Phase_3(_pos);
		if (phase3End == true)
		{
			phase3Num--;

			phase3End = false;

			phase = 1;
		}
	}

	if (phase1Num <= 0)
	{
		phase = 2;
		if (phase2Num <= 0)
			phase = 3;
	}
	if (phase2Num <= 0)
	{
		phase = 3;
		if (phase3Num <= 0)
			phase = 1;
	}
	if (phase3Num <= 0)
	{
		phase = 1;
		if (phase1Num <= 0)
			phase = 2;
	}

	if (BDead() == false
		&& phase1Num <= 0
		&& phase2Num <= 0
		&& phase3Num <= 0)
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_int_distribution<int> dis1(20, PHASE_1_MAX);
		phase1Num = dis1(gen);

		std::uniform_int_distribution<int> dis2(20, PHASE_2_MAX);
		phase2Num = dis2(gen);

		std::uniform_int_distribution<int> dis3(20, PHASE_3_MAX);
		phase3Num = dis3(gen);
	}
}

/*****************************************************************/ // State

void BossEnemy::CheckStateNumberAllto0Condition()
{
	if (BMove() == false
		&& Punch() == false
		&& BOnGround() == true)
	{
		SetStateNumber(0);
	}
}

void BossEnemy::CheckStateNumber0to1Condition()
{
	if (BOnGround() == true
		&& BMove() == true
		&& Punch() == false)
	{
		SetStateNumber(1);
	}
}

void BossEnemy::CheckStateNumber0to2Condition()
{
	if (BOnGround() == false
		&& Punch() == false)
	{
		SetStateNumber(2);
	}
}

void BossEnemy::CheckStateNumberAllto3Condition()
{
	if (Punch() == true)
		SetStateNumber(3);
}

/*****************************************************************/	// Phase

/*

	1. Phase 1	: 왼쪽 끝과 오른쪽 끝으로 긴 펀치를 날립니다.
	2. Phase 2	: Player의 지근거리에 접근하여 짧은 펀치를 연속적으로 날립니다.
	3. Phase 3	: Player의 지근거리에 접근하여 Bullet을 발사하며, BackJump를 합니다.

*/

void BossEnemy::CheckPhaseStart()
{
	if (PhaseStart() == false
		&& playerPos.x >= targetPos)
	{
		phaseStartDelay++;
		if (phaseStartDelay >= PHASE_START_DELAY)
		{
			SetPhaseStart(true);

			phaseStartDelay = 0;
		}

	}
}

void BossEnemy::Phase_1(D3DXVECTOR2& _pos)
{

	// Step 1 : 왼쪽으로 이동하며 긴 펀치
	if (phase1_step1 == true)
	{
		SetMoveSpeed(500.f);

		SetRightMove(false);
		SetLeftMove(true);

		SetPunch(true);

		if (_pos.x <= targetPos - 100.f)
		{
			SetRightMove(false);
			SetLeftMove(false);

			phase1_step1 = false;
			phase1_step2 = true;
		}
	}
	// Step 2 : 사정 거리 밖에서 사정 거리 안으로 복귀
	if (phase1_step2 == true)
	{
		SetMoveSpeed(150.f);

		SetRightMove(true);
		SetLeftMove(false);

		SetPunch(false);

		if (_pos.x >= targetPos)
		{
			phase1_step2 = false;
			phase1_step3 = true;
		}
	}
	// Step 3 : 잠시 멈추고 대기
	if (phase1_step3 == true)
	{
		SetLeftMove(false);
		SetRightMove(false);

		delayCnt++;

		if (delayCnt >= STOP_DELAY)
		{
			phase1_step3 = false;
			phase1_step4 = true;

			delayCnt = 0;
		}
	}
	// Step 4 : 오른쪽으로 긴 펀치
	if (phase1_step4 == true)
	{
		SetMoveSpeed(500.f);

		SetRightMove(true);
		SetLeftMove(false);

		SetPunch(true);

		if (_pos.x >= originalPos + 200.f)
		{
			SetRightMove(false);
			SetLeftMove(false);

			phase1_step4 = false;
			phase1_step5 = true;
		}
	}
	// Step 5 : Orignal Position으로 복귀합니다.
	if (phase1_step5 == true)
	{
		SetMoveSpeed(150.f);

		SetRightMove(false);
		SetLeftMove(true);

		SetPunch(false);

		if (_pos.x <= originalPos)
		{
			phase1_step5 = false;
			phase1_step6 = true;
		}
	}
	// Step 6 : 멈춰서 대기합니다.
	if (phase1_step6 == true)
	{
		SetMoveSpeed(0.f);

		SetRightMove(false);
		SetLeftMove(false);

		SetPunch(false);

		animation->RotationDegree(0, DirectionInDegree::Right, 0);

		delayCnt++;
		if (delayCnt >= STOP_DELAY)
		{
			phase1_step6 = false;

			delayCnt = 0;
			phase1_step1 = true;
			phase1End = true;
		}
	}
}

void BossEnemy::Phase_2(D3DXVECTOR2& _pos)
{
	D3DXVECTOR2 _playerPos = playerPos;

	bool leftDirection = false;
	bool rightDirection = false;

	// 방향 설정
	if (playerPos.x < _pos.x)
	{
		rightDirection = false;
		leftDirection = true;
	}
	else
	{
		rightDirection = true;
		leftDirection = false;
	}

	// Step 1 : Player의 지근거리에 접근
	if (phase2_step1 == true)
	{
		SetMoveSpeed(500.f);

		SetRightMove(rightDirection);
		SetLeftMove(leftDirection);

		SetPunch(false);

		if (fabs(_pos.x - _playerPos.x) <= 150.f)
		{
			phase2_step1 = false;
			phase2_step2 = true;
		}
	}
	// Step 2 : Short Punch를 Player에게 날립니다.
	if (phase2_step2 == true)
	{
		SetMoveSpeed(0.f);

		SetRightMove(false);
		SetLeftMove(false);

		SetPunch(true);

		delayCnt++;
		if (delayCnt >= STOP_DELAY)
		{
			phase2_step2 = false;
			phase2_step3 = true;

			delayCnt = 0;
		}
	}
	// Step 3 : Short Punch를 마치고, Player를 향해 이동할지, Short Punch를 다시할지 결정합니다.
	if (phase2_step3 == true)
	{
		SetRightMove(false);
		SetLeftMove(false);

		SetPunch(false);

		delayCnt++;
		if (delayCnt >= STOP_DELAY + 15)
		{
			phase2Num--;
			
			phase2_step3 = false;
			if (fabs(_pos.x - _playerPos.x) > 160.f)
			{
				phase2_step1 = true;
			}
			else
			{
				phase2_step2 = true;
			}

			// Cycle 종료 조건
			if (--phase2PunchNum <= 0)
			{
				phase2PunchNum = PHASE_2_MAX_PUNCH;

				phase2_step1 = true;

				phase2End = true;
			}
			delayCnt = 0;
		}
	}
}

void BossEnemy::Phase_3(D3DXVECTOR2& _pos)
{
	D3DXVECTOR2 _playerPos = playerPos;

	bool leftDirection = false;
	bool rightDirection = false;

	// 방향 설정
	if (playerPos.x < _pos.x)
	{
		leftDirection = true;
		rightDirection = false;
	}
	else
	{
		leftDirection = false;
		rightDirection = true;
	}

	// Step 1 : Player의 지근거리에 접근
	if (phase3_step1 == true)
	{
		SetMoveSpeed(500.f);

		SetRightMove(rightDirection);
		SetLeftMove(leftDirection);

		SetPunch(false);

		if (fabs(_pos.x - _playerPos.x) <= 150.f)
		{
			phase3_step1 = false;
			phase3_step2 = true;
		}
	}
	// 2. Step 2 : Bullet을 발사합니다
	if (phase3_step2 == true)
	{
		SetMoveSpeed(0.f);

		SetPunch(false);

		if (BEndOfSpawnBullet() == false)
			SetBSpawnBullet(true);

		phase3_step2 = false;
		phase3_step3 = true;
	}
	// 3. Step 3 : BackJump 합니다.
	if (phase3_step3 == true)
	{
		if (phase3Jump == false)
		{
			StartJump();
			phase3Jump = true;
		}

		SetPunch(false);

		if (phase3Jump == true && BOnGround() == true)
		{
			phase3Jump = false;
			
			SetBEndOfSpawnBullet(false);

			phase3_step3 = false;
			if (fabs(_pos.x - _playerPos.x) > 160.f)
			{
				phase3_step1 = true;
			}
			else
			{
				phase3_step2 = true;
			}
		
			// Cycle 종료 조건
			if (--phase3BulletNum <= 0)
			{
				phase3BulletNum = PHASE_3_MAX_BULLET;

				phase3_step1 = true;

				phase3End = true;
			}
		}
	}
}

/*****************************************************************/	// Physics

void BossEnemy::Move(D3DXVECTOR2 & _pos)
{
	if (RightMove() == true)
	{
		SetAngle(180);
		lastDirection = DirectionInDegree::Left;
	}
	else if (LeftMove() == true)
	{
		SetAngle(0);
		lastDirection = DirectionInDegree::Right;
	}

	if (RightMove() == true
		|| LeftMove() == true)
	{
		float radian = Math::ToRadian(angle);
		_pos.x -= cosf(radian) * moveSpeed * Timer->Elapsed();
		_pos.y -= sinf(radian) * moveSpeed * Timer->Elapsed();

		SetBMove(true);
		dustEffects.at(0)->SetBRender(true);

		animation->RotationDegree(0, lastDirection, 0);
	}

}

void BossEnemy::StartJump()
{
	if (BOnGround() == true)
	{
		velocity = 15.0f;
		backJump = 100.0f * Timer->Elapsed();

		SetBOnGround(false);

		// Jump 상태
		SetStateNumber(2);
	}
}

void BossEnemy::EndJump()
{
	if (velocity >= 14.8f)
	{
		velocity = 14.8f;
	}
}

void BossEnemy::Fall(D3DXVECTOR2 & _pos)
{
	EndJump();

	// X 좌표
	if (lastDirection == 180)
	{
		_pos.x += backJump;
	}
	else if (lastDirection == 0)
	{
		_pos.x -= backJump;
	}

	// Y 좌표
	velocity += gravity * Timer->Elapsed();
	_pos.y += velocity;

}

bool BossEnemy::LandCondition(float f)
{
	if (f <= GroundPosition())
		return true;
	else
		return false;

}

/*****************************************************************/	// Bullet

/*

	목적:
		Bullet 발사 각도를 보스와 플레이어간의 거리를통해 계산합니다.

*/

void BossEnemy::AngleBetweenPlayer(BossEnemyBullet& _bullet, D3DXVECTOR2 & _pos)
{

	float ab = sqrtf(pow(_pos.x - playerPos.x, 2) + pow(_pos.y - playerPos.y, 2));
	float ac = fabs(_pos.x - playerPos.x);

	float  tmpAngle = Math::ToDegree(acosf(ac / ab));

	// 1. Enemy 기준 Player가 왼쪽에 위치할 때
	if (playerPos.x < _pos.x)
	{
		tmpAngle = 180.f + tmpAngle;
	}
	// 2. Enemy 기준 Player가 오른쪽에 위치할 때
	if (playerPos.x > _pos.x)
	{
		tmpAngle =  -1.f * tmpAngle;
	}

	_bullet.SetAngle(tmpAngle);
}

void BossEnemy::SpawnBullets()
{
	for (auto it = bullets.begin(); it != bullets.end(); it++)
	{
		if ((*it)->BRender() == false)
		{
			AngleBetweenPlayer(*(*it), animation->Position());
			(*it)->SetBRender(true);
			break;
		}
		else
		{
			continue;
		}
	}

	SetBSpawnBullet(false);
	SetBEndOfSpawnBullet(true);
}

/*****************************************************************/	// Effects

void BossEnemy::HandleCrashEffectsRender()
{
	for (int i = 0; i < crashEffects.size() - 1; i++)
	{
		if (CheckCrashEffectDead(*crashEffects.at(i)) == true)
			if (crashEffects.at(i + 1)->BRender() == false)
				crashEffects.at(i + 1)->SetBRender(true);
	}
}

bool BossEnemy::CheckCrashEffectDead(CrashEffect & _crashEffect)
{
	if (_crashEffect.BDead() == true)
		return true;
	else
		return false;
}

void BossEnemy::HandleDustEffectsRender()
{
	for (int i = 0; i < dustEffects.size() - 1; i++)
	{
		if (CheckDustEffectDead(*dustEffects.at(i)) == true)
			if (dustEffects.at(i + 1)->BRender() == false)
				dustEffects.at(i + 1)->SetBRender(true);
	}
}

bool BossEnemy::CheckDustEffectDead(DustEffect & _dustEffect)
{
	if (_dustEffect.BDead() == true)
		return true;
	else
		return false;
}
