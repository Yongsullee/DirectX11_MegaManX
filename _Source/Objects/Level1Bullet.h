#pragma once
#include "Objects/Bullet.h"

class Level1Bullet : public Bullet
{
public:
	Level1Bullet(wstring shaderFile, D3DXVECTOR2 start, float angle);
	~Level1Bullet();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;
	virtual void Render() override;

	void HandleStateNumber();
	virtual void HandleDestruction() override;

	virtual void DelayDeadState() override;

	D3DXVECTOR2 AdjustPosition();


public:
	void SetStartMove(bool b) { startMove = b; }
	bool StartMove() { return startMove; }

	void SetBRender(bool b) { bRender = b; }
	bool BRender() { return bRender; }

public:
	void SetVelocity(int i);

	void SetStartDirection(int i) { startDirection = i; }

public:
	void SetOffsetX(float f) { offsetPositionX = f; }
	void SetOffsetY(float f) { offsetPositionY = f; }

private:
	bool startMove = false;
	bool bRender = false;

	int startDirection = 0;

private:
	float offsetPositionX = 0.f;
	float offsetPositionY = 0.f;

};