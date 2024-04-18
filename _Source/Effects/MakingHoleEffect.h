#pragma once
#include "Effects/Effect.h"

class MakingHoleEffect : public Effect
{
public:
	/*
	
		����:
			��ġ, ����, ���� ����
		����:
			1. direction�� ���
				1 -> ��
				2 -> �߰�
				3 -> ������
	
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