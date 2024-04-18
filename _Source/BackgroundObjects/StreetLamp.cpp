#include "stdafx.h"
#include "StreetLamp.h"

StreetLamp::StreetLamp()
	:BackgroundObject()
{
	SetBCollide(false);

	wstring textureFile = L"";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	Sprite* lamp;

	// ���ε� 1��
	textureFile = L"/MegaManX/Background/StreetLamp1.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-9643.f, 148.f);
	sprites.push_back(lamp);

	//���ε� 2��
	textureFile = L"/MegaManX/Background/StreetLamp2.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-8875.f, 148.f);
	sprites.push_back(lamp);

	//���ε� 3��
	textureFile = L"/MegaManX/Background/StreetLamp3.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-8096.f, 196.f);
	sprites.push_back(lamp);

	//���ε� 4��
	textureFile = L"/MegaManX/Background/StreetLamp4.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-7328.f, 196.f);
	sprites.push_back(lamp);

	//���ε� 5��
	textureFile = L"/MegaManX/Background/StreetLamp5.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-6560.f, 172.f);
	sprites.push_back(lamp);

	//���ε� 6��
	textureFile = L"/MegaManX/Background/StreetLamp6.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-5792.f, 172.f);
	sprites.push_back(lamp);

	//���ε� 7��
	textureFile = L"/MegaManX/Background/StreetLamp7.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-5024.f, 172.f);
	sprites.push_back(lamp);

	//���ε� 8��
	textureFile = L"/MegaManX/Background/StreetLamp8.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-4256.f, 172.f);
	sprites.push_back(lamp);

	//���ε� 9��
	textureFile = L"/MegaManX/Background/StreetLamp9.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-1973.f, 120.f);
	sprites.push_back(lamp);

	//���ε� 10��
	textureFile = L"/MegaManX/Background/StreetLamp10.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-438.f, 120.f);
	sprites.push_back(lamp);

	//���ε� 11��
	textureFile = L"/MegaManX/Background/StreetLamp11.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(736.f, 172.f);
	sprites.push_back(lamp);

	//���ε� 12��
	textureFile = L"/MegaManX/Background/StreetLamp12.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(1504.f, 172.f);
	sprites.push_back(lamp);

	//���ε� 13��
	textureFile = L"/MegaManX/Background/StreetLamp13.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(4170.f, 120.f);
	sprites.push_back(lamp);

	//���ε� 14��
	textureFile = L"/MegaManX/Background/StreetLamp14.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(5728.f, 173.f);
	sprites.push_back(lamp);

	//���ε� 15��
	textureFile = L"/MegaManX/Background/StreetLamp15.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(6475.f, 117.f);
	sprites.push_back(lamp);

	//���ε� 16��
	textureFile = L"/MegaManX/Background/StreetLamp16.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(7241.f, 117.f);
	sprites.push_back(lamp);
}

StreetLamp::~StreetLamp()
{
	for (Sprite* sprite : sprites)
		SAFE_DELETE(sprite);
}

void StreetLamp::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	for (Sprite* sprite : sprites)
		sprite->Update(V, P);
}

void StreetLamp::Render()
{
	for (Sprite* sprite : sprites)
		sprite->Render();
}
