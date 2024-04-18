#pragma once
#include "BackgroundObject.h"

class Bridge : public BackgroundObject
{
public:
	Bridge();
	virtual ~Bridge();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Render();

public:
	void SetStateNumber(int i) { stateNumber = i; }
	int StateNumber() { return stateNumber; }

	void SetBPartiable(bool b) { bPartiable = b; }
	bool BPartiable() { return bPartiable; }

	void SetBBroken(bool b) { bBroken = b; }
	bool BBroken() { return bBroken; }

	void SetBFall(bool b) { bFall = b; }
	bool BFall() { return bFall; }

public:
	void Position(float x, float y);
	void Position(D3DXVECTOR2& vec);
	D3DXVECTOR2 Position() { return animation->Position(); }

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2& vec);
	D3DXVECTOR2 Scale() { return animation->Scale(); }

	Sprite* GetSprite() { return animation->GetSprite(); }

protected:
	Animation* animation;

private:
	int stateNumber;

	bool bPartiable;
	bool bBroken;
	bool bFall;
};