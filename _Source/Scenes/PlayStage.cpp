#include "stdafx.h"
#include "PlayStage.h"
#include "Viewer/Camera.h"
#include "Viewer/Following.h"
#include "Viewer/Freedom.h"
#include "Objects/Background.h"
#include "Objects/Player.h"
#include "Objects/Bullet.h"
#include "Objects/Level1Bullet.h"
#include "Objects/Level2Bullet.h"
#include "Objects/Level3Bullet.h"
#include "Objects/Line.h"
#include "Objects/Level1Bullet.h"
#include "Objects/Level2Bullet.h"
#include "Objects/Level3Bullet.h"
#include "Objects/BossEnemyBullet.h"
#include "BackgroundObjects/BackgroundObject.h"
#include "BackgroundObjects/PartiableBridge.h"
#include "BackgroundObjects/SmallPartiableBridge.h"
#include "BackgroundObjects/StreetLamp.h"
#include "BackgroundObjects/Truck.h"
#include "BackgroundObjects/NormalCar.h"
#include "Enemies/Enemy.h"
#include "Enemies/SpinEnemy.h"
#include "Enemies/MissileEnemy.h"
#include "Enemies/LandMineEnemy.h"
#include "Enemies/BeeEnemy.h"
#include "Enemies/SuicideBombEnemy.h"
#include "Enemies/CarEnemy.h"
#include "Enemies/SubBossEnemy.h"
#include "Enemies/SubBossPawn.h"
#include "Enemies/BossEnemy.h"
#include "Weapons/Missile.h"
#include "Weapons/LandMine.h"
#include "Effects/Effect.h"
#include "Effects/MakingHoleEffect.h"
#include "Interfaces/HealthLevel.h"


/*****************************************************************/ // Main

