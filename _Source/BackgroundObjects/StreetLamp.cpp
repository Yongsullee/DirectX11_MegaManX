#include "stdafx.h"
#include "StreetLamp.h"

StreetLamp::StreetLamp()
	:BackgroundObject()
{
	SetBCollide(false);

	wstring textureFile = L"";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	Sprite* lamp;

	// 가로등 1번
	textureFile = L"/MegaManX/Background/StreetLamp1.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-9643.f, 148.f);
	sprites.push_back(lamp);

	//가로등 2번
	textureFile = L"/MegaManX/Background/StreetLamp2.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-8875.f, 148.f);
	sprites.push_back(lamp);

	//가로등 3번
	textureFile = L"/MegaManX/Background/StreetLamp3.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-8096.f, 196.f);
	sprites.push_back(lamp);

	//가로등 4번
	textureFile = L"/MegaManX/Background/StreetLamp4.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-7328.f, 196.f);
	sprites.push_back(lamp);

	//가로등 5번
	textureFile = L"/MegaManX/Background/StreetLamp5.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-6560.f, 172.f);
	sprites.push_back(lamp);

	//가로등 6번
	textureFile = L"/MegaManX/Background/StreetLamp6.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-5792.f, 172.f);
	sprites.push_back(lamp);

	//가로등 7번
	textureFile = L"/MegaManX/Background/StreetLamp7.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-5024.f, 172.f);
	sprites.push_back(lamp);

	//가로등 8번
	textureFile = L"/MegaManX/Background/StreetLamp8.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-4256.f, 172.f);
	sprites.push_back(lamp);

	//가로등 9번
	textureFile = L"/MegaManX/Background/StreetLamp9.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-1973.f, 120.f);
	sprites.push_back(lamp);

	//가로등 10번
	textureFile = L"/MegaManX/Background/StreetLamp10.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(-438.f, 120.f);
	sprites.push_back(lamp);

	//가로등 11번
	textureFile = L"/MegaManX/Background/StreetLamp11.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(736.f, 172.f);
	sprites.push_back(lamp);

	//가로등 12번
	textureFile = L"/MegaManX/Background/StreetLamp12.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(1504.f, 172.f);
	sprites.push_back(lamp);

	//가로등 13번
	textureFile = L"/MegaManX/Background/StreetLamp13.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(4170.f, 120.f);
	sprites.push_back(lamp);

	//가로등 14번
	textureFile = L"/MegaManX/Background/StreetLamp14.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(5728.f, 173.f);
	sprites.push_back(lamp);

	//가로등 15번
	textureFile = L"/MegaManX/Background/StreetLamp15.png";
	lamp = new Sprite(Textures + textureFile, shaderFile);
	lamp->Scale(3.0f, 3.0f);
	lamp->Position(6475.f, 117.f);
	sprites.push_back(lamp);

	//가로등 16번
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
