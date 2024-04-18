#pragma once
#include "BackgroundObject.h"

class Car : public BackgroundObject
{
public:
	Car();
	virtual ~Car();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Render();

public:
	void Position(float x, float y);
	void Position(D3DXVECTOR2& vec);
	D3DXVECTOR2 Position() { return animation->Position(); }

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2& vec);
	D3DXVECTOR2 Scale() { return animation->Scale(); }

public:
	float MoveSpeed() { return moveSpeed; }
	void SetMoveSpeed(float f) { moveSpeed = f; }

public:
	void SetBMove(bool b) { bMove = b; }
	bool BMove() { return bMove; }

	void SetBDead(bool b) { bDead= b; }
	bool BDead() { return bDead; }

protected:
	Animation* animation;

private:
	float moveSpeed;

private:
	bool bMove;
	bool bDead;
};