PlayStage::PlayStage(SceneValues * values)
	:Scene(values)
	, lineCount(0), wallCount(0)
{
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	/*****************************************************************/ // Background

	background = new Background(shaderFile);

	streetLamps = new StreetLamp();

	/*****************************************************************/ // Line & Wall

	/*
	목적:
		플레이어 충돌을 다루기위해 Collide Line 생성
	설명:
		플레이어 위치, 행동, 속성 변화를 구현하기위해
		Enemy, Weapons, BackgroundObject(Collision O)
		객체 들에 Line 혹은 Bound Line을 생성합니다.
	*/

	LinePointsSetting();

	WallPointsSetting();

	/*****************************************************************/ //Partiable Bridges

	/*
		목적:
			Partiable Bridge 생성
		설명:
			게임 과정에서 분리 가능한 Bridge를 생성하고,
			플레이어 진행 상황에 따라 Drop, Land 동작
	*/
	for (int i = 0; i < 2; i++)
	{
		PartiableBridge* bridge = new PartiableBridge(i + 1);
		partiableBridges.push_back(bridge);
	}

	/*****************************************************************/ // Small Partiable Bridge

	SmallPartiableBridge* smallBridge;
	smallBridge = new SmallPartiableBridge(0, D3DXVECTOR2(LINE_7_END_POINT_X+58.f, LINE_7_POS_Y - 80.3f));
	smallPartiableBridges.push_back(smallBridge);
	smallBridge = new SmallPartiableBridge(0, D3DXVECTOR2(LINE_8_END_POINT_X+58.f, LINE_8_POS_Y - 80.3f));
	smallPartiableBridges.push_back(smallBridge);
	smallBridge = new SmallPartiableBridge(0, D3DXVECTOR2(LINE_9_END_POINT_X+58.f, LINE_9_POS_Y - 78.5f));
	smallPartiableBridges.push_back(smallBridge);
	smallBridge = new SmallPartiableBridge(1, D3DXVECTOR2(LINE_10_START_POINT_X-68.f, LINE_10_POS_Y - 78.52f));
	smallPartiableBridges.push_back(smallBridge);
	smallBridge = new SmallPartiableBridge(2, D3DXVECTOR2(LINE_11_START_POINT_X-118.f, LINE_11_POS_Y - 78.5f));
	smallPartiableBridges.push_back(smallBridge);
	smallBridge = new SmallPartiableBridge(3, D3DXVECTOR2(LINE_12_START_POINT_X-150.2f, LINE_12_POS_Y - 78.45f));
	smallPartiableBridges.push_back(smallBridge);
	smallBridge = new SmallPartiableBridge(1, D3DXVECTOR2(LINE_13_START_POINT_X - 62.f, LINE_12_POS_Y - 174.5f));
	smallPartiableBridges.push_back(smallBridge);

	/*****************************************************************/ // Cars

	for (int i = 0; i < 2; i++)
	{
		trucks.push_back(new Truck(CARS_START_POINT_1 + i*500.f,i + 1, false, true, false));
	}
	for (int i = 0; i < 2; i++)
	{
		normalCars.push_back(new NormalCar(CARS_START_POINT_2 + i * 500.f, i + 1, false, true, false));
	}
	
	/*****************************************************************/ // Enemies
	// 난수 생성
	std::random_device rd;
	std::mt19937 gen(rd());

	float startPosX = 0.f;
	float startPosY = 0.f;

	//// 지상 적 1번, Spin Enemy 생성
	SpinEnemy* spin;

	spin = new SpinEnemy(D3DXVECTOR2(SPIN_1_START_POINT_X, SPIN_1_START_POINT_Y), SPIN_1_TARGET_POINT);
	spinEnemies.push_back(spin);
	spin = new SpinEnemy(D3DXVECTOR2(SPIN_2_START_POINT_X, SPIN_2_START_POINT_Y), SPIN_2_TARGET_POINT);
	spinEnemies.push_back(spin);

	//// 지상 적 2번, Missile Enemy 생성
	MissileEnemy* missile;

	missile = new MissileEnemy(D3DXVECTOR2(MISSILE_1_START_POINT_X, MISSILE_1_START_POINT_Y));
	missileEnemies.push_back(missile);

	missile = new MissileEnemy(D3DXVECTOR2(MISSILE_2_START_POINT_X, MISSILE_2_START_POINT_Y));
	missileEnemies.push_back(missile);

	missile = new MissileEnemy(D3DXVECTOR2(MISSILE_3_START_POINT_X, MISSILE_3_START_POINT_Y));
	missileEnemies.push_back(missile);

	// 공중 적 1번, LandMine Enemy 생성
	LandMineEnemy* landMineEnemy;

	// 1st
	{
		std::uniform_real_distribution<> distr1(LANDMINE_1_START_POINT_MIN_X, LANDMINE_1_START_POINT_MAX_X);
		startPosX = distr1(gen);

		std::uniform_real_distribution<> distr2(LANDMINE_1_START_POINT_MIN_Y, LANDMINE_1_START_POINT_MAX_Y);
		startPosY = distr2(gen);

		landMineEnemy = new LandMineEnemy(D3DXVECTOR2(startPosX, startPosY)
			, D3DXVECTOR2(startPosX - 500.f, startPosY));
		landMineEnemies.push_back(landMineEnemy);
	}
	{
		// 2nd
		std::uniform_real_distribution<> distr1(LANDMINE_2_START_POINT_MIN_X, LANDMINE_2_START_POINT_MAX_X);
		startPosX = distr1(gen);

		std::uniform_real_distribution<> distr2(LANDMINE_2_START_POINT_MIN_Y, LANDMINE_2_START_POINT_MAX_Y);
		startPosY = distr2(gen);

		landMineEnemy = new LandMineEnemy(D3DXVECTOR2(startPosX, startPosY)
			, D3DXVECTOR2(startPosX - 500.f, startPosY));
		landMineEnemies.push_back(landMineEnemy);

	}
	{
		// 3rd
		std::uniform_real_distribution<> distr1(LANDMINE_3_START_POINT_MIN_X, LANDMINE_3_START_POINT_MAX_X);
		startPosX = distr1(gen);

		std::uniform_real_distribution<> distr2(LANDMINE_3_START_POINT_MIN_Y, LANDMINE_3_START_POINT_MAX_Y);
		startPosY = distr2(gen);

		landMineEnemy = new LandMineEnemy(D3DXVECTOR2(startPosX, startPosY)
			, D3DXVECTOR2(startPosX - 500.f, startPosY));
		landMineEnemies.push_back(landMineEnemy);
	}
	{
		// 4th
		std::uniform_real_distribution<> distr1(LANDMINE_4_START_POINT_MIN_X, LANDMINE_4_START_POINT_MAX_X);
		startPosX = distr1(gen);

		std::uniform_real_distribution<> distr2(LANDMINE_4_START_POINT_MIN_Y, LANDMINE_4_START_POINT_MAX_Y);
		startPosY = distr2(gen);

		landMineEnemy = new LandMineEnemy(D3DXVECTOR2(startPosX, startPosY)
			, D3DXVECTOR2(startPosX - 500.f, startPosY));
		landMineEnemies.push_back(landMineEnemy);
	}
	{
		// 5th
		std::uniform_real_distribution<> distr1(LANDMINE_5_START_POINT_MIN_X, LANDMINE_5_START_POINT_MAX_X);
		startPosX = distr1(gen);

		std::uniform_real_distribution<> distr2(LANDMINE_5_START_POINT_MIN_Y, LANDMINE_5_START_POINT_MAX_Y);
		startPosY = distr2(gen);

		landMineEnemy = new LandMineEnemy(D3DXVECTOR2(startPosX, startPosY)
			, D3DXVECTOR2(startPosX - 500.f, startPosY));
		landMineEnemies.push_back(landMineEnemy);
	}
	{
		// 6th
		std::uniform_real_distribution<> distr1(LANDMINE_6_START_POINT_MIN_X, LANDMINE_6_START_POINT_MAX_X);
		startPosX = distr1(gen);

		std::uniform_real_distribution<> distr2(LANDMINE_6_START_POINT_MIN_Y, LANDMINE_6_START_POINT_MAX_Y);
		startPosY = distr2(gen);

		landMineEnemy = new LandMineEnemy(D3DXVECTOR2(startPosX, startPosY)
			, D3DXVECTOR2(startPosX - 500.f, startPosY));
		landMineEnemies.push_back(landMineEnemy);
		{
		// 7th
		std::uniform_real_distribution<> distr1(LANDMINE_7_START_POINT_MIN_X, LANDMINE_7_START_POINT_MAX_X);
		startPosX = distr1(gen);

		std::uniform_real_distribution<> distr2(LANDMINE_7_START_POINT_MIN_Y, LANDMINE_7_START_POINT_MAX_Y);
		startPosY = distr2(gen);

		landMineEnemy = new LandMineEnemy(D3DXVECTOR2(startPosX, startPosY)
			, D3DXVECTOR2(startPosX - 500.f, startPosY));
		landMineEnemies.push_back(landMineEnemy);
	}


	//공중 적 2번, Bee Enemy 생성

	BeeEnemy* bee;

	for (int i = 0; i < 2; i++)
	{
		std::uniform_real_distribution<> distr(BEE_1_START_POINT_MIN, BEE_1_START_POINT_MAX);
		startPosX = distr(gen);
		bee = new BeeEnemy(D3DXVECTOR2(startPosX, Math::Random(200.f, 240.f)), D3DXVECTOR2(startPosX - 700.f, Math::Random(100.f, 200.f)));
		beeEnemies.push_back(bee);
	}
	for (int i = 2; i < 4; i++)
	{
		std::uniform_real_distribution<> distr(BEE_2_START_POINT_MIN, BEE_2_START_POINT_MAX);
		startPosX = distr(gen);
		bee = new BeeEnemy(D3DXVECTOR2(startPosX, Math::Random(200.f, 240.f)), D3DXVECTOR2(startPosX - 700.f, Math::Random(100.f, 200.f)));
		beeEnemies.push_back(bee);
	}
	for (int i = 4; i < beeEnemyNum; i++)
	{
		std::uniform_real_distribution<> distr(BEE_3_START_POINT_MIN, BEE_3_START_POINT_MAX);
		startPosX = distr(gen);
		bee = new BeeEnemy(D3DXVECTOR2(startPosX, Math::Random(200.f, 240.f)), D3DXVECTOR2(startPosX - 700.f, Math::Random(100.f, 200.f)));
		beeEnemies.push_back(bee);
	}

	//공중 적 3번, Bomb Enemy 생성
	SuicideBombEnemy* bomb;

	for (int i = 0; i < 1; i++)
	{
		std::uniform_real_distribution<> distr(BOMB_1_START_POINT_MIN, BOMB_1_START_POINT_MAX);
		startPosX = distr(gen);
		bomb = new SuicideBombEnemy(D3DXVECTOR2(startPosX, Math::Random(170.f, 200.f)), D3DXVECTOR2(startPosX - 700.f, Math::Random(100.f, 200.f)));
		bombEnemies.push_back(bomb);
	}
	for (int i = 1; i < 3; i++)
	{
		std::uniform_real_distribution<> distr(BOMB_2_START_POINT_MIN, BOMB_2_START_POINT_MAX);
		startPosX = distr(gen);
		bomb = new SuicideBombEnemy(D3DXVECTOR2(startPosX, Math::Random(170.f, 200.f)), D3DXVECTOR2(startPosX - 700.f, Math::Random(100.f, 200.f)));
		bombEnemies.push_back(bomb);
	}
	for (int i = 3; i < bombEnemyNum; i++)
	{
		std::uniform_real_distribution<> distr(BOMB_3_START_POINT_MIN, BOMB_3_START_POINT_MAX);
		startPosX = distr(gen);
		bomb = new SuicideBombEnemy(D3DXVECTOR2(startPosX, Math::Random(170.f, 200.f)), D3DXVECTOR2(startPosX - 700.f, Math::Random(100.f, 200.f)));
		bombEnemies.push_back(bomb);
	}

	// 지상 적 3번, Car Enemy 생성
	CarEnemy* car;
	car = new CarEnemy(D3DXVECTOR2(CAR_1_RIGHT, CAR_ENEMY_Y), CAR_1_LEFT, CAR_1_LEFT, CAR_1_RIGHT);
	carEnemies.push_back(car);
	car = new CarEnemy(D3DXVECTOR2(CAR_2_RIGHT, CAR_ENEMY_Y), CAR_2_LEFT, CAR_2_LEFT, CAR_2_RIGHT);
	carEnemies.push_back(car);
	car = new CarEnemy(D3DXVECTOR2(CAR_3_RIGHT, CAR_ENEMY_Y-100.f), CAR_3_LEFT, CAR_3_LEFT, CAR_3_RIGHT);
	carEnemies.push_back(car);

	// 서브 보스
	SubBossEnemy* subBoss = new SubBossEnemy(D3DXVECTOR2(SUBBOSS_1_X, SUBBOSS_1_Y), SUBBOSS_1_TARGET);
	subBossEnemies.push_back(subBoss);
	SubBossEnemy* subBoss2 = new SubBossEnemy(D3DXVECTOR2(SUBBOSS_2_X, SUBBOSS_2_Y), SUBBOSS_2_TARGET);
	subBossEnemies.push_back(subBoss2);

	// 메인 보스
	bossEnemy = new BossEnemy(D3DXVECTOR2(BOSS_X, BOSS_Y), BOSS_TARGET);

	/*****************************************************************/ // Bullet

	for (int i = 0; i < 50; i++)
	{
		// Level 1
		Level1Bullet* bullet1 = new Level1Bullet(shaderFile, D3DXVECTOR2(6000.f, 100.f), 0);
		level1Bullets.push_back(bullet1);

		// Level 2
		Level2Bullet* bullet2 = new Level2Bullet(shaderFile, D3DXVECTOR2(6000.f, 100.f), 0);
		level2Bullets.push_back(bullet2);
		
		// Level 3
		Level3Bullet* bullet3 = new Level3Bullet(shaderFile, D3DXVECTOR2(6000.f, 100.f), 0);
		level3Bullets.push_back(bullet3);
	}

	/*****************************************************************/ // Player & Camera

	player = new Player(D3DXVECTOR2(START_POINT, 100.f), D3DXVECTOR2(2.5f, 2.5f));
	camera = new Following(player);
	values->MainCamera = camera;

	/*****************************************************************/ // Interfaces

	healthLevelInterface = new HealthLevel(100);

}

PlayStage::~PlayStage()
{
	SAFE_DELETE(background);
	SAFE_DELETE(streetLamps);

	for (Line* line : lines)
		SAFE_DELETE(line);
	for (Line* wall : walls)
		SAFE_DELETE(wall);

	for (PartiableBridge* bridge : partiableBridges)
		SAFE_DELETE(bridge);
	for (SmallPartiableBridge* smallBridge : smallPartiableBridges)
		SAFE_DELETE(smallBridge);

	for (Truck* truck : trucks)
		SAFE_DELETE(truck);
	for (NormalCar* normalCar : normalCars)
		SAFE_DELETE(normalCar);

	for (SpinEnemy* enemy : spinEnemies)
		SAFE_DELETE(enemy);
	for (MissileEnemy* enemy : missileEnemies)
		SAFE_DELETE(enemy);
	for (LandMineEnemy* enemy : landMineEnemies)
		SAFE_DELETE(enemy);
	for (BeeEnemy* enemy : beeEnemies)
		SAFE_DELETE(enemy);
	for (SuicideBombEnemy* enemy : bombEnemies)
		SAFE_DELETE(enemy);
	for (CarEnemy* enemy : carEnemies)
		SAFE_DELETE(enemy);
	for (SubBossEnemy* enemy : subBossEnemies)
		SAFE_DELETE(enemy);
	SAFE_DELETE(bossEnemy);

	for (Level1Bullet* bullet : level1Bullets)
		SAFE_DELETE(bullet);
	for (Level2Bullet* bullet : level2Bullets)
		SAFE_DELETE(bullet);
	for (Level3Bullet* bullet : level3Bullets)
		SAFE_DELETE(bullet);

	SAFE_DELETE(player);
	SAFE_DELETE(camera);

	SAFE_DELETE(healthLevelInterface);

}

bool Invincible = false;

void PlayStage::Update()
{
	// values->Projection 중심 맞추기
	CameraProjection();

	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	values->MainCamera->Update();

	CentralizeMousePos();

	/*****************************************************************/ // Player

	/*
		목적:
			Player vs BackgroundObject의 충돌 여부를 검사합니다.
		설명:
			1. 먼저 Line의 가로 범위 안에 Player가 존재하는지 체크합니다.
				
				* player->SetBFalling(false)
				* PlayerLineVerticalCollisionCheck() 호출

			2. 다음으로, Line의 위쪽에서 Player의 세로 스케일 범위 안에 Line이 충돌 했는지 체크합니다.

				* PlayerLineHorizontalCollisionCheck() 호출
				* 함수 내부에서 AdjustPlayerPosition() 호출

			3. 가로 범위 안에 Player가 존재하지 않는다면,

				* DrawCollision(false)
				* player->SetBFalling(true)
	*/

	PlayerBOutRange(*player);

	PlayerLineCollisionCheck(*player);
	PlayerWallCollisionCheck(*player);
	PlayerHoleCollisionCheck(*player);

	PlayerDeadSubBossCollisionCheck(*player);

	PlayerEnemyCollisionCheck(*player);
	PlayerWeaponCollisionCheck(*player);

	PlayerSpawnBullets(*player);

	CheckPlayerMeetSubBoss1(*player);
	CheckPlayerMeetSubBoss2(*player);
	CheckPlayerMeetBoss(*player);

	CheckSubBoss1Dead(*(subBossEnemies.at(0)), *player);
	CheckSubBoss2Dead(*(subBossEnemies.at(1)), *player);
	CheckBossDead(*bossEnemy, *player);

	player->Update(V, P);

	/*****************************************************************/ //Bullets

	CheckBulletsOutOfRange(*player);
	HandleBulletsDestruciton(*player);

	/*****************************************************************/ //Bg

	background->BackgroundMove(*player);
	background->Update(V, P);

	streetLamps->Update(V, P);

	/*****************************************************************/ //Line + Wall

	for (Line* line : lines)
		line->Update(V, P);

	for (Line* wall : walls)
		wall->Update(V, P);

	/*****************************************************************/ // Partiable Bridge + Small Partiable Bridge

	for (PartiableBridge* bridge : partiableBridges)
	{
		PlayerPartialBridgeCollisionCheck(*player, *bridge);
		bridge->Update(V, P);
	}

	for (SmallPartiableBridge* smallBridge : smallPartiableBridges)
	{
		PlayerSmallPartialBridgeCollisionCheck(*player, *smallBridge);
		smallBridge->Update(V, P);
	}

	/*****************************************************************/ // Trucks + NormalCars

	ExpireCars();

	for (Truck* truck : trucks)
	{
		truck->GetPlayerPos(*player);	// 플레이어 위치 정보 받기
		truck->Update(V, P);
	}
	for (NormalCar* normalCar : normalCars)
	{
		normalCar->GetPlayerPos(*player);
		normalCar->Update(V, P);
	}

	/*****************************************************************/ //All Enemies

	ExpireEnemies();

	// 0. Enemy vs Line
	EnemyLineCollisionCheck();
	// 1. Enemy vs Bullets
	EnemyBulletCollisionCheck(*player);
	// 2. Enemy vs Partiable Bridge ( * Sub Boss 한정 )
	EnemyPartiableBridgeCollisionCheck();

	for (SpinEnemy* enemy : spinEnemies)
	{
		enemy->GetPlayerPos(*player);
		enemy->Update(V, P);
	}

	CheckMissilesOutOfRange(*player);
	
	for (MissileEnemy* enemy : missileEnemies)
	{
		enemy->GetPlayerPos(*player);
		enemy->Update(V, P);
	}

	WeaponLineCollisionCheck();
	
	for (LandMineEnemy* enemy : landMineEnemies)
	{
		enemy->GetPlayerPos(*player);
		enemy->Update(V, P);
	}

	for (BeeEnemy* enemy : beeEnemies)
	{
		enemy->GetPlayerPos(*player);
		enemy->Update(V, P);
	}

	for (SuicideBombEnemy* enemy : bombEnemies)
	{
		enemy->GetPlayerPos(*player);
		enemy->Update(V, P);
	}

	for (CarEnemy* enemy : carEnemies)
	{
		enemy->GetPlayerPos(*player);
		enemy->Update(V, P);
	}

	for (SubBossEnemy* enemy : subBossEnemies)
	{
		enemy->GetPlayerPos(*player);
		enemy->Update(V, P);
	}

	bossEnemy->GetPlayerPos(*player);
	bossEnemy->Update(V, P);

	/*****************************************************************/ //Weapon

	// 2. Weapon vs Player
	for (Level1Bullet* bullet : level1Bullets)
	{
		bullet->SetPlayerPos(*player);

		// Bullet의 Start 위치 조정
		if (player->LastDirection() == 0)
			bullet->SetOffsetX(30.f);
		else
			bullet->SetOffsetX(-30.f);

		bullet->Update(V, P);
	}

	for (Level2Bullet* bullet : level2Bullets)
	{
		bullet->SetPlayerPos(*player);

		// Bullet의 Start 위치 조정
		if (player->LastDirection() == 0)
			bullet->SetOffsetX(30.f);
		else
			bullet->SetOffsetX(-30.f);

		bullet->Update(V, P);
	}

	for (Level3Bullet* bullet : level3Bullets)
	{
		bullet->SetPlayerPos(*player);

		// Bullet의 Start 위치 조정
		if (player->LastDirection() == 0)
			bullet->SetOffsetX(30.f);
		else
			bullet->SetOffsetX(-30.f);

		bullet->Update(V, P);
	}

	/*****************************************************************/ //Effects

	//HolesOverlapConditionCheck();

	/*****************************************************************/ //Interfaces

	healthLevelInterface->GetPlayerPos(*player);
	healthLevelInterface->SetPlayerHealthLevel(*player);
	healthLevelInterface->Update(V, P);

}

void PlayStage::Render()
{
	if (ImGui::Button("Invincible") == true)
		Invincible = true;
	ImGui::LabelText("Mouse Position", "%.0f, %.0f", finalMousePos.x, finalMousePos.y);
	ImGui::LabelText("Player Position", "%.0f, %.0f", player->GetSprite()->Position().x, player->GetSprite()->Position().y);


	/*****************************************************************/ //Bg

	background->Render();

	/*****************************************************************/ //Bg Objects

	for (PartiableBridge* bridge : partiableBridges)
	{
		bridge->Render();
	}

	for (SmallPartiableBridge* smallBridge : smallPartiableBridges)
	{
		smallBridge->Render();
	}

	/*****************************************************************/ //Line & Wall

	for (Line* line : lines)
		line->Render();

	for (Line* wall : walls)
		wall->Render();

	/*****************************************************************/ //Trucks.z > Player.z 

	for (Truck* truck : trucks)
		truck->Render();

	/*****************************************************************/ //Enemies

	for (SpinEnemy* enemy : spinEnemies)
	{
		enemy->Render();
	}
	for (MissileEnemy* enemy : missileEnemies)
	{
		enemy->Render();
	}
	for (LandMineEnemy* enemy : landMineEnemies)
	{
		enemy->Render();
	}
	for (BeeEnemy* enemy : beeEnemies)
	{
		enemy->Render();
	}
	for (SuicideBombEnemy* enemy : bombEnemies)
	{
		enemy->Render();
	}
	for (CarEnemy* enemy : carEnemies)
	{
		enemy->Render();
	}
	for (SubBossEnemy* enemy : subBossEnemies)
	{
		enemy->Render();
	}
	bossEnemy->Render();

	/*****************************************************************/ //Player

	player->Render();

	for (Level1Bullet* bullet : level1Bullets)
	{
		if(bullet->BRender() == true)
			bullet->Render();
	}
	for (Level2Bullet* bullet : level2Bullets)
	{
		if (bullet->BRender() == true)
			bullet->Render();
	}
	for (Level3Bullet* bullet : level3Bullets)
	{
		if (bullet->BRender() == true)
			bullet->Render();
	}

	/*****************************************************************/ //Normal Cars.z < Player.z 

	for (NormalCar* normalCar : normalCars)
		normalCar->Render();

	/*****************************************************************/ //Bg Objects

	streetLamps->Render();

	/*****************************************************************/ //Interfaces

	healthLevelInterface->Render();
}
			
/****************************************************************************************************/ // Camera

void PlayStage::CameraProjection()
{
	D3DXMatrixOrthoOffCenterLH(&values->Projection, 0, (float)Width, 0, (float)Height, -1, 1);
}

void PlayStage::CentralizeMousePos()
{
	D3DXVECTOR2 mouse = Mouse->Position();
	mouse.y = (mouse.y - (float)Height) * -1.0f;

	D3DXVECTOR2 camera = values->MainCamera->Position();

	finalMousePos = mouse + camera;
}

/****************************************************************************************************/ // ETC

float PlayStage::LineFunction(Line& line, float input)
{
	// ax + b = y
	float a = line.Gradient();
	float b = line.getStartPos().y - line.getStartPos().x * a;

	return a * input + b;
}

float PlayStage::PToPDistance(D3DXVECTOR2 & start, D3DXVECTOR2 & end)
{
	float result = 0.f;

	return result = sqrtf(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
}

void PlayStage::PlayerBOutRange(Player & player)
{
	/*

		목적:
			Player 와 Line, Wall, Partiable Bridge의 충돌 여부 검사
		설명:
			1. Player가 어떠한 Object와도 충돌하지 않을 때,
			
				* Player->SetBFalling(true)
				* Player->SetGroundPosition(-2000.f)

	*/
	bool Check = false;

	for (int i = 0; i < partiableBridges.size(); i++)
	{
		if (partiableBridges.at(i)->BCollide() == true)
		{
			Check = true;
			break;
		}
	}

	for (int i = 0; i < smallPartiableBridges.size(); i++)
	{
		if (smallPartiableBridges.at(i)->BCollide() == true)
		{
			Check = true;
			break;
		}
	}

	for (int i = 0; i < lines.size(); i++)
	{
		if (lines.at(i)->GetDrawCollision() == true
			&& player.BCollideWithHole() == false)
		{
			Check = true;
			break;
		}
	}

	for (int i = 0; i < walls.size(); i++)
	{
		if (walls.at(i)->GetDrawCollision() == true)
		{
			Check = true;
			break;
		}
	}

	for (int i = 0; i < landMineEnemies.size(); i++)
	{
		for (int j = 0; j < landMineEnemies.at(i)->landMine->holeEffects.size(); j++)
		{
			if (landMineEnemies.at(i)->landMine->holeEffects.at(j)->BCollide() == true)
			{
				Check = true;
				break;
			}
		}
	}

	for (int i = 0; i < subBossEnemies.size(); i++)
	{
		if (subBossEnemies.at(i)->BUnderPlayer() == true)
		{
			Check = true;
			break;
		}
	}

	if (Check == false)
	{
		player.SetBFalling(true);
		player.SetBOnGround(false);
		player.GetSprite()->DrawCollision(false);
		player.SetGroundPosition(-2000.f);
	}
}

/****************************************************************************************************/ // Bg Objects

/*
	목적:
		일정 범위를 넘어선 Background Object를 화면에서 지웁니다.
*/

void PlayStage::ExpireCars()
{
	for (int i = 0; i < trucks.size(); i++)
	{
		if (trucks.at(i)->Position().x < -10700.f)
		{
			trucks.erase(trucks.begin() + i);
		}
	}

	for (int i = 0; i < normalCars.size(); i++)
	{
		if (normalCars.at(i)->Position().x < -10700.f)
		{
			normalCars.erase(normalCars.begin() + i);
		}
	}
}

/****************************************************************************************************/ // Line

void PlayStage::LinePointsSetting()
{
	/*
		목적:
			Background Object위에 생성된 Line의 시작 점과 도착 점을 Pair로 관리합니다.
		설명:
			총 18개 라인의 시작점과 도착점을 pair 타입으로 묶어서
			벡터 타입의 "vecotr<pair<D3DXVECTOR2, D3DXVECTOR2> linePointPairs"로 관리합니다. 
	*/
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_1_START_POINT_X, LINE_1_POS_Y), D3DXVECTOR2(LINE_1_END_POINT_X, LINE_1_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_2_START_POINT_X, LINE_2_POS_Y), D3DXVECTOR2(LINE_2_END_POINT_X, LINE_2_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_3_START_POINT_X, LINE_3_POS_Y), D3DXVECTOR2(LINE_3_END_POINT_X, LINE_3_POS_Y)));
	
	// 낙하 구간
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_4_1_START_POINT_X, LINE_4_POS_Y), D3DXVECTOR2(LINE_4_1_END_POINT_X, LINE_4_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_4_2_START_POINT_X, LINE_4_POS_Y), D3DXVECTOR2(LINE_4_2_END_POINT_X, LINE_4_POS_Y)));
	// 낙하 구간
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_5_1_START_POINT_X, LINE_5_POS_Y), D3DXVECTOR2(LINE_5_1_END_POINT_X, LINE_5_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_5_2_START_POINT_X, LINE_5_POS_Y), D3DXVECTOR2(LINE_5_2_END_POINT_X, LINE_5_POS_Y)));

	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_6_START_POINT_X, LINE_6_START_POS_Y), D3DXVECTOR2(LINE_6_END_POINT_X, LINE_6_END_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_7_START_POINT_X, LINE_7_POS_Y), D3DXVECTOR2(LINE_7_END_POINT_X, LINE_7_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_8_START_POINT_X+20.f, LINE_8_POS_Y), D3DXVECTOR2(LINE_8_END_POINT_X, LINE_8_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_9_START_POINT_X+5.f, LINE_9_POS_Y), D3DXVECTOR2(LINE_9_END_POINT_X-5.f, LINE_9_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_10_START_POINT_X+5.f, LINE_10_POS_Y), D3DXVECTOR2(LINE_10_END_POINT_X-5.f, LINE_10_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_11_START_POINT_X+5.f, LINE_11_POS_Y), D3DXVECTOR2(LINE_11_END_POINT_X-5.f, LINE_11_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_12_START_POINT_X+12.f, LINE_12_POS_Y), D3DXVECTOR2(LINE_12_END_POINT_X-12.f, LINE_12_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_13_START_POINT_X, LINE_13_POS_Y), D3DXVECTOR2(LINE_13_END_POINT_X, LINE_13_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_14_START_POINT_X, LINE_14_START_POS_Y), D3DXVECTOR2(LINE_14_END_POINT_X, LINE_14_END_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_15_START_POINT_X, LINE_15_POS_Y), D3DXVECTOR2(LINE_15_END_POINT_X-30.f, LINE_15_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_16_START_POINT_X+30, LINE_16_POS_Y), D3DXVECTOR2(LINE_16_END_POINT_X, LINE_16_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_17_START_POINT_X, LINE_17_POS_Y), D3DXVECTOR2(LINE_17_END_POINT_X, LINE_17_POS_Y)));
	SetLinePointsPairs(make_pair(D3DXVECTOR2(LINE_18_START_POINT_X, LINE_18_POS_Y), D3DXVECTOR2(LINE_18_END_POINT_X, LINE_18_POS_Y)));

	Line* sprite;

	for (int i = 0; i < lineCount; i++)
	{
		sprite = new Line(shaderFile, linePointsPairs.at(i).first, linePointsPairs.at(i).second, 0);
		sprite->DrawBound(true);
		lines.push_back(sprite);
	}
}

