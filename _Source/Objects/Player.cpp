#include "stdafx.h"
#include "Player.h"
#include "Objects/Bullet.h"
#include "Objects/Level1Bullet.h"
#include "Objects/Level2Bullet.h"
#include "Objects/Level3Bullet.h"
#include "Effects/Effect.h"
#include "Effects/ChargingEffect.h"
#include "Effects/SparkEffect.h"
#include "Interfaces/Interface.h"
#include "Interfaces/HealthLevel.h"

Player::Player(D3DXVECTOR2 position, D3DXVECTOR2 scale)
	: stateNumber(0), focusOffset(0,0)
{

	animation = new Animation;

	wstring spriteFile = Textures + L"MegaManX/Player/Player.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	/*****************************************************************/ // Sprite

	Clip* clip;
	//Idle, stateNumber = 0
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 323, 17, 323 + 30, 17 + 34), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 357, 17, 357 + 30, 17 + 34), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 391, 17, 391 + 30, 17 + 34), 0.3f);
		animation->AddClip(clip);
	}
	//Run, stateNumber = 1
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 106, 107, 106 + 30, 107 + 34), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 137, 107, 137 + 20, 107 + 34), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 158, 106, 158 + 23, 106 + 35), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 181, 107, 181 + 32, 107 + 34), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 213, 108, 213 + 34, 108 + 33), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 247, 108, 247 + 26, 108 + 33), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 276, 107, 276 + 22, 107 + 34), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 298, 106, 298 + 25, 106 + 35), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 326, 107, 326 + 30, 107 + 34), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 357, 108, 357 + 34, 108 + 33), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 391, 108, 391 + 29, 108 + 33), 0.03f);
		animation->AddClip(clip);
	}
	//Jump, stateNumber = 2
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 202, 63, 202 + 24, 63 + 37), 0.08f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 231, 59, 231 + 15, 59 + 41), 0.08f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 252, 54, 252 + 19, 54 + 46), 0.08f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 274, 59, 274 + 23, 59 + 41), 0.08f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 299, 58, 299 + 27, 58 + 42), 2.5f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 356, 68, 356 + 30, 68 + 32), 0.15f);
		animation->AddClip(clip);
	}
	//OnWall,stateNumber = 3
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 144, 527, 144 + 28, 527 + 42), 0.15f);
		animation->AddClip(clip);
	}
	//NormalSpawn, stateNumber = 4
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 133, 66, 133 + 30, 66 + 34), 0.08f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 168, 66, 168 + 29, 66 + 34), 0.08f);
		animation->AddClip(clip);
	}
	//RunningSpawn, stateNumber = 5
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 61, 192, 61 + 35, 192 + 34), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 97, 193, 97 + 29 + 5, 193 + 34), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 126, 192, 126 + 32, 192 + 35), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 158, 193, 158 + 35, 193 + 34), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 194, 194, 194 + 38, 194 + 33), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 233, 194, 233 + 34, 194 + 33), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 294, 193, 294 + 31, 193 + 34), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 298, 192, 298 + 33, 192 + 35), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 331, 193, 331 + 35, 193 + 34), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 367, 194, 367 + 37, 194 + 33), 0.03f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 404, 194, 404 + 35, 194 + 33), 0.03f);
		animation->AddClip(clip);
	}
	//JumpSpawn, stateNumber = 6
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 181, 147, 181 + 31, 147 + 42), 2.50f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 213, 151, 213 + 30, 151 + 38), 0.18f);
		animation->AddClip(clip);
	}
	// WallSpawn, stateNumber = 7
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 272, 527, 272 + 32, 527 + 43), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 310, 526, 310 + 32, 526 + 42), 2.5f);
		animation->AddClip(clip);
	}
	//WallJump, stateNumber = 8
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 272, 527, 272 + 32, 527 + 43), 0.08f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 310, 526, 310 + 32, 526 + 42), 0.08f);
		animation->AddClip(clip);
	}
	//bHit, stateNumber = 9
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 65, 702, 65 + 25, 702 + 36), 0.5f);
		animation->AddClip(clip);
	}
	// Groggy, stateNumber = 10
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 174, 856, 174 + 25, 856 + 27), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 202, 856, 202 + 25, 856 + 27), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 230, 856, 230 + 25, 856 + 27), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 258, 856, 258 + 25, 856 + 27), 0.3f);
		animation->AddClip(clip);
	}
	// Electrical Hit, stateNumber = 11
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 146, 632, 146+32, 632+48), 0.3f);
		animation->AddClip(clip);

	}


	/*****************************************************************/

	animation->Scale(scale);
	animation->Position(position);
	animation->DrawBound(false);
	animation->Play(stateNumber);

	/*****************************************************************/ // Effect 

	chargingEffect = new ChargingEffect(position);
	sparkEffect = new SparkEffect(position);

}

