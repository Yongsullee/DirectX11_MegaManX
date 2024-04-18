#pragma once
#include "Weapons/Weapon.h"

class Missile : public Weapon
{
public:
	Missile(wstring shaderFile, D3DXVECTOR2 start, float angle);
	virtual ~Missile();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	virtual void SetWeaponSpec() override;

	Sprite* GetSprite() { return sprite; }

public:
	virtual D3DXVECTOR2 Move() override;

	void AdjustPosition(D3DXVECTOR2 _pos);

public:
	virtual void HandleAttack() override;
	virtual void HandleDestruction() override;

private:
	Sprite* sprite;

private:
	class FlyingDustEffect* flyingDustEffect;

private:
	D3DXVECTOR2 velocity;
	float speed;
};