void PlayStage::SetLinePointsPairs(pair<D3DXVECTOR2, D3DXVECTOR2> linePos)
{
	/*
		목적:
			입력받은 pair 타입의 linePos를 "vector<pair<D3DXVECTOR2, D3DXVECTOR2>>linePointsPairs"에 추가합니다. 
		설명:
			PlayStage::lineCount를 Line Points Pair가 추가될 때, 증가시켜줍니다.
	*/
	linePointsPairs.push_back(make_pair(linePos.first, linePos.second));

	lineCount++;
}

/****************************************************************************************************/ // Wall

void PlayStage::WallPointsSetting()
{
	/*
		목적:
			Background Object위에 생성된 Line의 시작 점과 도착 점을 Pair로 관리합니다.
		설명:
			총 18개 라인의 시작점과 도착점을 pair 타입으로 묶어서
			벡터 타입의 "vecotr<pair<D3DXVECTOR2, D3DXVECTOR2> linePointPairs"로 관리합니다.
	*/
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_1_START_POINT_X, WALL_1_Y), D3DXVECTOR2(LINE_1_START_POINT_X, LINE_1_POS_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_2_START_POINT_X-30.f, LINE_2_POS_Y-30.f), D3DXVECTOR2(LINE_2_START_POINT_X-30.f, WALL_3_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_3_END_POINT_X+30.f, LINE_3_POS_Y-30.f), D3DXVECTOR2(LINE_3_END_POINT_X+30.f, WALL_6_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_4_1_START_POINT_X-30.f, LINE_4_POS_Y-30.f), D3DXVECTOR2(LINE_4_1_START_POINT_X-30.f, WALL_7_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_4_2_END_POINT_X+25.f, LINE_4_POS_Y-30.f), D3DXVECTOR2(LINE_4_2_END_POINT_X+25.f, WALL_8_Y)));

	// 낙하 구간
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_4_1_END_POINT_X+30.f, LINE_4_POS_Y-30.f), D3DXVECTOR2(LINE_4_1_END_POINT_X+30.f, DROP_WALL_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_4_2_START_POINT_X-30.f, LINE_4_POS_Y-30.f), D3DXVECTOR2(LINE_4_2_START_POINT_X-30.f, DROP_WALL_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_5_1_START_POINT_X-40.f, LINE_5_POS_Y-30.f), D3DXVECTOR2(LINE_5_1_START_POINT_X-40.f, WALL_9_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_5_2_END_POINT_X+30.f, LINE_5_POS_Y-30.f), D3DXVECTOR2(LINE_5_2_END_POINT_X+30.f, WALL_10_Y)));
	// 낙하 구간
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_5_1_END_POINT_X+20.f, LINE_5_POS_Y-30.f), D3DXVECTOR2(LINE_5_1_END_POINT_X+20.f, DROP_WALL_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_5_2_START_POINT_X-30.f, LINE_5_POS_Y-30.f), D3DXVECTOR2(LINE_5_2_START_POINT_X-30.f, DROP_WALL_Y)));

	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_6_START_POINT_X-30.f, LINE_6_START_POS_Y-30.f), D3DXVECTOR2(LINE_6_START_POINT_X-30.f, WALL_11_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_7_END_POINT_X+10.f, LINE_7_POS_Y-30.f), D3DXVECTOR2(LINE_7_END_POINT_X+10.f, WALL_14_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_8_START_POINT_X, LINE_8_POS_Y - 30.f), D3DXVECTOR2(LINE_8_START_POINT_X, WALL_15_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_8_END_POINT_X+15.f, LINE_8_POS_Y - 30.f), D3DXVECTOR2(LINE_8_END_POINT_X+15.f, WALL_16_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_9_START_POINT_X-16.f, LINE_9_POS_Y-30.f), D3DXVECTOR2(LINE_9_START_POINT_X-16.f, WALL_17_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_9_END_POINT_X+10.f, LINE_9_POS_Y-30.f), D3DXVECTOR2(LINE_9_END_POINT_X+10.f, WALL_18_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_10_START_POINT_X-20.f, LINE_10_POS_Y-30.f), D3DXVECTOR2(LINE_10_START_POINT_X-20.f, WALL_19_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_10_END_POINT_X+20.f, LINE_10_POS_Y-30.f), D3DXVECTOR2(LINE_10_END_POINT_X+20.f, WALL_20_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_11_START_POINT_X-20.f, LINE_11_POS_Y-30.f), D3DXVECTOR2(LINE_11_START_POINT_X-20.f, WALL_21_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_11_END_POINT_X+10.f, LINE_11_POS_Y-30.f), D3DXVECTOR2(LINE_11_END_POINT_X+10.f, WALL_22_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_12_START_POINT_X-7.f, LINE_12_POS_Y-30.f), D3DXVECTOR2(LINE_12_START_POINT_X-7.f, WALL_23_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_12_END_POINT_X+10.f, LINE_12_POS_Y-30.f), D3DXVECTOR2(LINE_12_END_POINT_X+10.f, WALL_24_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_13_START_POINT_X-12.f, LINE_13_POS_Y-30.f), D3DXVECTOR2(LINE_13_START_POINT_X-12.f, WALL_25_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_15_END_POINT_X+20.f, LINE_15_POS_Y-20.f), D3DXVECTOR2(LINE_15_END_POINT_X+20.f, WALL_27_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_16_START_POINT_X-20.f, LINE_16_POS_Y-20.f), D3DXVECTOR2(LINE_16_START_POINT_X-20.f, WALL_28_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_16_END_POINT_X+20.f, LINE_16_POS_Y-20.f), D3DXVECTOR2(LINE_16_END_POINT_X+20.f, WALL_27_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_17_START_POINT_X-30.f, LINE_17_POS_Y-20.f), D3DXVECTOR2(LINE_17_START_POINT_X-30.f, WALL_29_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_17_END_POINT_X+30.f, LINE_17_POS_Y), D3DXVECTOR2(LINE_17_END_POINT_X+30.f, WALL_30_Y)));
	
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_18_START_POINT_X-30.f, LINE_18_POS_Y), D3DXVECTOR2(LINE_18_START_POINT_X-30.f, WALL_31_Y)));
	SetWallPointsPairs(make_pair(D3DXVECTOR2(LINE_18_END_POINT_X+30.f, LINE_18_POS_Y), D3DXVECTOR2(LINE_18_END_POINT_X+30.f, WALL_32_Y)));

	Line* sprite;

	for (int i = 0; i < wallCount; i++)
	{
		sprite = new Line(shaderFile, wallPointsPairs.at(i).first, wallPointsPairs.at(i).second, 0);
		sprite->DrawBound(true);
		walls.push_back(sprite);
	}
}

void PlayStage::SetWallPointsPairs(pair<D3DXVECTOR2, D3DXVECTOR2> linePos)
{
	/*
	목적:
		입력받은 pair 타입의 linePos를 "vector<pair<D3DXVECTOR2, D3DXVECTOR2>>linePointsPairs"에 추가합니다.
	설명:
		PlayStage::lineCount를 Line Points Pair가 추가될 때, 증가시켜줍니다.
*/
	wallPointsPairs.push_back(make_pair(linePos.first, linePos.second));

	wallCount++;
}

/****************************************************************************************************/ // Player vs Line

void PlayStage::PlayerLineCollisionCheck(Player & player)
{
	for (int i = 0; i < lines.size(); i++)
	{
		// Player vs Line - Vertical Check
		if (PlayerLineVerticalCollisionCheck(player, *lines.at(i)) == true)
		{
			player.SetBFalling(false);

			// Player vs Line - Horizontal Check
			PlayerLineHorizontalCollisionCheck(player, *lines.at(i));
		}
		else
		{
			lines.at(i)->DrawCollision(false);
		}
	}
}

bool PlayStage::PlayerLineVerticalCollisionCheck(Player & player, Line & line)
{
	float xScale = player.GetSprite()->Scale().x * player.GetSprite()->TextureSize().x * 0.5f;
	float yScale = player.GetSprite()->Scale().y * player.GetSprite()->TextureSize().y * 0.5f;

	float playerLeft = player.GetSprite()->Position().x - xScale;
	float playerRight = player.GetSprite()->Position().x + xScale;

	if (playerLeft < line.getEndPos().x &&
		playerRight > line.getStartPos().x)
	{
		return true;
	}

	return false;
}

void PlayStage::PlayerLineHorizontalCollisionCheck(Player & player, Line& line)
{
	/*
		목적:
			Player와 Line의 Collision 여부를 체크합니다.
		설명:
			
			1.	Player의 중심부와 Line과의 거리 그리고, Player의 scaleY(Player 중심부로부터 바닥 까지 거리)를 비교합니다.
				이때, Player의 중심부와 Line과의 거리가 Player의 Y scale 값 보다 적으면, Check = true;
			
			2. Player의 중심부가 Line보다 위에 있을 경우 Check = true
			
			3. 모든 조건을 충족할 경우 Player와 Line의 충돌을 인정합니다.
	*/
	float xScale = player.GetSprite()->Scale().x * player.GetSprite()->TextureSize().x * 0.5f;
	float yScale = player.GetSprite()->Scale().y * player.GetSprite()->TextureSize().y * 0.5f;

	float playerCenterX = player.GetSprite()->Position().x;
	float playerCenterY = player.GetSprite()->Position().y;

	bool Check = true;

	if (PToPDistance(D3DXVECTOR2(playerCenterX, playerCenterY), D3DXVECTOR2(playerCenterX, LineFunction(line, playerCenterX)))
			<= yScale)
	{
		Check &= true;
	}
	else
	{
		Check &= false;
	}

	Check &= playerCenterY > LineFunction(line, playerCenterX);

	AdjustPlayerPosition(Check, player, line);
}

void PlayStage::AdjustPlayerPosition(bool bCollide, Player& player, Line& line)
{
	/*
		목적:
			Collision 여부에 따라 Player의 상태 변화를 적용합니다.
		설명:
			Line 과 Player가 충돌하면 호출되는 함수이며,
			
			1. Line 과 Player의  DrawCollision을 True로 설정합니다.
			2. Player의 GroundPosition이 Line.y + Player의 Scale().y * TextureSize().y * 0.5f로 설정
	*/
	float xScale = player.GetSprite()->Scale().x * player.GetSprite()->TextureSize().x * 0.5f;
	float yScale = player.GetSprite()->Scale().y * player.GetSprite()->TextureSize().y * 0.5f;

	float playerCenterX = player.GetSprite()->Position().x;

	float groundPos = LineFunction(line, playerCenterX) + yScale - 16.f;
	float groundAngle = line.getAngle();

	if (bCollide == true)
	{
		line.DrawCollision(true);
		player.GetSprite()->DrawCollision(true);
	}
	
	if(bCollide == false)
	{
		line.DrawCollision(false);
	}

	// 예외로, Hole을 만나게되면 다른 동작을 수행합니다.
	if (player.BCollideWithHole() == false)
	{
		player.SetGroundPosition(groundPos);
		player.SetAngle(groundAngle);
	}
}

/****************************************************************************************************/ // Player vs Wall

void PlayStage::PlayerWallCollisionCheck(Player & player)
{
	if (player.BOnGround() == false || player.BFalling() == true)
	{
		for (int i = 0; i < walls.size(); i++)
		{
			// Player vs Wall - Vertical Check
			if (PlayerWallHorizontalCollisionCheck(player, *walls.at(i)) == true
				&& player.BWallJump() == false)
			{
				// Player vs Wall - Horizontal Check
				PlayerWallVerticalCollisionCheck(player, *walls.at(i));
			}
			else
			{
				walls.at(i)->DrawCollision(false);
			}
		}
	}
}

bool PlayStage::PlayerWallHorizontalCollisionCheck(Player & player, Line & line)
{
	float xScale = player.GetSprite()->Scale().x * player.GetSprite()->TextureSize().x * 0.5f;
	float yScale = player.GetSprite()->Scale().y * player.GetSprite()->TextureSize().y * 0.5f;

	float playerCenterX = player.GetSprite()->Position().x;

	if (fabs(playerCenterX - line.getStartPos().x) <= xScale)
	{
		return true;
	}

	return false;
}

void PlayStage::PlayerWallVerticalCollisionCheck(Player & player, Line & line)
{
	float xScale = player.GetSprite()->Scale().x * player.GetSprite()->TextureSize().x * 0.5f;
	float yScale = player.GetSprite()->Scale().y * player.GetSprite()->TextureSize().y * 0.5f;

	float playerTop = player.GetSprite()->Position().y + yScale;
	float playerBottom = player.GetSprite()->Position().y - yScale;

	bool Check = false;

	if (playerTop > line.getEndPos().y
		&& playerBottom < line.getStartPos().y)
	{
		Check = true;
	}

	AdjustPlayerWallPosition(Check, player, line);
}

