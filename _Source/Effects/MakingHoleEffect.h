#pragma once
#include "Effects/Effect.h"

class MakingHoleEffect : public Effect
{
public:
	/*
	
		목적:
			위치, 깊이, 방향 설정
		설명:
			1. direction의 경우
				1 -> 왼
				2 -> 중간
				3 -> 오른쪽
	
	*/
	MakingHoleEffect(D3DXVECTOR2 _position, int _direction);
	virtual ~MakingHoleEffect();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	Sprite* GetSprite() { return sprite; }

	void SetPlayerPos(D3DXVECTOR2& _position) { playerPos = _position; }
	D3DXVECTOR2 PlayerPos() { playerPos; }

	void SetBCollide(bool b) { bCollide = b; }
	bool BCollide() { return bCollide; }

public:
	void AdjustPosition(D3DXVECTOR2 _weaponPos);

private:
	Sprite* sprite;

private:
	int direction;

private:
	D3DXVECTOR2 playerPos;

	bool bCollide = false;
};