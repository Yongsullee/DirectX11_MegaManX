#pragma once

class Weapon
{
public:
	Weapon();
	virtual ~Weapon();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Render();

	virtual void SetWeaponSpec() = 0;

public:
	Sprite* GetSprite() { return animation->GetSprite(); }

public:
	void GetEnemyPos(D3DXVECTOR2& _position) { enemyPos = _position; }

public:
	virtual D3DXVECTOR2 Move();

public:
	virtual void HandleAttack();
	virtual void HandleDestruction();

public:

	void SetBMove(bool b) { bMove = b; }
	bool BMove() { return bMove; }

	void SetBAttack(bool b) { bAttack = b; }
	bool BAttack() { return bAttack; }

	void SetBDead(bool b) { bDead = b; }
	bool BDead() { return bDead; }

	void SetBRender(bool b) { bRender = b; }
	bool BRender() { return bRender; }

public:

	void SetStateNumber(int i) { stateNumber = i; }
	int StateNumber() { return stateNumber; }

	void SetMoveSpeed(float f) { moveSpeed = f; }
	float MoveSpeed() { return moveSpeed; }

	void SetAttackLevel(int i) { attackLevel = i; }
	int AttackLevel() { return attackLevel; }

protected:
	Animation* animation;
	int stateNumber;

	D3DXVECTOR2 enemyPos;

private:
	float moveSpeed;
	int attackLevel;

private:
	bool bMove;
	bool bAttack;
	bool bDead;
	bool bRender;

};