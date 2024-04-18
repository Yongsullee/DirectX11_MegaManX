#pragma once
#include "Objects/Bullet.h"

class Level2Bullet : public Bullet
{
public:
	Level2Bullet(wstring shaderFile, D3DXVECTOR2 start, float angle);
	~Level2Bullet();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;
	virtual void Render() override;
	
	/***************************************/ // Handle

	void HandleStateNumber();
	virtual void HandleDestruction() override;

	/***************************************/ // State

	void DelayStartState();
	void ChangeStateNumber0to1Conditon();
	
	virtual void DelayDeadState() override;

public:
	void SetStartMove(bool b) { startMove = b; }
	bool StartMove() { return startMove; }

	void SetBRender(bool b) { bRender = b; }
	bool BRender() { return bRender; }

	void SetStartDirection(int i) { startDirection = i; }

	void SetVelocity(int i);

	D3DXVECTOR2 AdjustPosition();

public:
	void SetOffsetX(float f) { offsetPositionX = f; }
	void SetOffsetY(float f) { offsetPositionY = f; }

private:
	bool startMove = false;
	bool bRender = false;

private:

	int frameCycle = 0;

	bool stateChange = false;

	int startDirection = 0;

private:
	float offsetPositionX = 0.f;
	float offsetPositionY = 0.f;

};