void PlayStage::AdjustPlayerWallPosition(bool bCollide, Player & player, Line & line)
{
	float xScale = player.GetSprite()->Scale().x * player.GetSprite()->TextureSize().x * 0.5f;

	float playerCenterX = player.GetSprite()->Position().x;
	float playerCenterY = player.GetSprite()->Position().y;

	float linePosX = line.getStartPos().x;

	if (bCollide == true)
	{
		line.DrawCollision(true);

		player.GetSprite()->DrawCollision(true);
		if (player.BOnWall() == false)
		{
			float wallPosX = playerCenterX + -1.f * player.LimitFlag() * (xScale + 2.f);
			player.GetSprite()->Position(wallPosX, playerCenterY);

			if (linePosX > playerCenterX)
			{
				player.SetLimitFlag(1);
			}
			else if (linePosX < playerCenterX)
			{
				player.SetLimitFlag(-1);
			}
			player.StartWallFriciton();
		}
	}

	if (bCollide == false)
	{
		player.SetLimitFlag(0);
		line.DrawCollision(false);
	}
}

/****************************************************************************************************/ //Player vs Partiable Bridgs + Small Partiable Bridges

/*
	목적:
		Player와 Partiable Bridge, Small Partiable Bridge의 Aabb 충돌 체크
	설명:
		Player와 Partiable Bridge 혹은 Small Partiable Bridge의 Aabb를 Update() 안에서 지속적으로 체크합니다.

		* 주의: Small Partiable Bridge의 경우에만, bBroken을 True로 설정합니다.
*/

void PlayStage::PlayerPartialBridgeCollisionCheck(Player & player, PartiableBridge& bridge)
{
	if (Sprite::Aabb(player.GetSprite(), bridge.GetSprite()) == true)
	{
		float yScale = player.GetSprite()->Scale().y * player.GetSprite()->TextureSize().y * 0.5f;
		float bridgeYscale = bridge.GetSprite()->Scale().y * bridge.GetSprite()->TextureSize().y * 0.5f;
		float groundPos = 0.f;

		if(bridge.StateNumber() == 0)
			groundPos = bridge.Position().y + bridgeYscale + yScale - 31.f;
		else
			groundPos = bridge.Position().y + bridgeYscale + yScale - 9.f;

		bridge.SetBCollide(true);
		bridge.GetSprite()->DrawCollision(true);

		player.SetGroundPosition(groundPos);
	}
	else
	{
		bridge.SetBCollide(false);
		bridge.GetSprite()->DrawCollision(false);
	}
}

void PlayStage::PlayerSmallPartialBridgeCollisionCheck(Player & player, SmallPartiableBridge& bridge)
{
	if (bridge.BRender() == true
		&& Sprite::Aabb(player.GetSprite(), bridge.GetSprite()) == true)
	{
		float yScale = player.GetSprite()->Scale().y * player.GetSprite()->TextureSize().y * 0.5f;
		float bridgeYscale = bridge.GetSprite()->Scale().y * bridge.GetSprite()->TextureSize().y * 0.5f;
		float groundPos = bridge.Position().y + bridgeYscale + yScale - 30.f;

		// 부서짐이 시작됩니다.
		bridge.SetBCollide(true);
		bridge.SetBBroken(true);
		bridge.GetSprite()->DrawCollision(true);

		player.SetGroundPosition(groundPos);
	}
	else
	{
		bridge.SetBCollide(false);
		bridge.GetSprite()->DrawCollision(false);
	}
}

/****************************************************************************************************/ //Player vs Bullet

/*

	목적:
		Bullet을 생성합니다.

*/

void PlayStage::PlayerSpawnBullets(Player & _player)
{
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	int direction = 0;

	// Bullet의 Angle 설정
	if (_player.LastDirection() == 180)
	{
		if (_player.BOnWall() == true)
		{
			direction = 0;
		}
		else
		{
			direction = 180;
		}
	}
	else
	{
		if (_player.BOnWall() == true)
		{
			direction = 180;
		}
		else
		{
			direction = 0;
		}
	}

	// Level 1
	if (_player.BSpawnLevel1Bullet() == true)
	{
		level1Bullets.at(Level1BulletNum)->SetAngle(direction);
		level1Bullets.at(Level1BulletNum)->SetVelocity(direction);

		level1Bullets.at(Level1BulletNum)->SetBRender(true);
		level1Bullets.at(Level1BulletNum)->SetStartMove(true);

		_player.SetBSpawnLevel1Bullet(false);

		Level1BulletNum++;
	}
	// Level 2
	if (_player.BSpawnLevel2Bullet() == true)
	{
		level2Bullets.at(Level2BulletNum)->SetAngle(direction);
		level2Bullets.at(Level2BulletNum)->SetVelocity(direction);

		level2Bullets.at(Level2BulletNum)->SetBRender(true);
		level2Bullets.at(Level2BulletNum)->SetStartMove(true);

		_player.SetBSpawnLevel2Bullet(false);

		Level2BulletNum++;
	}
	// Level 3
	if (_player.BSpawnLevel3Bullet() == true)
	{
		level3Bullets.at(Level3BulletNum)->SetAngle(direction);
		level3Bullets.at(Level3BulletNum)->SetVelocity(direction);

		level3Bullets.at(Level3BulletNum)->SetBRender(true);
		level3Bullets.at(Level3BulletNum)->SetStartMove(true);

		_player.SetBSpawnLevel3Bullet(false);

		Level3BulletNum++;
	}
}

/****************************************************************************************************/ // Player vs Enemy

/*	목적:
		Player와 Enemy간의 물리적 충돌을 감지하고 피해량을 감지합니다.
	설명:
		Player와 Enemy의 Aabb 충돌을 통해 충돌 여부를 검사하고,
		충돌했다면, Enemy의 collideLevel만큼 Player에게 Damage를 입힙니다.

		1. Aabb 충돌 여부 검사
		2. Enemy의 Collide Level이 0보다 큰지 검사 * Enemy는 죽고 Crash Effect로 전환되어 Collide Level이 0이 됩니다.
*/


void PlayStage::PlayerEnemyCollisionCheck(Player& _player)
{
	std::string damageType = "Collision";

	// 지상 적 1번, Spin Enemy
	for (int i = 0; i < spinEnemies.size(); i++)
	{
		if (Sprite::Aabb(_player.GetSprite(), spinEnemies.at(i)->GetSprite()) == true
			&& spinEnemies.at(i)->CollideLevel() > 0)
		{
			int damage = spinEnemies.at(i)->CollideLevel();
			_player.DamageTaken(damage, damageType);
		}
	}
	// 지상 적 2번, Missile Enemy
	for (int i = 0; i < missileEnemies.size(); i++)
	{
		if (Sprite::Aabb(_player.GetSprite(), missileEnemies.at(i)->GetSprite()) == true
			&& missileEnemies.at(i)->CollideLevel() > 0)
		{
			_player.GetSprite()->DrawCollision(true);
			missileEnemies.at(i)->GetSprite()->DrawCollision(true);

			int damage = missileEnemies.at(i)->CollideLevel();
			_player.DamageTaken(damage, damageType);
		}
		else
		{
			missileEnemies.at(i)->GetSprite()->DrawCollision(false);
		}
	}
	// 공중 적 1번, LandMine Enemy
	for (int i = 0; i < landMineEnemies.size(); i++)
	{
		if (Sprite::Aabb(_player.GetSprite(), landMineEnemies.at(i)->GetSprite()) == true
			&& landMineEnemies.at(i)->CollideLevel() > 0)
		{
			_player.GetSprite()->DrawCollision(true);
			landMineEnemies.at(i)->GetSprite()->DrawCollision(true);

			int damage = landMineEnemies.at(i)->CollideLevel();
			_player.DamageTaken(damage, damageType);
		}
		else
		{
			landMineEnemies.at(i)->GetSprite()->DrawCollision(false);
		}
	}
	// 공중 적 2번, Bee Enemy
	for (int i = 0; i < beeEnemies.size(); i++)
	{
		if (Sprite::Aabb(_player.GetSprite(), beeEnemies.at(i)->GetSprite()) == true
			&& beeEnemies.at(i)->CollideLevel() > 0)
		{
			_player.GetSprite()->DrawCollision(true);
			beeEnemies.at(i)->GetSprite()->DrawCollision(true);

			int damage = beeEnemies.at(i)->CollideLevel();
			_player.DamageTaken(damage, damageType);
		}
		else
		{
			beeEnemies.at(i)->GetSprite()->DrawCollision(false);
		}
	}
	// 공중 적 3번, Bomb Enemy
	for (int i = 0; i < bombEnemies.size(); i++)
	{
		if (Sprite::Aabb(_player.GetSprite(), bombEnemies.at(i)->GetSprite()) == true
			&& bombEnemies.at(i)->CollideLevel() > 0
			&& bombEnemies.at(i)->BMove() == true)
		{
			bombEnemies.at(i)->HandleDamage(80);
			int damage = bombEnemies.at(i)->CollideLevel();
			_player.DamageTaken(damage, damageType);
		}
	}
	// 지상 적 3번, Car Enemy
	for (int i = 0; i < carEnemies.size(); i++)
	{
		if (Sprite::Aabb(_player.GetSprite(), carEnemies.at(i)->GetSprite()) == true
			&& carEnemies.at(i)->CollideLevel() > 0)
		{
			int damage = carEnemies.at(i)->CollideLevel();
			_player.DamageTaken(damage, damageType);
		}
	}
	// 서브 보스, Sub Boss
	for (int i = 0; i < subBossEnemies.size(); i++)
	{
		if (subBossEnemies.at(i)->BDead() == false)
		{
			if (Sprite::Aabb(_player.GetSprite(), subBossEnemies.at(i)->GetSprite()) == true)
			{
				int damage = subBossEnemies.at(i)->CollideLevel();
				_player.DamageTaken(damage, damageType);
			}
		}
	}
	// 서브 소스 병사, Sub Boss Pawn
	for (int i = 0; i < subBossEnemies.size(); i++)
	{
		for (int j = 0; j < subBossEnemies.at(i)->subBossPawns.size(); j++)
		{
			if (subBossEnemies.at(i)->subBossPawns.at(j)->BDead() == false)
			{
				if (Sprite::Aabb(_player.GetSprite(), subBossEnemies.at(i)->subBossPawns.at(j)->GetSprite()) == true)
				{
					int damage = subBossEnemies.at(i)->subBossPawns.at(j)->CollideLevel();
					_player.DamageTaken(damage, damageType);
				}
			}
		}
	}
	// 보스 적
	{
		if (Sprite::Aabb(_player.GetSprite(), bossEnemy->GetSprite()) == true
			&& bossEnemy->CollideLevel() > 0)
		{
			_player.GetSprite()->DrawCollision(true);
			bossEnemy->GetSprite()->DrawCollision(true);
			int damage = bossEnemy->CollideLevel();
			damageType = "Electric";
			_player.DamageTaken(damage, damageType);
		}
		else
		{
			bossEnemy->GetSprite()->DrawCollision(false);
		}
	}

}

/*

	목적:
		죽은 SubBoss를 Player는 올라탈 수 있습니다.
	설명:
		1. Line, 그리고 Wall과 동일하게 SubBoss의 윗 라인과 Player의 충돌 여부 검사
		2. 조건이 충족되면 Player는 SubBoss의 윗쪽에 위치 할 수 있습니다.

*/

void PlayStage::PlayerDeadSubBossCollisionCheck(Player & _player)
{
	for (int i = 0; i < subBossEnemies.size(); i++)
	{
		if (subBossEnemies.at(i)->BDead() == true)
		{
			if (PlayerDeadSubBossHorizontalCheck(_player, i) == true)
			{
				PlayerDeadSubBossVerticalCheck(_player, i);
			}
			else
			{
				subBossEnemies.at(i)->SetBUnderUnderPlayer(false);
			}
		}
	}
}

bool PlayStage::PlayerDeadSubBossHorizontalCheck(Player& _player, int _index)
{
	float xScale = _player.GetSprite()->Scale().x * _player.GetSprite()->TextureSize().x * 0.5f;
	float enemyXScale = subBossEnemies.at(_index)->GetSprite()->Scale().x * subBossEnemies.at(_index)->GetSprite()->TextureSize().x * 0.5f;

	float playerLeft = _player.GetSprite()->Position().x - xScale;
	float playerRight = _player.GetSprite()->Position().x + xScale;
	float enemyLeft = subBossEnemies.at(_index)->GetSprite()->Position().x - enemyXScale;
	float enemyRight = subBossEnemies.at(_index)->GetSprite()->Position().x + enemyXScale;

	if (playerLeft < enemyRight &&
		playerRight > enemyLeft)
	{
		return true;
	}

	return false;

}

void PlayStage::PlayerDeadSubBossVerticalCheck(Player& _player, int _index)
{
	float xScale = _player.GetSprite()->Scale().x * _player.GetSprite()->TextureSize().x * 0.5f;
	float yScale = _player.GetSprite()->Scale().y * _player.GetSprite()->TextureSize().y * 0.5f;
	float enemyXScale = subBossEnemies.at(_index)->GetSprite()->Scale().x * subBossEnemies.at(_index)->GetSprite()->TextureSize().x * 0.5f;
	float enemyYScale = subBossEnemies.at(_index)->GetSprite()->Scale().y * subBossEnemies.at(_index)->GetSprite()->TextureSize().y * 0.5f;

	float playerCenterX = _player.GetSprite()->Position().x;
	float playerCenterY = _player.GetSprite()->Position().y;

	float enemyTop = subBossEnemies.at(_index)->GetSprite()->Position().y + enemyYScale;

	bool Check = true;

	if (playerCenterY -  enemyTop
		<= yScale)
	{
		Check &= true;
	}
	else
	{
		Check &= false;
	}

	Check &= playerCenterY > enemyTop;	

	// Player의 Ground Position을 설정합니다.
	if (Check == true)
	{
		float groundPos = enemyTop + yScale - 10.f;

		_player.SetGroundPosition(groundPos);
		subBossEnemies.at(_index)->SetBUnderUnderPlayer(true);
	}
	else
	{
		subBossEnemies.at(_index)->SetBUnderUnderPlayer(false);
	}

}

/****************************************************************************************************/ // Player vs Weapon

/*

	목적:
		Enemy의 Weapon과 Player간의 Attack 충돌을 감지하고, Player에 피해량을 적용합니다.
	설명:
		Weapon과 Player의 Obb 충돌을 통해 충돌 여부를 검사하고,
		충돌했다면, Weapon의 Attack Level 만큼 Player에게 Damage를 입힙니다.

*/

void PlayStage::PlayerWeaponCollisionCheck(Player & _player)
{
	std::string damageType = "Attack";

	// Missile Enemy -> Missile
	for (int i = 0; i < missileEnemies.size(); i++)
	{
		for (int j = 0; j < missileEnemies.at(i)->missiles.size(); j++)
		{
			if (Sprite::Obb(_player.GetSprite(), missileEnemies.at(i)->missiles.at(j)->GetSprite()) == true
				&& missileEnemies.at(i)->missiles.at(j)->AttackLevel() > 0)
			{
				_player.GetSprite()->DrawCollision(true);
				missileEnemies.at(i)->missiles.at(j)->GetSprite()->DrawCollision(true);

				int damage = missileEnemies.at(i)->missiles.at(j)->AttackLevel();
				_player.DamageTaken(damage, damageType);
			}
			else
			{
				missileEnemies.at(i)->missiles.at(j)->GetSprite()->DrawCollision(false);
			}
		}
	}
	// LandMine Enemy -> LandMine
	for (int i = 0; i < landMineEnemies.size(); i++)
	{
		if (landMineEnemies.at(i)->landMine->BRender() == true)
		{
			if (Sprite::Aabb(_player.GetSprite(), landMineEnemies.at(i)->landMine->GetSprite()) == true
				&& landMineEnemies.at(i)->landMine->AttackLevel() > 0)
			{
				landMineEnemies.at(i)->landMine->GetSprite()->DrawCollision(true);

				int damage = landMineEnemies.at(i)->landMine->AttackLevel();
				_player.GetSprite()->DrawCollision(true);
				_player.DamageTaken(damage, damageType);
			}
			else
			{
				landMineEnemies.at(i)->landMine->GetSprite()->DrawCollision(false);
			}
		}
	}
	// Boss Enemy -> Bullet
	for (int i = 0; i < bossEnemy->bullets.size(); i++)
	{
		if (bossEnemy->bullets.at(i)->BRender() == true)
		{
			if (Sprite::Aabb(_player.GetSprite(), bossEnemy->bullets.at(i)->GetSprite()) == true
				&& landMineEnemies.at(i)->landMine->AttackLevel() > 0)
			{
				bossEnemy->bullets.at(i)->GetSprite()->DrawCollision(true);

				int damage = bossEnemy->bullets.at(i)->AttackLevel();
				_player.GetSprite()->DrawCollision(true);
				_player.DamageTaken(damage, damageType);
			}
			else
			{
				bossEnemy->bullets.at(i)->GetSprite()->DrawCollision(false);
			}
		}
	}

}

