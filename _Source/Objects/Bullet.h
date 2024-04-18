#pragma once
#include "Objects/Player.h"

class Bullet
{
public:
	Bullet();
	~Bullet();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	virtual void Render();

	Sprite* GetSprite() { return animation->GetSprite(); }
	
	virtual D3DXVECTOR2 Move();

public:

	void SetBAttack(bool b) { bAttack = b; }
	bool BAttack() { return bAttack; }

	void SetBDead(bool b) { bDead = b; }
	bool BDead() { return bDead; }

public:
	void SetPlayerPos(Player& _player) { playerPos = _player.GetSprite()->Position(); }
	D3DXVECTOR2 GetPlayerPos() { return playerPos; }

public:
	 
	void SetAttackLevel(int i) { attackLevel = i; }
	int AttackLevel() { return attackLevel; }

	void SetSpeed(float f) { speed = f; }
	float Speed() { return speed; }

	void SetAngle(int i) { angle = i; }
	int Angle() { return angle; }

	void SetEnemyHealthLevel(int i) { enemyHealthLevel = i; }
	int EnemyHealthLevel() { return enemyHealthLevel; }

public:

	virtual void HandleDestruction();
	virtual void DelayDeadState();

protected:
	Animation* animation;
	int clipNumber;

protected:
	D3DXVECTOR2 velocity;
	float speed;

	int angle = 0;

private:
	bool bAttack;
	bool bDead;

private:
	int attackLevel;
	int enemyHealthLevel;

protected:
	D3DXVECTOR2 playerPos;
};