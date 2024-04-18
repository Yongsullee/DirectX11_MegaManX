#pragma once
#include "Bullet.h"

class BossEnemyBullet : public Bullet
{
public:
	BossEnemyBullet(D3DXVECTOR2 _pos, float _angle);
	virtual ~BossEnemyBullet();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;
	virtual void Render() override;

	void GetEnemyPos(D3DXVECTOR2& _enemyPos) { enemyPos = _enemyPos; }
	void GetPlayerPos(D3DXVECTOR2& _playerPos) { playerPos = _playerPos; }

	/*****************************************/ // Handle

	void HandleStateNumber();
	void HandleMovement();
	virtual void HandleDestruction() override;

	/*****************************************/ // State

	void DelayStartState();
	void CheckStateNumberChange0to1Condition();
	
	/*****************************************/ // Adjust Position

	void AdjustPosition(D3DXVECTOR2& _pos);
	void AngleBetweenPlayer(D3DXVECTOR2& _pos);

	/*****************************************/ // Physics
	
	void SetVelocity(int i);
	void Move(D3DXVECTOR2& _pos);

	/*****************************************/ // Setter & Getter


	void SetStartMove(bool b) { startMove = b; }
	bool StartMove() { return startMove; }

	void SetBRender(bool b) { bRender = b; }
	bool BRender() { return bRender; }

	void SetStartDirection(int i) { startDirection = i; }

public:
	void SetOffsetX(float f) { offsetPositionX = f; }
	void SetOffsetY(float f) { offsetPositionY = f; }

private:
	bool startMove = false;
	bool bRender = false;

private:
	int frameCycle = 0;
	int startDirection = 0;

private:
	D3DXVECTOR2 enemyPos = D3DXVECTOR2(0.f, 0.f);

	float offsetPositionX = 0.f;
	float offsetPositionY = 0.f;

};