/****************************************************************************************************/ // Player vs Effect

/*

	목적:
		Player가 Hole을 만나면 Hole의 바닥으로 떨어집니다.
	설명:
		1. Aabb 충돌 체크를 통해 Player와 Hole의 충돌을 감지합니다.
		2. 충돌 여부를 체크한 후, Player가 바닥에 닿을때까지 Fall 상태로 전환됩니다.
		3. 바닥에 도달하면, Player의 포지션을 고정시키고, BOnGround 상태로 전환합니다.

*/

void PlayStage::PlayerHoleCollisionCheck(Player & _player)
{
	bool tCheck = false;

	for (int i = 0; i < landMineEnemies.size(); i++)
	{
		for (int j = 0; j < landMineEnemies.at(i)->landMine->holeEffects.size(); j++)
		{
			if ( landMineEnemies.at(i)->landMine->holeEffects.at(j)->BRender() == true
				&& Sprite::Aabb(_player.GetSprite(), landMineEnemies.at(i)->landMine->holeEffects.at(j)->GetSprite()) == true)
			{
				tCheck = true;

				float playerScaleY = _player.GetSprite()->Scale().y * _player.GetSprite()->TextureSize().y * 0.5f;

				float xScale = landMineEnemies.at(i)->landMine->holeEffects.at(j)->GetSprite()->Scale().x * landMineEnemies.at(i)->landMine->holeEffects.at(j)->GetSprite()->TextureSize().x * 0.5f;
				float yScale = landMineEnemies.at(i)->landMine->holeEffects.at(j)->GetSprite()->Scale().y * landMineEnemies.at(i)->landMine->holeEffects.at(j)->GetSprite()->TextureSize().y * 0.5f;

				float holeBottom = landMineEnemies.at(i)->landMine->holeEffects.at(j)->GetSprite()->Position().y - yScale;

				float groundPos = holeBottom + playerScaleY - 9.f;

				landMineEnemies.at(i)->landMine->holeEffects.at(j)->GetSprite()->DrawCollision(true);

				_player.GetSprite()->DrawCollision(true);
				_player.SetBCollideWithHole(true);
				_player.SetGroundPosition(groundPos);

				if (_player.GetSprite()->Position().y <= groundPos)
				{
					landMineEnemies.at(i)->landMine->holeEffects.at(j)->SetBCollide(true);
				}
			}
			else
			{
				landMineEnemies.at(i)->landMine->holeEffects.at(j)->SetBCollide(false);

			}
		}
	}

	if (tCheck == false)
	{
		_player.SetBCollideWithHole(false);
	}

}

/****************************************************************************************************/ // Player vs Camera

/*
	목적:
		Player와 SubBoss가 만나면 카메라를 특정 위치에 고정합니다.
	설명:
		1. Player->SetMeetSubBoss1(true)
		2. healthLevelInterface->SetCameraSetToSubBoss1(true)
*/

void PlayStage::CheckPlayerMeetSubBoss1(Player & _player)
{
	if (_player.MeetSubBoss1() == false
		&& (_player.GetSprite()->Position().x >= -3109.f &&  _player.GetSprite()->Position().x <= SUBBOSS_1_X) )
	{
		_player.SetMeetSubBoss1(true);

		healthLevelInterface->SetCameraSetToSubBoss1(true);
	}
}

void PlayStage::CheckPlayerMeetSubBoss2(Player & _player)
{
	if (_player.MeetSubBoss2() == false
		&& (_player.GetSprite()->Position().x >= -1197.f && _player.GetSprite()->Position().x <= SUBBOSS_2_X))
	{
		_player.SetMeetSubBoss2(true);

		healthLevelInterface->SetCameraSetToSubBoss2(true);
	}
}

void PlayStage::CheckPlayerMeetBoss(Player & _player)
{
	if (_player.MeetBoss() == false
		&& (_player.GetSprite()->Position().x >= BOSS_TARGET && _player.GetSprite()->Position().x <= BOSS_X))
	{
		_player.SetMeetBoss(true);

		healthLevelInterface->SetCameraSetToBoss(true);
	}

}

void PlayStage::CheckSubBoss1Dead(SubBossEnemy & _enemy, Player & _player)
{
	if (_enemy.BDead() == true)
	{
		_player.SetSubBoss1Dead(true);

		healthLevelInterface->SetCameraSetToSubBoss1(false);
	}
}

void PlayStage::CheckSubBoss2Dead(SubBossEnemy & _enemy, Player & _player)
{
	if (_enemy.BDead() == true)
	{
		_player.SetSubBoss2Dead(true);

		healthLevelInterface->SetCameraSetToSubBoss2(false);
	}
}

void PlayStage::CheckBossDead(BossEnemy & _enemy, Player & _player)
{
	if (_enemy.BDead() == true)
	{
		_player.SetBossDead(true);

		healthLevelInterface->SetCameraSetToBoss(false);
	}
}

/****************************************************************************************************/ //Bullet

/****************************************************************************************************/ //Destrcution

/*

	목적:
		Bullet의 범위 지정을 통해 범위 밖으로 나간 Bullet은 삭제합니다.
	설명:
		Bullet의 X 좌표가 Player 위치 + 2.0f *Width를 넘어설 때,
		Bullet을 Dead 처리합니다 ( bullet->SetBDead(true) ).

*/

void PlayStage::CheckBulletsOutOfRange(Player& player)
{
	// Level 1
	for (int i = 0; i < level1Bullets.size(); i++)
	{
		if (level1Bullets.at(i) != nullptr)
		{
			if(ExpireLevel1Bullets(player, *level1Bullets.at(i)) == true)
			{
				level1Bullets.at(i)->SetBDead(true);
			}
		}
	}
	// Level 2
	for (int i = 0; i < level2Bullets.size(); i++)
	{
		if (level2Bullets.at(i) != nullptr)
		{
			if (ExpireLevel2Bullets(player, *level2Bullets.at(i)) == true)
			{
				level2Bullets.at(i)->SetBDead(true);
			}
		}
	}
	// Level 3
	for (int i = 0; i < level3Bullets.size(); i++)
	{
		if (level3Bullets.at(i) != nullptr)
		{
			if (ExpireLevel3Bullets(player, *level3Bullets.at(i)) == true)
			{
				level3Bullets.at(i)->SetBDead(true);
			}
		}
	}
}

bool PlayStage::ExpireLevel1Bullets(Player & player, Level1Bullet & bullet)
{
	float bulletPosX = bullet.GetSprite()->Position().x;
	float endPosX = 0.f;

	if (bullet.Angle() == 0)
	{
		endPosX = player.GetSprite()->Position().x + 10.0f *Width;	
		if (bulletPosX > endPosX)
		{
			return true;
		}
	}
	else
	{
		endPosX = player.GetSprite()->Position().x - 10.0f *Width;
		if (bulletPosX < endPosX)
		{
			return true;
		}
	}

	return false;
}

bool PlayStage::ExpireLevel2Bullets(Player & player, Level2Bullet & bullet)
{
	float bulletPosX = bullet.GetSprite()->Position().x;
	float endPosX = 0.f;

	if (bullet.Angle() == 0)
	{
		endPosX = player.GetSprite()->Position().x + 10.0f *Width;
		if (bulletPosX > endPosX)
		{
			return true;
		}
	}
	else
	{
		endPosX = player.GetSprite()->Position().x - 10.0f *Width;
		if (bulletPosX < endPosX)
		{
			return true;
		}
	}

	return false;
}

bool PlayStage::ExpireLevel3Bullets(Player & player, Level3Bullet & bullet)
{
	float bulletPosX = bullet.GetSprite()->Position().x;
	float endPosX = 0.f;

	if (bullet.Angle() == 0)
	{
		endPosX = player.GetSprite()->Position().x + 10.0f *Width;
		if (bulletPosX > endPosX)
		{
			return true;
		}
	}
	else
	{
		endPosX = player.GetSprite()->Position().x - 10.0f *Width;
		if (bulletPosX < endPosX)
		{
			return true;
		}
	}

	return false;
}

/*

	목적:
		이미 발사된 Bullet 인스턴스가 Dead 상태로 전환되면,
		해당 Bullet 인스턴스를 삭제한 후,
		새로운 Bullet을 추가합니다.

*/

void PlayStage::HandleBulletsDestruciton(Player& _player)
{
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	Level1Bullet* bullet1;
	Level2Bullet* bullet2;
	Level3Bullet* bullet3;

	//Level 1
	for (int i = 0; i < level1Bullets.size(); i++)
	{
		if (level1Bullets.at(i)->BDead() == true)
		{
			level1Bullets.erase(level1Bullets.begin() + i);

			bullet1 = new Level1Bullet(shaderFile, _player.GetSprite()->Position(), 0);
			level1Bullets.push_back(bullet1);

			Level1BulletNum--;
			if (Level1BulletNum < 0)
			{
				Level1BulletNum = 0;
			}
		}
	}
	//Level 2
	for (int i = 0; i < level2Bullets.size(); i++)
	{
		if (level2Bullets.at(i)->BDead() == true)
		{
			level2Bullets.erase(level2Bullets.begin() + i);

			bullet2 = new Level2Bullet(shaderFile, _player.GetSprite()->Position(), 0);
			level2Bullets.push_back(bullet2);

			Level2BulletNum--;
			if (Level2BulletNum < 0)
			{
				Level2BulletNum = 0;
			}
		}
	}
	//Level 3
	for (int i = 0; i < level3Bullets.size(); i++)
	{
		if (level3Bullets.at(i)->BDead() == true)
		{
			level3Bullets.erase(level3Bullets.begin() + i);

			bullet3 = new Level3Bullet(shaderFile, _player.GetSprite()->Position(), 0);
			level3Bullets.push_back(bullet3);

			Level3BulletNum--;
			if (Level3BulletNum < 0)
			{
				Level3BulletNum = 0;
			}
		}
	}
}

/****************************************************************************************************/ // Enemy

/****************************************************************************************************/ // Destruction

void PlayStage::ExpireEnemies()
{
	// 지상 적 1번, Spin Enemy
	for (int i = 0; i < spinEnemies.size(); i++)
	{
		if (spinEnemies.at(i)->BDead() == true)
		{
			spinEnemies.erase(spinEnemies.begin() + i);
		}
	}
	// 지상 적 2번, Missile Enemy
	for (int i = 0; i < missileEnemies.size(); i++)
	{
		if (missileEnemies.at(i)->BDead() == true)
		{
			missileEnemies.erase(missileEnemies.begin() + i);
		}
	}
	// 공중 적 1번, LandMine Enemy
	for (int i = 0; i < landMineEnemies.size(); i++)
	{
		if (landMineEnemies.at(i)->BDead() == true)
		{
			landMineEnemies.erase(landMineEnemies.begin() + i);
		}
	}
	// 공중 적 2번, Bee Enemy
	for (int i = 0; i < beeEnemies.size(); i++)
	{
		if (beeEnemies.at(i)->BDead() == true)
		{
			beeEnemies.erase(beeEnemies.begin() + i);
		}
	}
	// 공중 적 3번, Bomb Enemy
	for (int i = 0; i < bombEnemies.size(); i++)
	{
		if (bombEnemies.at(i)->BDead() == true)
		{
			bombEnemies.erase(bombEnemies.begin() + i);
		}
	}
	// 지상 적 3번, Car Enemy
	for (int i = 0; i < carEnemies.size(); i++)
	{
		if (carEnemies.at(i)->BDead() == true)
		{
			carEnemies.erase(carEnemies.begin() + i);
		}
	}
	// 중간 보스
	{}
	// 최종 보스
	{}
}

/****************************************************************************************************/ // Enemy(지상 적) vs Line

/*

	목적:
		지상 적들과 Line의 충돌 여부를 검사합니다.
	설명:
		1. Line과 충돌 여부 검사
		2. 충돌했다면, Line을 Ground Position으로 세팅합니다.

*/

void PlayStage::EnemyLineCollisionCheck()
{
	for (int i = 0; i < lines.size(); i++)
	{
		// 지상 적 1번, Spin Enemy
		for (int j = 0; j < spinEnemies.size(); j++)
		{
			std::string name = "SpinEnemy";
			// Spin Enemy vs Line - Vertical Check
			if (EnemyLineVerticalCollisionCheck(name, j, *lines.at(i)) == true)
			{
				// Spin Enemy vs Line - Horizontal Check
				EnemyLineHorizontalCollisionCheck(name, j, *lines.at(i));
			}
		}
		// 지상 적 2번, Missile Enemy
		for(int j =0; j<missileEnemies.size(); j++)
		{
			std::string name = "MissileEnemy";
			// Missile Enemy vs Line - Vertical Check
			if (EnemyLineVerticalCollisionCheck(name, j, *lines.at(i)) == true)
			{
				// Missile Enemy vs Line - Horizontal Check
				EnemyLineHorizontalCollisionCheck(name, j, *lines.at(i));
			}
		}	
		// 지상 적 3번, Car Enemy
		for(int j =0; j<carEnemies.size(); j++)
		{
			std::string name = "CarEnemy";
			// Car Enemy vs Line - Vertical Check
			if (EnemyLineVerticalCollisionCheck(name, j, *lines.at(i)) == true)
			{
				// Car Enemy vs Line - Horizontal Check
				EnemyLineHorizontalCollisionCheck(name, j, *lines.at(i));
			}
		}
		// 중간 보스 '병사', SubBoss Pawns
		for (int j = 0; j < subBossEnemies.size(); j++)
		{
			for (int k = 0; k < subBossEnemies.at(j)->subBossPawns.size(); k++)
			{
				// Sub Boss Pawn vs Line - Vertical Check
				if (SubBossPawnLineVerticalCollisionCheck(j, k, *lines.at(i)) == true)
				{
					// Sub Boss Pawn vs Line - Horizontal Check
					SubBossPawnLineHorizontalCollisionCheck(j, k, *lines.at(i));
				}
			}
		}
		// 최종 보스
		{
			std::string name = "BossEnemy";
			if (EnemyLineVerticalCollisionCheck(name, 0, *lines.at(i)) == true)
			{
				EnemyLineHorizontalCollisionCheck(name, 0, *lines.at(i));
			}
		}
	}
}