Player::~Player()
{
	SAFE_DELETE(animation);
	SAFE_DELETE(chargingEffect);
	SAFE_DELETE(sparkEffect);
}

void Player::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	D3DXVECTOR2 position = animation->Position();

	/*****************************************************************/ // Handle

	HandleStateNumber();
	HandleJump();
	HandleAttack();
	HandleMovement();
	HandleGroggyState();

	/*****************************************************************/ // Movement Condition
	
	CheckLimitMovementCondition();
	CheckInvincibleCondition();	

	/*****************************************************************/ //Animation Update

	animation->Play(stateNumber);
	animation->Update(V, P);

	/*****************************************************************/ //Effect

	HandleChargingEffectRender();
	HandleSparkEffectRender();

	/*****************************************************************/ //Effects Update
	
	chargingEffect->GetPlayerPos(*this);
	chargingEffect->Update(V, P);

	sparkEffect->GetPlayerPos(*this);
	sparkEffect->SetAngle(lastDirection);
	sparkEffect->Update(V, P);

}

void Player::Render()
{
	animation->Render();

	/*****************************************************************/ //Effects
		
	chargingEffect->Render();
	sparkEffect->Render();

}

/*****************************************************************/ // Camera

void Player::Focus(D3DXVECTOR2 * position, D3DXVECTOR2 * size)
{
	D3DXVECTOR2 playerPos = animation->Position();

	/*
		1. Partiable Bridge 추락 시 Decrease
		2. 올라올때, position.y - focusOffset
		3. -100.f 이상일때, offset.y로
	*/
	if (CheckMeetSubBoss1() == true)
	{
		(*position).x = CAMERA_SET_TO_SUBBOSS1;
	}
	else if (CheckMeetSubBoss2() == true)
	{
		(*position).x = CAMERA_SET_TO_SUBBOSS2;
	}
	else if (CheckMeetBoss() == true)
	{
		(*position).x = CAMERA_SET_TO_BOSS;
	}
	else
	{
		(*position).x = playerPos.x - focusOffset.x;
	}


	if (playerPos.y > -200.f)
	{
		cameraFocus = focusOffset.y;
	}
	if (playerPos.y <= -200.f)
	{
		if (MeetSubBoss1() == false
			&& MeetSubBoss2() == false
			&& MeetBoss() == false)
		{
			cameraFocus = focusOffset.y;
		}
		else if (MeetSubBoss1() == true
			|| MeetSubBoss2() == true
			|| MeetBoss() == true)
		{
			cameraFocus = playerPos.y - focusOffset.y;
			if (playerPos.y < -500.f)
			{
				cameraFocus = -520.f;
			}
		}
	}

	(*position).y = cameraFocus;

	(*size) = D3DXVECTOR2(1, 1);
}

/*****************************************************************/ // Etc

Sprite * Player::GetSprite()
{
	return animation->GetSprite();
}

/*****************************************************************/ // Handle

void Player::HandleStateNumber()
{
	CheckStateChangeAttackToNotAttackState();

	if (BGroggy() == true)
	{
		stateNumber = 10;
	}
	else
	{
		if (BInvincible() == true)
		{
			if (BElectricalHit() == true)
			{
				stateNumber = 11;
			}
			else
			{
				stateNumber = 9;
			}
		}
		else
		{
			if (BOnGround() == true)
			{
				stateNumber = 0;
				if (BAttack() == true
					&& BMove() == false)
				{
					stateNumber = 4;
				}
				if (BMove() == true)
				{
					if (BAttack() == true)
					{
						stateNumber = 5;
					}
					else
					{
						stateNumber = 1;
					}
				}
			}
			else
			{
				//Jump
				stateNumber = 2;
				//JumpSpawn
				if (BAttack() == true)
				{
					stateNumber = 6;
				}
				//bOnWall
				if (BOnWall() == true)
				{
					stateNumber = 3;
					if (BAttack() == true)
					{
						//WallSpawn
						stateNumber = 7;
					}
				}
			}
		}
	}
}

void Player::HandleMovement()
{
	D3DXVECTOR2 _pos = animation->Position();

	if(BInvincible() == false)
		Move(_pos);

	if (BOnWall() == true)
	{
		WallFall(_pos);
	}
	else
	{

		if (BWallJump() == true)
		{
			WallJump(_pos);
		}
		if (BOnGround() == false || BFalling() == true)
		{
			NormalFall(_pos);
		}
		if (_pos.y < GroundPosition())
		{
			Land(_pos);
		}
	}

	animation->Position(_pos);
}

