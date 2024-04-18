#pragma once

class Enemy
{
public:
	Enemy();
	virtual ~Enemy();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Render();

public:
	void GetPlayerPos(class Player& _player);

public:
	void SetBMove(bool b) { bMove = b; }
	bool BMove() { return bMove; }

	void SetBAttack(bool b) { bAttack = b; }
	bool BAttack() { return bAttack; }

	void SetBHit(bool b) { bHit = b; }
	bool BHit() { return bHit; }

	void SetBDead(bool b) { bDead = b; }
	bool BDead() { return bDead; }

	void SetBRender(bool b) { bRender = b; }
	bool BRender() { return bRender; }

public:
	void SetStateNumber(int i) { stateNumber = i; }
	int StateNumber() { return stateNumber; }

	void SetHealthLevel(int i) { healthLevel = i; }
	int HealthLevel() { return healthLevel; }

	void SetCollideLevel(int i) { collideLevel = i; }
	int CollideLevel() { return collideLevel; }

	void SetAttackLevel(int i) { attackLevel = i; }
	int AttackLevel() { return attackLevel; }

public:
	virtual void SetEnemySpec() = 0;

public:
	virtual void HandleMovement();
	virtual void HandleAttack();
	virtual void HandleDamage(int damage);
	virtual void HandleDestruction();

protected:
	Animation* animation;
	int stateNumber;

protected:
	D3DXVECTOR2 playerPos;

private:
	bool bMove;
	bool bAttack;
	bool bHit;
	bool bDead;
	bool bRender;

private:
	int healthLevel;
	int collideLevel;
	int attackLevel;

};