bool PlayStage::EnemyLineVerticalCollisionCheck(const std::string& _enemyName, int index, Line & line)
{
	// _enemyName == "Spin Enemy"
	if (_enemyName == "SpinEnemy")
	{
		float xScale = spinEnemies.at(index)->GetSprite()->Scale().x * spinEnemies.at(index)->GetSprite()->TextureSize().x * 0.5f;
		float yScale = spinEnemies.at(index)->GetSprite()->Scale().y * spinEnemies.at(index)->GetSprite()->TextureSize().y * 0.5f;

		float playerLeft = spinEnemies.at(index)->GetSprite()->Position().x - xScale;
		float playerRight = spinEnemies.at(index)->GetSprite()->Position().x + xScale;

		if (playerLeft < line.getEndPos().x &&
			playerRight > line.getStartPos().x)
		{
			return true;
		}

		return false;
	}
	// _enemyName == "Missile Eenmy"
	if (_enemyName == "MissileEnemy")
	{
		float xScale = missileEnemies.at(index)->GetSprite()->Scale().x * missileEnemies.at(index)->GetSprite()->TextureSize().x * 0.5f;
		float yScale = missileEnemies.at(index)->GetSprite()->Scale().y * missileEnemies.at(index)->GetSprite()->TextureSize().y * 0.5f;

		float playerLeft = missileEnemies.at(index)->GetSprite()->Position().x - xScale;
		float playerRight = missileEnemies.at(index)->GetSprite()->Position().x + xScale;

		if (playerLeft < line.getEndPos().x &&
			playerRight > line.getStartPos().x)
		{
			return true;
		}

		return false;
	}
	// _enemyName == "Car Enemy"
	if (_enemyName == "CarEnemy")
	{
		float xScale = carEnemies.at(index)->GetSprite()->Scale().x * carEnemies.at(index)->GetSprite()->TextureSize().x * 0.5f;
		float yScale = carEnemies.at(index)->GetSprite()->Scale().y * carEnemies.at(index)->GetSprite()->TextureSize().y * 0.5f;

		float playerLeft = carEnemies.at(index)->GetSprite()->Position().x - xScale;
		float playerRight = carEnemies.at(index)->GetSprite()->Position().x + xScale;

		if (playerLeft < line.getEndPos().x &&
			playerRight > line.getStartPos().x)
		{
			return true;
		}

		return false;
	}
	// _enemyName == "Boss Enemy"
	if (_enemyName == "BossEnemy")
	{
		float xScale = bossEnemy->GetSprite()->Scale().x * bossEnemy->GetSprite()->TextureSize().x * 0.5f;
		float yScale = bossEnemy->GetSprite()->Scale().y * bossEnemy->GetSprite()->TextureSize().y * 0.5f;

		float playerLeft = bossEnemy->GetSprite()->Position().x - xScale;
		float playerRight = bossEnemy->GetSprite()->Position().x + xScale;

		if (playerLeft < line.getEndPos().x &&
			playerRight > line.getStartPos().x)
		{
			return true;
		}

		return false;
	}

	return false;
}

void PlayStage::EnemyLineHorizontalCollisionCheck(const std::string& _enemyName, int index, Line & line)
{
	bool Check = true;

	// _enemyName == "SpinEnemy"
	if (_enemyName == "SpinEnemy")
	{
		float xScale = spinEnemies.at(index)->GetSprite()->Scale().x * spinEnemies.at(index)->GetSprite()->TextureSize().x * 0.5f;
		float yScale = spinEnemies.at(index)->GetSprite()->Scale().y * spinEnemies.at(index)->GetSprite()->TextureSize().y * 0.5f;

		float playerCenterX = spinEnemies.at(index)->GetSprite()->Position().x;
		float playerCenterY = spinEnemies.at(index)->GetSprite()->Position().y;


		if (PToPDistance(D3DXVECTOR2(playerCenterX, playerCenterY), D3DXVECTOR2(playerCenterX, LineFunction(line, playerCenterX)))
			<= yScale)
		{
			Check &= true;
		}
		else
		{
			Check &= false;
		}
		Check &= playerCenterY > LineFunction(line, playerCenterX);
	}
	// _enemyName == "Missile Eenmy"
	if (_enemyName == "MissileEnemy")
	{
		float xScale = missileEnemies.at(index)->GetSprite()->Scale().x * missileEnemies.at(index)->GetSprite()->TextureSize().x * 0.5f;
		float yScale = missileEnemies.at(index)->GetSprite()->Scale().y * missileEnemies.at(index)->GetSprite()->TextureSize().y * 0.5f;

		float playerCenterX = missileEnemies.at(index)->GetSprite()->Position().x;
		float playerCenterY = missileEnemies.at(index)->GetSprite()->Position().y;


		if (PToPDistance(D3DXVECTOR2(playerCenterX, playerCenterY), D3DXVECTOR2(playerCenterX, LineFunction(line, playerCenterX)))
			<= yScale)
		{
			Check &= true;
		}
		else
		{
			Check &= false;
		}
		Check &= playerCenterY > LineFunction(line, playerCenterX);
	}
	// _enemyName == "Car Enemy"
	if (_enemyName == "CarEnemy")
	{
		float xScale = carEnemies.at(index)->GetSprite()->Scale().x * carEnemies.at(index)->GetSprite()->TextureSize().x * 0.5f;
		float yScale = carEnemies.at(index)->GetSprite()->Scale().y * carEnemies.at(index)->GetSprite()->TextureSize().y * 0.5f;

		float playerCenterX = carEnemies.at(index)->GetSprite()->Position().x;
		float playerCenterY = carEnemies.at(index)->GetSprite()->Position().y;


		if (PToPDistance(D3DXVECTOR2(playerCenterX, playerCenterY), D3DXVECTOR2(playerCenterX, LineFunction(line, playerCenterX)))
			<= yScale)
		{
			Check &= true;
		}
		else
		{
			Check &= false;
		}
		Check &= playerCenterY > LineFunction(line, playerCenterX);
	}
	// _enemyName == "FinalBoss Enemy"
	if (_enemyName == "BossEnemy")
	{
		float xScale = bossEnemy->GetSprite()->Scale().x * bossEnemy->GetSprite()->TextureSize().x * 0.5f;
		float yScale = bossEnemy->GetSprite()->Scale().y * bossEnemy->GetSprite()->TextureSize().y * 0.5f;

		float playerCenterX = bossEnemy->GetSprite()->Position().x;
		float playerCenterY = bossEnemy->GetSprite()->Position().y;


		if (PToPDistance(D3DXVECTOR2(playerCenterX, playerCenterY), D3DXVECTOR2(playerCenterX, LineFunction(line, playerCenterX)))
			<= yScale)
		{
			Check &= true;
		}
		else
		{
			Check &= false;
		}
		Check &= playerCenterY > LineFunction(line, playerCenterX);
	}

	AdjustEnemyPosition(Check, _enemyName, index, line);
}

void PlayStage::AdjustEnemyPosition(bool bCollide, const std::string& _enemyName, int index, Line & line)
{
	// _enemyName == "SpinEnemy"
	if (_enemyName == "SpinEnemy")
	{
		float xScale = spinEnemies.at(index)->GetSprite()->Scale().x * spinEnemies.at(index)->GetSprite()->TextureSize().x * 0.5f;
		float yScale = spinEnemies.at(index)->GetSprite()->Scale().y * spinEnemies.at(index)->GetSprite()->TextureSize().y * 0.5f;

		float playerCenterX = spinEnemies.at(index)->GetSprite()->Position().x;

		float groundPos = LineFunction(line, playerCenterX) + yScale - 9.f;
		float groundAngle = line.getAngle();

		if (bCollide == true)
		{
			spinEnemies.at(index)->GetSprite()->DrawCollision(true);
		}

		spinEnemies.at(index)->SetGroundPosition(groundPos);
		spinEnemies.at(index)->SetAngle(groundAngle);
	}
	// _enemyName == "Missile Eenmy"
	if (_enemyName == "MissileEnemy")
	{
		float xScale = missileEnemies.at(index)->GetSprite()->Scale().x * missileEnemies.at(index)->GetSprite()->TextureSize().x * 0.5f;
		float yScale = missileEnemies.at(index)->GetSprite()->Scale().y * missileEnemies.at(index)->GetSprite()->TextureSize().y * 0.5f;

		float playerCenterX = missileEnemies.at(index)->GetSprite()->Position().x;

		float groundPos = LineFunction(line, playerCenterX) + yScale - 9.f;
		float groundAngle = line.getAngle();

		if (bCollide == true)
		{
			missileEnemies.at(index)->GetSprite()->DrawCollision(true);
		}
		missileEnemies.at(index)->SetGroundPosition(groundPos);
		missileEnemies.at(index)->SetAngle(groundAngle);
	}
	// _enemyName == "Car Enemy"
	if (_enemyName == "CarEnemy")
	{
		float xScale = carEnemies.at(index)->GetSprite()->Scale().x * carEnemies.at(index)->GetSprite()->TextureSize().x * 0.5f;
		float yScale = carEnemies.at(index)->GetSprite()->Scale().y * carEnemies.at(index)->GetSprite()->TextureSize().y * 0.5f;

		float playerCenterX = carEnemies.at(index)->GetSprite()->Position().x;

		float groundPos = LineFunction(line, playerCenterX) + yScale - 9.f;
		float groundAngle = line.getAngle();

		if (bCollide == true)
		{
			carEnemies.at(index)->GetSprite()->DrawCollision(true);
		}
		carEnemies.at(index)->SetGroundPosition(groundPos);
		carEnemies.at(index)->SetAngle(groundAngle);
	}
	// _enemyName == "FinalBoss Enemy"
	if (_enemyName == "BossEnemy")
	{
		float xScale = bossEnemy->GetSprite()->Scale().x * bossEnemy->GetSprite()->TextureSize().x * 0.5f;
		float yScale = bossEnemy->GetSprite()->Scale().y * bossEnemy->GetSprite()->TextureSize().y * 0.5f;

		float playerCenterX = bossEnemy->GetSprite()->Position().x;

		float groundPos = LineFunction(line, playerCenterX) + yScale - 9.f;
		float groundAngle = line.getAngle();

		if (bCollide == true)
		{
			bossEnemy->GetSprite()->DrawCollision(true);
		}
		bossEnemy->SetGroundPosition(groundPos);
		bossEnemy->SetAngle(groundAngle);
	}
}

/*
	목적: 
		Sub Boss Pawn (서브 보스 병사) 객체는 2개의 Index를 필요로하므로, 별도의 메서드를 작성합니다.
	설명:
		방식은 위와 동일합니다.
*/

bool PlayStage::SubBossPawnLineVerticalCollisionCheck(int _subBossIndex, int _subBossPawnIndex, Line & line)
{

		float xScale = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Scale().x * subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->TextureSize().x * 0.5f;
		float yScale = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Scale().y * subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->TextureSize().y * 0.5f;

		float playerLeft = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Position().x - xScale;
		float playerRight = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Position().x + xScale;

		if (playerLeft < line.getEndPos().x &&
			playerRight > line.getStartPos().x)
		{
			return true;
		}

		return false;
}

void PlayStage::SubBossPawnLineHorizontalCollisionCheck(int _subBossIndex, int _subBossPawnIndex, Line & line)
{
	float xScale = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Scale().x * subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->TextureSize().x * 0.5f;
	float yScale = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Scale().y * subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->TextureSize().y * 0.5f;

	float playerCenterX = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Position().x;
	float playerCenterY = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Position().y;

	bool Check = true;

	if (PToPDistance(D3DXVECTOR2(playerCenterX, playerCenterY), D3DXVECTOR2(playerCenterX, LineFunction(line, playerCenterX)))
		<= yScale)
	{
		Check &= true;
	}
	else
	{
		Check &= false;
	}
	Check &= playerCenterY > LineFunction(line, playerCenterX);

	AdjustSubBossPawnPosition(Check, _subBossIndex, _subBossPawnIndex, line);
}

void PlayStage::AdjustSubBossPawnPosition(bool bCollide, int _subBossIndex, int _subBossPawnIndex, Line& line)
{
	float xScale = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Scale().x * subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->TextureSize().x * 0.5f;
	float yScale = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Scale().y * subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->TextureSize().y * 0.5f;

	float playerCenterX = subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->Position().x;

	float groundPos = 0;
	float groundAngle = line.getAngle();

	if (bCollide == true)
	{
		subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->GetSprite()->DrawCollision(true);
	}

	groundPos = LineFunction(line, playerCenterX) + yScale;
	
	subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->SetGroundPosition(groundPos);
	subBossEnemies.at(_subBossIndex)->subBossPawns.at(_subBossPawnIndex)->SetAngle(groundAngle);
}

/****************************************************************************************************/ // Enemy 1번, 미사일 적

/*

	목적:
		Missile Enemy가 발사한 Missile이 유효 범위를 넘어섰는지 확인합니다.

*/

void PlayStage::CheckMissilesOutOfRange(Player& _player)
{
	for (int i = 0; i < missileEnemies.size(); i++)
	{
		for (int j = 0; j < missileEnemies.at(i)->missiles.size(); j++)
		{
			if (missileEnemies.at(i)->missiles.at(j) != nullptr
				&& ExpireMissiles(_player, *(missileEnemies.at(i)->missiles.at(j))) == true)
			{
				missileEnemies.at(i)->missiles.erase(missileEnemies.at(i)->missiles.begin() + j);
			}
		}
	}
}

bool PlayStage::ExpireMissiles(Player & _player, Missile& _missile)
{
	float endPosX = _player.GetSprite()->Position().x - 1.0f *Width;
	float missilePosX = _missile.GetSprite()->Position().x;

	if (missilePosX < endPosX)
	{
		return true;
	}

	return false;
}

/****************************************************************************************************/ // Enemy vs Bullets

/*
	목적:
		Bullet과 Enemy간의 물리적 충돌을 감지하고 Enemy에 피해량을 적용합니다.
	설명:
		Bullet과 Enemy의 Aabb 충돌을 통해 충돌 여부를 검사하고,
		충돌했다면, Bullet의 Attack Level 만큼 Enemy에게 Damage를 입힙니다.
*/