void Player::HandleAttack()
{
	if (BInvincible() == false)
	{
		if (Key->Press('A'))	// Charging Effect
		{
			chargingTime++;
			SetBCharging(true);
		}
		else
		{
			// Level 1 
			if (chargingTime >= 1
				&& chargingTime < 10)
			{
				SetBAttack(true);

				SetBSpawnLevel1Bullet(true);

				chargingTime = 0;
				SetBCharging(false);
			}
			//Level 2
			else if (chargingTime >= 10
				&& chargingTime < 30)
			{
				SetBAttack(true);

				SetBSpawnLevel2Bullet(true);

				chargingTime = 0;
				SetBCharging(false);
			}
			//Level 3
			else if (chargingTime >= 30)
			{
				SetBAttack(true);

				SetBSpawnLevel3Bullet(true);

				chargingTime = 0;
				SetBCharging(false);
			}
		}
	}
}

void Player::HandleJump()
{
	if (BInvincible() == false)
	{
		if (Key->Down(VK_SPACE))
		{
			StartJump();
		}
		else if (Key->Up(VK_SPACE))
		{
			EndJump();
		}
	}
}

void Player::HandleCollisionDamage(int damage)
{
	int presentHealthLevel = PlayerHealthLevel();

	if (BInvincible() == false)
	{
		presentHealthLevel -= damage;

		if (presentHealthLevel < 0)
			presentHealthLevel = 0;

		SetPlayerHealthLevel(presentHealthLevel);

		SetBPhysicalHit(true);
		SetBInvincible(true);

		KnockBack();
	}
}

void Player::HandleAttackDamage(int damage)
{
	int presentHealthLevel = PlayerHealthLevel();

	if (BInvincible() == false)
	{
		presentHealthLevel -= damage;

		if (presentHealthLevel < 0)
			presentHealthLevel = 0;

		SetPlayerHealthLevel(presentHealthLevel);

		SetBHit(true);
		SetBInvincible(true);

		KnockBack();
	}
}

void Player::HandleElectricDamage(int damage)
{
	int presentHealthLevel = PlayerHealthLevel();

	if (BInvincible() == false)
	{
		presentHealthLevel -= damage;
		if (presentHealthLevel < 0)
			presentHealthLevel = 0;

		SetPlayerHealthLevel(presentHealthLevel);

		SetBElectricalHit(true);
		SetBInvincible(true);

		KnockBack();
	}
}

void Player::HandleGroggyState()
{
	int presentHealthLevel = PlayerHealthLevel();
	
	if (presentHealthLevel <= 0
		&& MeetBoss() == true)
	{
		SetBGroggy(true);
		SetBMove(false);
	}
}

/*****************************************************************/ // State

void Player::CheckStateChangeAttackToNotAttackState()
{
	// 4 to 0, IdleSpawn to Idle
	if (stateNumber == 4
		&& BAttack() == true)
	{
		if (frameCycle > 20)
		{
			SetBAttack(false);
			stateNumber = 0;

			frameCycle = 0;
		}
		frameCycle++;
	}

	// 5 to 1, RunningSpawn to Running
	if (stateNumber == 5
		|| stateNumber == 6)
	{
		if (BCharging() == false
			&& frameCycle > 10)
		{
			SetBAttack(false);
			bReadyToAttack = false;

			frameCycle = 0;
		}
		if (BCharging() == false)
			frameCycle++;
		else
			frameCycle = 0;
	}
}

/*****************************************************************/ // Physics

void Player::Move(D3DXVECTOR2 & _pos)
{
	if (BGroggy() == false)
	{
		if (Key->Press(VK_LEFT))
		{
			bMove = true;
			float radian = Math::ToRadian(angle);
			_pos.x -= cosf(radian) * moveSpeed * limitLeftMove * Timer->Elapsed();
			_pos.y -= sinf(radian) * moveSpeed * limitLeftMove * Timer->Elapsed();

			lastDirection = DirectionInDegree::Left;
			animation->RotationDegree(0, DirectionInDegree::Left, 0);
		}
		else if (Key->Press(VK_RIGHT))
		{
			bMove = true;
			float radian = Math::ToRadian(angle);
			_pos.x += cosf(radian) * moveSpeed * limitRightMove * Timer->Elapsed();
			_pos.y += sinf(radian) * moveSpeed * limitRightMove * Timer->Elapsed();

			lastDirection = DirectionInDegree::Right;
			animation->RotationDegree(0, DirectionInDegree::Right, 0);
		}
		else
		{
			bMove = false;
		}
	}
}