void PlayStage::EnemyBulletCollisionCheck(Player& _player)
{
	bool Check = false;
	int damage = 0;

	// 지상 적 1번, Spin Enemy
	for (int i = 0; i < spinEnemies.size(); i++)
	{
		if (fabs(_player.GetSprite()->Position().x - spinEnemies.at(i)->GetSprite()->Position().x) <= Width * 0.6f)
		{
			// vs Level 1 Bullet 
			for (int j = 0; j < level1Bullets.size(); j++)
			{

				if (Sprite::Aabb(spinEnemies.at(i)->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
					&& level1Bullets.at(j)->BRender() == true
					&& level1Bullets.at(j)->BAttack() == false)
				{
					level1Bullets.at(j)->SetEnemyHealthLevel(spinEnemies.at(i)->HealthLevel());
					level1Bullets.at(j)->HandleDestruction();

					int damage = level1Bullets.at(j)->AttackLevel();
					spinEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 2 Bullet 
			for (int j = 0; j < level2Bullets.size(); j++)
			{
				if (Sprite::Aabb(spinEnemies.at(i)->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
					&& level2Bullets.at(j)->BRender() == true
					&& level2Bullets.at(j)->BAttack() == false)
				{
					level2Bullets.at(j)->SetEnemyHealthLevel(spinEnemies.at(i)->HealthLevel());
					level2Bullets.at(j)->HandleDestruction();

					int damage = level2Bullets.at(j)->AttackLevel();
					spinEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 3 Bullet 
			for (int j = 0; j < level3Bullets.size(); j++)
			{
				if (Sprite::Aabb(spinEnemies.at(i)->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
					&& level3Bullets.at(j)->BRender() == true
					&& level3Bullets.at(j)->BAttack() == false)
				{
					level3Bullets.at(j)->SetEnemyHealthLevel(spinEnemies.at(i)->HealthLevel());
					level3Bullets.at(j)->HandleDestruction();

					int damage = level3Bullets.at(j)->AttackLevel();
					spinEnemies.at(i)->HandleDamage(damage);
				}
			}
		}
	}
	// 지상 적 2번, Missile Enemy
	for (int i = 0; i < missileEnemies.size(); i++)
	{
		if (fabs(_player.GetSprite()->Position().x - missileEnemies.at(i)->GetSprite()->Position().x) <= Width * 0.6f)
		{
			// vs Level 1 Bullet 
			for (int j = 0; j < level1Bullets.size(); j++)
			{
				if (Sprite::Aabb(missileEnemies.at(i)->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
					&& level1Bullets.at(j)->BRender() == true
					&& level1Bullets.at(j)->BAttack() == false)
				{
					level1Bullets.at(j)->SetEnemyHealthLevel(missileEnemies.at(i)->HealthLevel());
					level1Bullets.at(j)->HandleDestruction();

					int damage = level1Bullets.at(j)->AttackLevel();
					missileEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 2 Bullet 
			for (int j = 0; j < level2Bullets.size(); j++)
			{
				if (Sprite::Aabb(missileEnemies.at(i)->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
					&& level2Bullets.at(j)->BRender() == true
					&& level2Bullets.at(j)->BAttack() == false)
				{
					level2Bullets.at(j)->SetEnemyHealthLevel(missileEnemies.at(i)->HealthLevel());
					level2Bullets.at(j)->HandleDestruction();

					int damage = level2Bullets.at(j)->AttackLevel();
					missileEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 3 Bullet 
			for (int j = 0; j < level3Bullets.size(); j++)
			{
				if (Sprite::Aabb(missileEnemies.at(i)->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
					&& level3Bullets.at(j)->BRender() == true
					&& level3Bullets.at(j)->BAttack() == false)
				{
					level3Bullets.at(j)->SetEnemyHealthLevel(missileEnemies.at(i)->HealthLevel());
					level3Bullets.at(j)->HandleDestruction();

					int damage = level3Bullets.at(j)->AttackLevel();
					missileEnemies.at(i)->HandleDamage(damage);
				}
			}
		}
	}
	// 공중 적 1번, LandMine Enemy
	for (int i = 0; i < landMineEnemies.size(); i++)
	{
		// 1. Enemy와 충돌
		if (fabs(_player.GetSprite()->Position().x - landMineEnemies.at(i)->GetSprite()->Position().x) <= Width * 0.6f)
		{
			// vs Level 1 Bullet 
			for (int j = 0; j < level1Bullets.size(); j++)
			{
				if (level1Bullets.at(j) != nullptr)
				{
					if (Sprite::Aabb(landMineEnemies.at(i)->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
						&& level1Bullets.at(j)->BRender() == true
						&& level1Bullets.at(j)->BAttack() == false)
					{
						level1Bullets.at(j)->SetEnemyHealthLevel(landMineEnemies.at(i)->HealthLevel());
						level1Bullets.at(j)->HandleDestruction();

						int damage = level1Bullets.at(j)->AttackLevel();
						landMineEnemies.at(i)->HandleDamage(damage);
					}
				}
			}
			// vs Level 2 Bullet 
			for (int j = 0; j < level2Bullets.size(); j++)
			{
				if (Sprite::Aabb(landMineEnemies.at(i)->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
					&& level2Bullets.at(j)->BRender() == true
					&& level2Bullets.at(j)->BAttack() == false)
				{
					level2Bullets.at(j)->SetEnemyHealthLevel(landMineEnemies.at(i)->HealthLevel());
					level2Bullets.at(j)->HandleDestruction();

					int damage = level2Bullets.at(j)->AttackLevel();
					landMineEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 3 Bullet 
			for (int j = 0; j < level3Bullets.size(); j++)
			{
				if (Sprite::Aabb(landMineEnemies.at(i)->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
					&& level3Bullets.at(j)->BRender() == true
					&& level3Bullets.at(j)->BAttack() == false)
				{
					level3Bullets.at(j)->SetEnemyHealthLevel(landMineEnemies.at(i)->HealthLevel());
					level3Bullets.at(j)->HandleDestruction();

					int damage = level3Bullets.at(j)->AttackLevel();
					landMineEnemies.at(i)->HandleDamage(damage);
				}
			}
		}

		// 2. Weapon과 충돌
		if (landMineEnemies.at(i)->landMine->StateNumber() == 1)
		{
			if (fabs(_player.GetSprite()->Position().x - landMineEnemies.at(i)->landMine->GetSprite()->Position().x) <= Width * 0.6f)
			{
				// vs Level 1 Bullet 
				for (int j = 0; j < level1Bullets.size(); j++)
				{
					if (level1Bullets.at(j) != nullptr)
					{
						if (Sprite::Aabb(landMineEnemies.at(i)->landMine->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
							&& level1Bullets.at(j)->BAttack() == false)
						{
							level1Bullets.at(j)->SetEnemyHealthLevel(landMineEnemies.at(i)->landMine->HealthLevel());
							level1Bullets.at(j)->HandleDestruction();

							int damage = level1Bullets.at(j)->AttackLevel();
							landMineEnemies.at(i)->landMine->HandleDamage(damage);
						}
					}
				}
				// vs Level 2 Bullet 
				for (int j = 0; j < level2Bullets.size(); j++)
				{
					if (Sprite::Aabb(landMineEnemies.at(i)->landMine->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
						&& level2Bullets.at(j)->BAttack() == false)
					{
						level2Bullets.at(j)->SetEnemyHealthLevel(landMineEnemies.at(i)->landMine->HealthLevel());
						level2Bullets.at(j)->HandleDestruction();

						int damage = level2Bullets.at(j)->AttackLevel();
						landMineEnemies.at(i)->landMine->HandleDamage(damage);
					}
				}
				// vs Level 3 Bullet 
				for (int j = 0; j < level3Bullets.size(); j++)
				{
					if (Sprite::Aabb(landMineEnemies.at(i)->landMine->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
						&& level3Bullets.at(j)->BAttack() == false)
					{
						level3Bullets.at(j)->SetEnemyHealthLevel(landMineEnemies.at(i)->landMine->HealthLevel());
						level3Bullets.at(j)->HandleDestruction();

						int damage = level3Bullets.at(j)->AttackLevel();
						landMineEnemies.at(i)->landMine->HandleDamage(damage);
					}
				}
			}
		}

	
	}
	// 공중 적 2번, Bee Enemy
	for (int i = 0; i < beeEnemies.size(); i++)
	{
		if (fabs(_player.GetSprite()->Position().x - beeEnemies.at(i)->GetSprite()->Position().x) <= Width * 0.6f)
		{
			// vs Level 1 Bullet 
			for (int j = 0; j < level1Bullets.size(); j++)
			{
				if (Sprite::Aabb(beeEnemies.at(i)->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
					&& level1Bullets.at(j)->BRender() == true
					&& level1Bullets.at(j)->BAttack() == false)
				{
					level1Bullets.at(j)->SetEnemyHealthLevel(beeEnemies.at(i)->HealthLevel());
					level1Bullets.at(j)->HandleDestruction();

					int damage = level1Bullets.at(j)->AttackLevel();
					beeEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 2 Bullet 
			for (int j = 0; j < level2Bullets.size(); j++)
			{
				if (Sprite::Aabb(beeEnemies.at(i)->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
					&& level2Bullets.at(j)->BRender() == true
					&& level2Bullets.at(j)->BAttack() == false)
				{
					level2Bullets.at(j)->SetEnemyHealthLevel(beeEnemies.at(i)->HealthLevel());
					level2Bullets.at(j)->HandleDestruction();

					int damage = level2Bullets.at(j)->AttackLevel();
					beeEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 3 Bullet 
			for (int j = 0; j < level3Bullets.size(); j++)
			{
				if (Sprite::Aabb(beeEnemies.at(i)->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
					&& level3Bullets.at(j)->BRender() == true
					&& level3Bullets.at(j)->BAttack() == false)
				{
					level3Bullets.at(j)->SetEnemyHealthLevel(beeEnemies.at(i)->HealthLevel());
					level3Bullets.at(j)->HandleDestruction();

					int damage = level3Bullets.at(j)->AttackLevel();
					beeEnemies.at(i)->HandleDamage(damage);
				}
			}
		}
	}
	// 공중 적 3번, Bomb Enemy
	for (int i = 0; i < bombEnemies.size(); i++)
	{
		if (fabs(_player.GetSprite()->Position().x - bombEnemies.at(i)->GetSprite()->Position().x) <= Width * 0.6f)
		{
			// vs Level 1 Bullet 
			for (int j = 0; j < level1Bullets.size(); j++)
			{
				if (Sprite::Aabb(bombEnemies.at(i)->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
					&& level1Bullets.at(j)->BRender() == true
					&& level1Bullets.at(j)->BAttack() == false)
				{
					level1Bullets.at(j)->SetEnemyHealthLevel(bombEnemies.at(i)->HealthLevel());
					level1Bullets.at(j)->HandleDestruction();

					int damage = level1Bullets.at(j)->AttackLevel();
					bombEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 2 Bullet 
			for (int j = 0; j < level2Bullets.size(); j++)
			{
				if (Sprite::Aabb(bombEnemies.at(i)->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
					&& level2Bullets.at(j)->BRender() == true
					&& level2Bullets.at(j)->BAttack() == false)
				{
					level2Bullets.at(j)->SetEnemyHealthLevel(bombEnemies.at(i)->HealthLevel());
					level2Bullets.at(j)->HandleDestruction();

					int damage = level2Bullets.at(j)->AttackLevel();
					bombEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 3 Bullet 
			for (int j = 0; j < level3Bullets.size(); j++)
			{
				if (Sprite::Aabb(bombEnemies.at(i)->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
					&& level3Bullets.at(j)->BRender() == true
					&& level3Bullets.at(j)->BAttack() == false)
				{
					level3Bullets.at(j)->SetEnemyHealthLevel(bombEnemies.at(i)->HealthLevel());
					level3Bullets.at(j)->HandleDestruction();

					int damage = level3Bullets.at(j)->AttackLevel();
					bombEnemies.at(i)->HandleDamage(damage);
				}
			}
		}
	}
	// 지상 적 3번, Car Enemy
	for (int i = 0; i < carEnemies.size(); i++)
	{
		if (fabs(_player.GetSprite()->Position().x - carEnemies.at(i)->GetSprite()->Position().x) <= Width * 0.6f)
		{
			// vs Level 1 Bullet 
			for (int j = 0; j < level1Bullets.size(); j++)
			{
				if (Sprite::Aabb(carEnemies.at(i)->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
					&& level1Bullets.at(j)->BRender() == true
					&& level1Bullets.at(j)->BAttack() == false)
				{
					level1Bullets.at(j)->SetEnemyHealthLevel(carEnemies.at(i)->HealthLevel());
					level1Bullets.at(j)->HandleDestruction();

					int damage = level1Bullets.at(j)->AttackLevel();
					carEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 2 Bullet 
			for (int j = 0; j < level2Bullets.size(); j++)
			{
				if (Sprite::Aabb(carEnemies.at(i)->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
					&& level2Bullets.at(j)->BRender() == true
					&& level2Bullets.at(j)->BAttack() == false)
				{
					level2Bullets.at(j)->SetEnemyHealthLevel(carEnemies.at(i)->HealthLevel());
					level2Bullets.at(j)->HandleDestruction();

					int damage = level2Bullets.at(j)->AttackLevel();
					carEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 3 Bullet 
			for (int j = 0; j < level3Bullets.size(); j++)
			{
				if (Sprite::Aabb(carEnemies.at(i)->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
					&& level3Bullets.at(j)->BRender() == true
					&& level3Bullets.at(j)->BAttack() == false)
				{
					level3Bullets.at(j)->SetEnemyHealthLevel(carEnemies.at(i)->HealthLevel());
					level3Bullets.at(j)->HandleDestruction();

					int damage = level3Bullets.at(j)->AttackLevel();
					carEnemies.at(i)->HandleDamage(damage);
				}
			}
		}
	}
	// 중간 보스
	for (int i = 0; i < subBossEnemies.size(); i++)
	{
		if (fabs(_player.GetSprite()->Position().x - subBossEnemies.at(i)->GetSprite()->Position().x) <= Width * 0.6f)
		{
			// vs Level 1 Bullet 
			for (int j = 0; j < level1Bullets.size(); j++)
			{
				if (Sprite::Aabb(subBossEnemies.at(i)->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
					&& subBossEnemies.at(i)->BDead() == false
					&& level1Bullets.at(j)->BRender() == true)
				{
					level1Bullets.at(j)->SetEnemyHealthLevel(subBossEnemies.at(i)->HealthLevel());
					level1Bullets.at(j)->HandleDestruction();

					int damage = level1Bullets.at(j)->AttackLevel();
					subBossEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 2 Bullet 
			for (int j = 0; j < level2Bullets.size(); j++)
			{
				if (Sprite::Aabb(subBossEnemies.at(i)->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
					&& subBossEnemies.at(i)->BDead() == false
					&& level2Bullets.at(j)->BRender() == true)
				{
					level2Bullets.at(j)->SetEnemyHealthLevel(subBossEnemies.at(i)->HealthLevel());
					level2Bullets.at(j)->HandleDestruction();

					int damage = level2Bullets.at(j)->AttackLevel();
					subBossEnemies.at(i)->HandleDamage(damage);
				}
			}
			// vs Level 3 Bullet 
			for (int j = 0; j < level3Bullets.size(); j++)
			{
				if (Sprite::Aabb(subBossEnemies.at(i)->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
					&& subBossEnemies.at(i)->BDead() == false
					&& level3Bullets.at(j)->BRender() == true
					&& level3Bullets.at(j)->BAttack() == false)
				{
					level3Bullets.at(j)->SetEnemyHealthLevel(subBossEnemies.at(i)->HealthLevel());
					level3Bullets.at(j)->HandleDestruction();

					int damage = level3Bullets.at(j)->AttackLevel();
					subBossEnemies.at(i)->HandleDamage(damage);
				}
			}
		}
	}
	// 중간 보스 1번의 병사
	if (subBossEnemies.at(0)->BDead() == false)
	{
		for (int i = 0; i < subBossEnemies.at(0)->subBossPawns.size(); i++)
		{
			for (int k = 0; k < subBossEnemies.at(0)->subBossPawns.size(); k++)
			{
				if (subBossEnemies.at(0)->subBossPawns.at(k) != nullptr)
				{
					if (fabs(_player.GetSprite()->Position().x - subBossEnemies.at(0)->subBossPawns.at(k)->GetSprite()->Position().x) <= Width * 0.6f)
					{
						// vs Level 1 Bullet 
						for (int j = 0; j < level1Bullets.size(); j++)
						{
							if (Sprite::Aabb(subBossEnemies.at(0)->subBossPawns.at(k)->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
								&& level1Bullets.at(j)->BRender() == true
								&& level1Bullets.at(j)->BAttack() == false)
							{
								level1Bullets.at(j)->SetEnemyHealthLevel(subBossEnemies.at(0)->subBossPawns.at(k)->HealthLevel());
								level1Bullets.at(j)->HandleDestruction();

								int damage = level1Bullets.at(j)->AttackLevel();
								subBossEnemies.at(0)->subBossPawns.at(k)->HandleDamage(damage);
							}
						}
						// vs Level 2 Bullet 
						for (int j = 0; j < level2Bullets.size(); j++)
						{
							if (Sprite::Aabb(subBossEnemies.at(0)->subBossPawns.at(k)->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
								&& level2Bullets.at(j)->BRender() == true
								&& level2Bullets.at(j)->BAttack() == false)
							{
								level2Bullets.at(j)->SetEnemyHealthLevel(subBossEnemies.at(0)->subBossPawns.at(k)->HealthLevel());
								level2Bullets.at(j)->HandleDestruction();

								int damage = level2Bullets.at(j)->AttackLevel();
								subBossEnemies.at(0)->subBossPawns.at(k)->HandleDamage(damage);
							}
						}
						// vs Level 3 Bullet 
						for (int j = 0; j < level3Bullets.size(); j++)
						{
							if (Sprite::Aabb(subBossEnemies.at(0)->subBossPawns.at(k)->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
								&& level3Bullets.at(j)->BRender() == true
								&& level3Bullets.at(j)->BAttack() == false)
							{
								level3Bullets.at(j)->SetEnemyHealthLevel(subBossEnemies.at(0)->subBossPawns.at(k)->HealthLevel());
								level3Bullets.at(j)->HandleDestruction();

								int damage = level3Bullets.at(j)->AttackLevel();
								subBossEnemies.at(0)->subBossPawns.at(k)->HandleDamage(damage);
							}
						}
					}
				}
			}
		}
	}
	// 중간 보스 2번의 병사
	if (subBossEnemies.at(1)->BDead() == false)
	{
		for (int i = 0; i < subBossEnemies.at(1)->subBossPawns.size(); i++)
		{
			for (int k = 0; k < subBossEnemies.at(1)->subBossPawns.size(); k++)
			{
				if (subBossEnemies.at(1)->subBossPawns.at(k) != nullptr)
				{
					if (fabs(_player.GetSprite()->Position().x - subBossEnemies.at(1)->subBossPawns.at(k)->GetSprite()->Position().x) <= Width * 0.6f)
					{
						// vs Level 1 Bullet 
						for (int j = 0; j < level1Bullets.size(); j++)
						{
							if (Sprite::Aabb(subBossEnemies.at(1)->subBossPawns.at(k)->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
								&& level1Bullets.at(j)->BRender() == true
								&& level1Bullets.at(j)->BAttack() == false)
							{
								level1Bullets.at(j)->SetEnemyHealthLevel(subBossEnemies.at(1)->subBossPawns.at(k)->HealthLevel());
								level1Bullets.at(j)->HandleDestruction();

								int damage = level1Bullets.at(j)->AttackLevel();
								subBossEnemies.at(1)->subBossPawns.at(k)->HandleDamage(damage);
							}
						}
						// vs Level 2 Bullet 
						for (int j = 0; j < level2Bullets.size(); j++)
						{
							if (Sprite::Aabb(subBossEnemies.at(1)->subBossPawns.at(k)->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
								&& level2Bullets.at(j)->BRender() == true
								&& level2Bullets.at(j)->BAttack() == false)
							{
								level2Bullets.at(j)->SetEnemyHealthLevel(subBossEnemies.at(1)->subBossPawns.at(k)->HealthLevel());
								level2Bullets.at(j)->HandleDestruction();

								int damage = level2Bullets.at(j)->AttackLevel();
								subBossEnemies.at(1)->subBossPawns.at(k)->HandleDamage(damage);
							}
						}
						// vs Level 3 Bullet 
						for (int j = 0; j < level3Bullets.size(); j++)
						{
							if (Sprite::Aabb(subBossEnemies.at(1)->subBossPawns.at(k)->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
								&& level3Bullets.at(j)->BRender() == true
								&& level3Bullets.at(j)->BAttack() == false)
							{
								level3Bullets.at(j)->SetEnemyHealthLevel(subBossEnemies.at(1)->subBossPawns.at(k)->HealthLevel());
								level3Bullets.at(j)->HandleDestruction();

								int damage = level3Bullets.at(j)->AttackLevel();
								subBossEnemies.at(1)->subBossPawns.at(k)->HandleDamage(damage);
							}
						}
					}
				}
			}
		}
	}
	// 최종 보스
	{
		if (fabs(_player.GetSprite()->Position().x - bossEnemy->GetSprite()->Position().x) <= Width * 0.6f)
		{
			// vs Level 1 Bullet 
			for (int j = 0; j < level1Bullets.size(); j++)
			{

				if (Sprite::Aabb(bossEnemy->GetSprite(), level1Bullets.at(j)->GetSprite()) == true
					&& level1Bullets.at(j)->BRender() == true
					&& level1Bullets.at(j)->BAttack() == false)
				{
					level1Bullets.at(j)->SetEnemyHealthLevel(bossEnemy->HealthLevel());
					level1Bullets.at(j)->HandleDestruction();

					int damage = level1Bullets.at(j)->AttackLevel();
					bossEnemy->HandleDamage(damage);
				}
			}
			// vs Level 2 Bullet 
			for (int j = 0; j < level2Bullets.size(); j++)
			{
				if (Sprite::Aabb(bossEnemy->GetSprite(), level2Bullets.at(j)->GetSprite()) == true
					&& level2Bullets.at(j)->BRender() == true
					&& level2Bullets.at(j)->BAttack() == false)
				{
					level2Bullets.at(j)->SetEnemyHealthLevel(bossEnemy->HealthLevel());
					level2Bullets.at(j)->HandleDestruction();

					int damage = level2Bullets.at(j)->AttackLevel();
					bossEnemy->HandleDamage(damage);
				}
			}
			// vs Level 3 Bullet 
			for (int j = 0; j < level3Bullets.size(); j++)
			{
				if (Sprite::Aabb(bossEnemy->GetSprite(), level3Bullets.at(j)->GetSprite()) == true
					&& level3Bullets.at(j)->BRender() == true
					&& level3Bullets.at(j)->BAttack() == false)
				{
					level3Bullets.at(j)->SetEnemyHealthLevel(bossEnemy->HealthLevel());
					level3Bullets.at(j)->HandleDestruction();

					int damage = level3Bullets.at(j)->AttackLevel();
					bossEnemy->HandleDamage(damage);
				}
			}
		}
	}
}

/****************************************************************************************************/ // Enemy vs Partiable Bridge 

/*
	목적:
		Sub Boss와 Partiable Bridge의 충돌 여부를 검사합니다.
*/

void PlayStage::EnemyPartiableBridgeCollisionCheck()
{
	// 1. 첫 번째 서브 보스
	if (subBossEnemies.at(0)->BDead() == true)
	{
		if (Sprite::Aabb(subBossEnemies.at(0)->GetSprite(), partiableBridges.at(0)->GetSprite()) == true)
		{
			float yScale = subBossEnemies.at(0)->GetSprite()->Scale().y * subBossEnemies.at(0)->GetSprite()->TextureSize().y * 0.5f;
			float bridgeYscale = partiableBridges.at(0)->GetSprite()->Scale().y * partiableBridges.at(0)->GetSprite()->TextureSize().y * 0.5f;
			float groundPos = 0.f;

			if (partiableBridges.at(0)->StateNumber() == 0)
				groundPos = partiableBridges.at(0)->Position().y + bridgeYscale + yScale - 30.f;
			else
				groundPos = partiableBridges.at(0)->Position().y + bridgeYscale + yScale - 9.f;;

			subBossEnemies.at(0)->SetGroundPosition(groundPos);
			subBossEnemies.at(0)->GetSprite()->DrawCollision(true);

			partiableBridges.at(0)->SetBCollideWithSubBoss(true);
			subBossEnemies.at(0)->SetBCollideWithPartiableBridge(true);
		}
		else
		{
			subBossEnemies.at(0)->SetGroundPosition(-1000.f);
			partiableBridges.at(0)->SetBCollideWithSubBoss(false);
		}
	}

	// 2. 두 번째 서브 보스
	if (subBossEnemies.at(1)->BDead() == true)
	{
		if (Sprite::Aabb(subBossEnemies.at(1)->GetSprite(), partiableBridges.at(1)->GetSprite()) == true)
		{
			float yScale = subBossEnemies.at(1)->GetSprite()->Scale().y * subBossEnemies.at(1)->GetSprite()->TextureSize().y * 0.5f;
			float bridgeYscale = partiableBridges.at(1)->GetSprite()->Scale().y * partiableBridges.at(1)->GetSprite()->TextureSize().y * 0.5f;
			float groundPos = 0.f;

			if (partiableBridges.at(1)->StateNumber() == 0)
				groundPos = partiableBridges.at(1)->Position().y + bridgeYscale + yScale - 30.f;
			else
				groundPos = partiableBridges.at(1)->Position().y + bridgeYscale + yScale - 9.f;;

			subBossEnemies.at(1)->SetGroundPosition(groundPos);
			subBossEnemies.at(1)->GetSprite()->DrawCollision(true);

			partiableBridges.at(1)->SetBCollideWithSubBoss(true);
			subBossEnemies.at(1)->SetBCollideWithPartiableBridge(true);
		}
		else
		{
			subBossEnemies.at(1)->SetGroundPosition(-1000.f);
			partiableBridges.at(1)->SetBCollideWithSubBoss(false);
		}
	}

	// 3. 첫 번째 서브 보스 병사들
	for (int i = 0; i < subBossEnemies.at(0)->subBossPawns.size(); i++)
	{
		if (Sprite::Aabb(subBossEnemies.at(0)->subBossPawns.at(i)->GetSprite(),
			partiableBridges.at(0)->GetSprite()) == true)
		{
			float yScale = subBossEnemies.at(0)->subBossPawns.at(i)->GetSprite()->Scale().y * subBossEnemies.at(0)->subBossPawns.at(i)->GetSprite()->TextureSize().y * 0.5f;
			float bridgeYscale = partiableBridges.at(0)->GetSprite()->Scale().y * partiableBridges.at(0)->GetSprite()->TextureSize().y * 0.5f;
			float groundPos = 0.f;


			if (subBossEnemies.at(0)->subBossPawns.at(i)->StateNumber() <= 2)
			{
				groundPos = partiableBridges.at(0)->Position().y + bridgeYscale + yScale - 65.f;
			}
			else
			{
				groundPos = partiableBridges.at(0)->Position().y + bridgeYscale + yScale - 28.f;
			}

			subBossEnemies.at(0)->subBossPawns.at(i)->SetGroundPosition(groundPos);
			subBossEnemies.at(0)->subBossPawns.at(i)->GetSprite()->DrawCollision(true);
		}
	}

	// 4. 두 번째 서브 보스 병사들
	for (int i = 0; i < subBossEnemies.at(1)->subBossPawns.size(); i++)
	{
		if (Sprite::Aabb(subBossEnemies.at(1)->subBossPawns.at(i)->GetSprite(),
			partiableBridges.at(1)->GetSprite()) == true)
		{
			float yScale = subBossEnemies.at(1)->subBossPawns.at(i)->GetSprite()->Scale().y * subBossEnemies.at(1)->subBossPawns.at(i)->GetSprite()->TextureSize().y * 0.5f;
			float bridgeYscale = partiableBridges.at(1)->GetSprite()->Scale().y * partiableBridges.at(1)->GetSprite()->TextureSize().y * 0.5f;
			float groundPos = 0.f;


			if (subBossEnemies.at(1)->subBossPawns.at(i)->StateNumber() <= 2)
			{
				groundPos = partiableBridges.at(1)->Position().y + bridgeYscale + yScale - 65.f;
			}
			else
			{
				groundPos = partiableBridges.at(1)->Position().y + bridgeYscale + yScale - 28.f;
			}

			subBossEnemies.at(1)->subBossPawns.at(i)->SetGroundPosition(groundPos);
			subBossEnemies.at(1)->subBossPawns.at(i)->GetSprite()->DrawCollision(true);
		}
	}
}

/****************************************************************************************************/ // Weapon vs Line

/*
	목적:
		Weapon과 Line간의 충돌을 감지하고, Weapon의 위치를 조정합니다.
*/

void PlayStage::WeaponLineCollisionCheck()
{
	std::string weaponName = "";
	for (int i = 0; i < lines.size(); i++)
	{
		// 1. Landmine
		for (int j = 0; j < landMineEnemies.size(); j++)
		{
			weaponName = "LandMine";
			// Spin Enemy vs Line - Vertical Check
			if (WeaponLineVerticalCollisionCheck(weaponName, j, *lines.at(i)) == true)
			{
				// Spin Enemy vs Line - Horizontal Check
				WeaponLineHorizontalCollisionCheck(weaponName, j, *lines.at(i));
			}
		}
	}
}

bool PlayStage::WeaponLineVerticalCollisionCheck(std::string & _weapon, int index, Line & line)
{
	// 1. Landmine
	if (_weapon == "LandMine")
	{
		float xScale = landMineEnemies.at(index)->landMine->GetSprite()->Scale().x * landMineEnemies.at(index)->landMine->GetSprite()->TextureSize().x * 0.5f;
		float yScale = landMineEnemies.at(index)->landMine->GetSprite()->Scale().y * landMineEnemies.at(index)->landMine->GetSprite()->TextureSize().y * 0.5f;

		float weaponLeft = landMineEnemies.at(index)->landMine->GetSprite()->Position().x - xScale;
		float weaponRight = landMineEnemies.at(index)->landMine->GetSprite()->Position().x + xScale;

		if (weaponLeft < line.getEndPos().x &&
			weaponRight > line.getStartPos().x)
		{
			return true;
		}

		return false;
	}
}

void PlayStage::WeaponLineHorizontalCollisionCheck(std::string & _weapon, int index, Line & line)
{
	bool Check = true;

	// _weaponName == "Missile Eenmy"
	if (_weapon== "MissileEnemy")
	{
		float xScale = landMineEnemies.at(index)->landMine->GetSprite()->Scale().x * landMineEnemies.at(index)->landMine->GetSprite()->TextureSize().x * 0.5f;
		float yScale = landMineEnemies.at(index)->landMine->GetSprite()->Scale().y * landMineEnemies.at(index)->landMine->GetSprite()->TextureSize().y * 0.5f;

		float weaponCenterX = landMineEnemies.at(index)->landMine->GetSprite()->Position().x;
		float weaponCenterY = landMineEnemies.at(index)->landMine->GetSprite()->Position().y;


		if (PToPDistance(D3DXVECTOR2(weaponCenterX, weaponCenterY), D3DXVECTOR2(weaponCenterX, LineFunction(line, weaponCenterX)))
			<= yScale)
		{
			Check &= true;
		}
		else
		{
			Check &= false;
		}
		Check &= weaponCenterY > LineFunction(line, weaponCenterX);
	}

	AdjustWeaponPosition(Check, _weapon, index, line);

}

void PlayStage::AdjustWeaponPosition(bool bCollide, std::string & _weapon, int index, Line & line)
{
	// _weapon == "LandMine"
	if (_weapon == "LandMine")
	{
		float xScale = landMineEnemies.at(index)->landMine->GetSprite()->Scale().x * landMineEnemies.at(index)->landMine->GetSprite()->TextureSize().x * 0.5f;
		float yScale = landMineEnemies.at(index)->landMine->GetSprite()->Scale().y * landMineEnemies.at(index)->landMine->GetSprite()->TextureSize().y * 0.5f;

		float weaponCenterX = landMineEnemies.at(index)->landMine->GetSprite()->Position().x;

		float groundPos = LineFunction(line, weaponCenterX) + yScale - 54.f;

		if (bCollide == true)
		{
			landMineEnemies.at(index)->landMine->GetSprite()->DrawCollision(true);
			landMineEnemies.at(index)->landMine->SetGroundPosition(groundPos);

			//RemakeLines(index, line);
		}
	}
}