void Player::NormalFall(D3DXVECTOR2& _pos)
{

	velocity += gravity * Timer->Elapsed();
	_pos.y += velocity;

}

void Player::WallFall(D3DXVECTOR2 & _pos)
{
	velocity += (gravity + friction) * Timer->Elapsed();
	_pos.y += velocity;
}

void Player::WallJump(D3DXVECTOR2 & _pos)
{
	_pos.x += tension * -1.f * LimitFlag() * Timer->Elapsed();
	tension++;

	if (tension >= 160.f)
	{
		tension = 0;

		SetLimitFlag(0);
		SetBWallJump(false);
	}
}

void Player::Land(D3DXVECTOR2 & _pos)
{
	if (_pos.y < GroundPosition())
	{
		_pos.y = GroundPosition();
		velocity = 0.f;

		SetBOnGround(true);
	}
}

void Player::StartJump()
{
	D3DXVECTOR2 position = GetSprite()->Position();

		if (BOnGround() == true)
		{
			velocity = 16.0f;

			SetBOnGround(false);
		}
		if (BOnWall() == true)	// WallJump Effect
		{
			velocity = 15.0f;
			tension = 150.f;

			SetBWallJump(true);
			SetBOnWall(false);
		}
}

void Player::EndJump()
{
	if (velocity <= 15.8f)
	{

	}
	else	
	{
		velocity = 15.8f;
	}
}

void Player::StartWallFriciton()
{
	if (BOnWall() == false)
	{
		velocity = 0.f;
		SetBOnWall(true);
	}
}

void Player::LimitMovement()
{
	switch (LimitFlag())
	{
	case 1:
		SetLimitRightMove(0);
		break;
	case -1:
		SetLimitLeftMove(0);
		break;
	}
}

void Player::CheckLimitMovementCondition()
{
	if (LimitFlag() == 0)
	{
		SetLimitRightMove(1);
		SetLimitLeftMove(1);
	}
	else
	{
		LimitMovement();
	}
}

void Player::KnockBack()
{
	D3DXVECTOR2 position = animation->Position();

	if (lastDirection == DirectionInDegree::Right)
	{
		SetLimitFlag(1);
		position.x -= knockBackDist;
	}
	else if (lastDirection == DirectionInDegree::Left)
	{
		SetLimitFlag(-1);
		position.x += knockBackDist;
	}

	animation->Position(position);
}

/*****************************************************************/ // Damage

/*

	목적:
		Player가 받은 Damage에 대한 처리
	설명:
		Damage와 Damage Type을 통해 HandleCollisionDamage 혹은 HandleAttackDamage로 처리할지 결정
		피격 당해 knockBack 당하면, maxInvincibleTime 만큼 무적 상태가 됩니다.

*/

void Player::CheckInvincibleCondition()
{
	if (BInvincible() == true)
	{
		invincibleTime++;
		if (invincibleTime >= maxInvincibleTime)
		{
			SetBInvincible(false);
			invincibleTime = 0;
			SetLimitFlag(0);
		}
	}
}

void Player::DamageTaken(int& damage, const std::string& damageType)
{
	if (damageType == "Collision")
	{
		HandleCollisionDamage(damage);
	}
	else if (damageType == "Attack")
	{
		HandleAttackDamage(damage);
	}
	else if (damageType == "Electric")
	{
		HandleElectricDamage(damage);
	}
}

/*****************************************************************/ // Camera

bool Player::CheckMeetSubBoss1()
{

	if (MeetSubBoss1() == true && SubBoss1Dead() == false)
		return true;
	else
		return false;
}

bool Player::CheckMeetSubBoss2()
{
	if (MeetSubBoss2() == true && SubBoss2Dead() == false)
		return true;
	else
		return false;
}

bool Player::CheckMeetBoss()
{
	if (MeetBoss() == true && BossDead() == false)
		return true;
	else
		return false;
}

/*****************************************************************/ // Effect

void Player::HandleChargingEffectRender()
{
	if (BCharging() == true
		&& chargingTime > 20)
	{
		// Charging Effect
		chargingEffect->SetBRender(true);
	}
	else
	{
		chargingEffect->SetBRender(false);
	}
}

void Player::HandleSparkEffectRender()
{

	if (BWallJump() == true)
	{
		// Spark Effect
		sparkEffect->SetBRender(true);
	}
	else
	{
		sparkEffect->SetBRender(false);
	}
}