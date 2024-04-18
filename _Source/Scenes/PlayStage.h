#pragma once

#include "Objects/Background.h"
#include "Scene.h"

class PlayStage : public Scene
{
public:
	PlayStage(SceneValues* values);
	~PlayStage();

	void Update() override;
	void Render() override;

	void CameraProjection();

public:

	void CentralizeMousePos();

	/*****************************************************************/ // Options

public:

	float LineFunction(class Line& line, float input);
	float PToPDistance(D3DXVECTOR2& start, D3DXVECTOR2& end);
	void PlayerBOutRange(class Player& player);

	/*****************************************************************/ // Bg Objects

public:

	void ExpireCars();

	/*****************************************************************/ // Line

public:

	void LinePointsSetting();
	void SetLinePointsPairs(pair<D3DXVECTOR2, D3DXVECTOR2> linePos);
	
	/*****************************************************************/ // Wall

public:

	void WallPointsSetting();
	void SetWallPointsPairs(pair<D3DXVECTOR2, D3DXVECTOR2> linePos);

	/*****************************************************************/ // Player vs Line
public:

	void PlayerLineCollisionCheck(class Player& player);
	void PlayerLineHorizontalCollisionCheck(class Player& player, class Line& line);
	bool PlayerLineVerticalCollisionCheck(class Player& player, class Line& line);
	void AdjustPlayerPosition(bool bCollide, class Player& player, class Line& line);

	/*****************************************************************/ // Player vs Wall

public:

	void PlayerWallCollisionCheck(class Player& player);
	bool PlayerWallHorizontalCollisionCheck(class Player& player, class Line& line);
	void PlayerWallVerticalCollisionCheck(class Player& player, class Line& line);
	void AdjustPlayerWallPosition(bool bCollide, class Player& player, class Line& line);

	/*****************************************************************/ // Player vs Partiable Bridge

public:

	void PlayerPartialBridgeCollisionCheck(class Player& player, class PartiableBridge& bridge);
	void PlayerSmallPartialBridgeCollisionCheck(class Player& player, class SmallPartiableBridge& bridge);

	/*****************************************************************/ // Player vs Bullet

	void PlayerSpawnBullets(Player& _player);

	/*****************************************************************/ // Player vs Enemy

	void PlayerEnemyCollisionCheck(Player& _player);

	void PlayerDeadSubBossCollisionCheck(Player& _player);
	bool PlayerDeadSubBossHorizontalCheck(Player& _player, int _index);
	void PlayerDeadSubBossVerticalCheck(Player& _player, int _index);

	/*****************************************************************/ // Player vs Wweapon

	void PlayerWeaponCollisionCheck(Player& _player);

	/*****************************************************************/ // Player vs Effect

	void PlayerHoleCollisionCheck(Player& _player);

	/*****************************************************************/ // Player vs Camera

	void CheckPlayerMeetSubBoss1(Player& _player);
	void CheckPlayerMeetSubBoss2(Player& _player);
	void CheckPlayerMeetBoss(Player& _player);

	void CheckSubBoss1Dead(class SubBossEnemy& _enemy, Player& _player);
	void CheckSubBoss2Dead(class SubBossEnemy& _enemy, Player& _player);
	void CheckBossDead(class BossEnemy& _enemy, Player& _player);


	/*****************************************************************/ // Bullets

public:
	
	// 최대 범위를 넘어선 Bullet의 파괴
	void CheckBulletsOutOfRange(Player& player);

	bool ExpireLevel1Bullets(class Player& player, class Level1Bullet& bullet);
	bool ExpireLevel2Bullets(class Player& player, class Level2Bullet& bullet);
	bool ExpireLevel3Bullets(class Player& player, class Level3Bullet& bullet);

	void HandleBulletsDestruciton(Player& _player);

	/*****************************************************************/ // Enemies

	/*****************************************************************/ // Line vs Enemies

public:

	void ExpireEnemies();

	void EnemyLineCollisionCheck();
	bool EnemyLineVerticalCollisionCheck(const std::string& _enemyName, int index, Line& line);
	void EnemyLineHorizontalCollisionCheck(const std::string& _enemyName, int index, Line& line);
	void AdjustEnemyPosition(bool bCollide, const std::string& _enemyName, int index, Line& line);

	// Sub Boss Pawn의 경우, 2개의 Index가 필요하므로, 별도의 메서드를 선언합니다.
	bool SubBossPawnLineVerticalCollisionCheck(int _subBossIndex, int _subBossPawnIndex, Line& line);
	void SubBossPawnLineHorizontalCollisionCheck(int _subBossIndex, int _subBossPawnIndex, Line& line);
	void AdjustSubBossPawnPosition(bool bCollide, int _subBossIndex, int _subBossPawnIndex, Line& line);

	/*****************************************************************/ // Enemy Weapons

private:

	void CheckMissilesOutOfRange(Player& _player);
	bool ExpireMissiles(Player& _player, class Missile& _missile);

	/*****************************************************************/ // Bullet vs Enemy

	void EnemyBulletCollisionCheck(Player& _player);

	/*****************************************************************/ // Paritable Bridge vsEnemy

	void EnemyPartiableBridgeCollisionCheck();

	/*****************************************************************/ // Wepaons

	/*****************************************************************/ // Line vs Weapons

	void WeaponLineCollisionCheck();
	bool WeaponLineVerticalCollisionCheck(std::string& _weapon, int index,  Line& line);
	void WeaponLineHorizontalCollisionCheck(std::string& _weapon, int index, Line& line);
	void AdjustWeaponPosition(bool bCollide, std::string& _weapon, int index, Line& line);

	/*****************************************************************/ // Members

private:

	D3DXVECTOR2 finalMousePos;

	/*****************************************************************/ // Bg Objects
private:

	class Background* background;

	vector <class PartiableBridge*> partiableBridges;
	vector <class SmallPartiableBridge*> smallPartiableBridges;

	class StreetLamp* streetLamps;

	vector<class Truck*> trucks;
	vector<class NormalCar*> normalCars;

	/*****************************************************************/ // Player

	class Player* player;
	class Camera* camera;

	/*****************************************************************/ // Line

private:

	vector<pair<D3DXVECTOR2, D3DXVECTOR2>> linePointsPairs;
	vector<class Line*> lines;
	int lineCount;

	/*****************************************************************/ // Wall

private:

	vector<pair<D3DXVECTOR2, D3DXVECTOR2>> wallPointsPairs;
	vector<class Line*> walls;
	/*
		목적:
			Player가 속해 있는 Line의 Number를 true로 설정합니다.
		설명:
			Playr가 어떠한 Line에도 속해 있지 않을 경우,
			Player가 추락합니다. ( Player->SetBFalling(true) )
	*/
	vector<bool> playerOnWallNumbers;
	int wallCount;

	/*****************************************************************/ // Bullet


	const int MaxLevel1BulletNum = 50;
	int Level1BulletNum = 0;
	vector <class Level1Bullet*> level1Bullets;

	const int MaxLevel2BulletNum = 50;
	int Level2BulletNum = 0;
	vector <class Level2Bullet*> level2Bullets;

	const int MaxLevel3BulletNum = 50;
	int Level3BulletNum = 0;
	vector <class Level3Bullet*> level3Bullets;

	/*****************************************************************/ // Enemies

private:

	const int spinEnemyNum = 1;
	vector<class SpinEnemy*> spinEnemies;

	const int missileEnemyNum = 4;
	vector <class MissileEnemy* > missileEnemies;

	const int landMineEnemyNum = 7;
	vector <class LandMineEnemy*> landMineEnemies;

	const int beeEnemyNum = 6;
	vector <class BeeEnemy*> beeEnemies;

	const int bombEnemyNum = 5;
	vector <class SuicideBombEnemy*> bombEnemies;

	const int carEnemyNum = 2;
	vector <class CarEnemy*> carEnemies;

	const int subBossEnemyNum = 2;
	vector <class SubBossEnemy*> subBossEnemies;

	const int bossEnemyNum = 1;
	class BossEnemy* bossEnemy;

	/*****************************************************************/ // Interfaces

private:

	class HealthLevel* healthLevelInterface;

	const float LINE_1_START_POINT_X = -10976.f;
	const float LINE_1_END_POINT_X = -8700.f;
	const float LINE_1_POS_Y = -30.f;

	const float LINE_2_START_POINT_X = -8606.f;
	const float LINE_2_END_POINT_X = -7216.f;
	const float LINE_2_POS_Y = 65.f;

	const float LINE_3_START_POINT_X = -7049.f;
	const float LINE_3_END_POINT_X = -4120.f;
	const float LINE_3_POS_Y = 21.f;

	const float LINE_4_1_START_POINT_X = -3941.f;
	const float LINE_4_1_END_POINT_X = -3467.f;

	const float LINE_4_2_START_POINT_X = -2740.f;
	const float LINE_4_2_END_POINT_X = -2450.f;

	const float LINE_4_POS_Y = 8.f;

	const float LINE_5_1_START_POINT_X = -2250.f;
	const float LINE_5_1_END_POINT_X = -1540.f;

	const float LINE_5_2_START_POINT_X = -820.f;
	const float LINE_5_2_END_POINT_X = -13.f;

	const float LINE_5_POS_Y = -85.f;

	const float LINE_6_START_POINT_X = 231.f;
	const float LINE_6_END_POINT_X = 577.f;
	const float LINE_6_START_POS_Y = -70.f;
	const float LINE_6_END_POS_Y = 15.f;

	const float LINE_7_START_POINT_X = 577.f + 30.f;
	const float LINE_7_END_POINT_X = 803.f - 30.f;
	const float LINE_7_POS_Y = 20.f;

	const float LINE_8_START_POINT_X = 1071.f;
	const float LINE_8_END_POINT_X = 1543.f;
	const float LINE_8_POS_Y = 20.f;

	const float LINE_9_START_POINT_X = 1855.f;
	const float LINE_9_END_POINT_X = 1925.f;
	const float LINE_9_POS_Y = 114.f;

	const float LINE_10_START_POINT_X = 2243.f;
	const float LINE_10_END_POINT_X = 2300.f;
	const float LINE_10_POS_Y = 114.f;

	const float LINE_11_START_POINT_X = 2630.f;
	const float LINE_11_END_POINT_X = 2690.f;
	const float LINE_11_POS_Y = 114.f;

	const float LINE_12_START_POINT_X = 2997.f;
	const float LINE_12_END_POINT_X = 3081.f;
	const float LINE_12_POS_Y = 114.f;

	const float LINE_13_START_POINT_X = 3390.f;
	const float LINE_13_END_POINT_X = 3455.f;
	const float LINE_13_POS_Y = 20.f;

	const float LINE_14_START_POINT_X = 3500.f;
	const float LINE_14_END_POINT_X = 3857.f;
	const float LINE_14_START_POS_Y = 20.f;
	const float LINE_14_END_POS_Y = -74.f;

	const float LINE_15_START_POINT_X = 3887.f;
	const float LINE_15_END_POINT_X = 4759.f;
	const float LINE_15_POS_Y = -74.f;

	const float LINE_16_START_POINT_X = 4923.f;
	const float LINE_16_END_POINT_X = 5764.f;
	const float LINE_16_POS_Y = 20.f;

	const float LINE_17_START_POINT_X = 6028.f;
	const float LINE_17_END_POINT_X = 7353.f;
	const float LINE_17_POS_Y = -74.f;

	const float LINE_18_START_POINT_X = 7611.f;
	const float LINE_18_END_POINT_X = 11890.f;
	const float LINE_18_POS_Y = -215.f;

	const float WALL_1_Y = 300.f;
	const float WALL_2_Y = -175.f;
	const float WALL_3_Y = -80.f;
	const float WALL_4_Y = -80.f;
	const float WALL_5_Y = -120.f;
	const float WALL_6_Y = -120.f;
	const float WALL_7_Y = -120.f;
	const float WALL_8_Y = -120.f;
	const float WALL_9_Y = -215.f;
	const float WALL_10_Y = -325.f;
	const float WALL_11_Y = -215.f;
	const float WALL_12_Y = -325.f;
	const float WALL_13_Y = -325.f;
	const float WALL_14_Y = -325.f;
	const float WALL_15_Y = -325.f;
	const float WALL_16_Y = -325.f;
	const float WALL_17_Y = -325.f;
	const float WALL_18_Y = -325.f;
	const float WALL_19_Y = -325.f;
	const float WALL_20_Y = -325.f;
	const float WALL_21_Y = -325.f;
	const float WALL_22_Y = -325.f;
	const float WALL_23_Y = -325.f;
	const float WALL_24_Y = -215.f;
	const float WALL_25_Y = -325.f;
	const float WALL_26_Y = -325.f;
	const float WALL_27_Y = -215.f;
	const float WALL_28_Y = -215.f;
	const float WALL_29_Y = -215.f;
	const float WALL_30_Y = -215.f;
	const float WALL_31_Y = -215.f;
	const float WALL_32_Y = -215.f;

	const float DROP_WALL_Y = -750.f;

	/*****************************************************************/ // Player Const

	const float START_POINT = -10700.f;

	/*****************************************************************/ // BG Object

	const float CARS_START_POINT_1 = -10000.f;
	const float CARS_START_POINT_2 = -9900.f;

	/*****************************************************************/ // Enemy Const

	// 0. Spin Enemy
	const float SPIN_1_START_POINT_X = -8558.f;
	const float SPIN_1_START_POINT_Y = 0.f;
	const float SPIN_1_TARGET_POINT = -10700.f;

	const float SPIN_2_START_POINT_X = 753.f;
	const float SPIN_2_START_POINT_Y = 50.f;
	const float SPIN_2_TARGET_POINT = 297.f;

	// 1. Missile Enemy
	const float MISSILE_1_START_POINT_X = -8970.f;
	const float MISSILE_1_START_POINT_Y = 30.f;

	const float MISSILE_2_START_POINT_X = -5463.f;
	const float MISSILE_2_START_POINT_Y = 80.f;

	const float MISSILE_3_START_POINT_X = -4636.f;
	const float MISSILE_3_START_POINT_Y = 80.f;

	// 2. LandMine Enemy
	const float LANDMINE_1_START_POINT_MIN_X = -7800.f;
	const float LANDMINE_1_START_POINT_MAX_X = -7700.f;

	const float LANDMINE_1_START_POINT_MIN_Y = 180.f;
	const float LANDMINE_1_START_POINT_MAX_Y = 220.f;

	const float LANDMINE_2_START_POINT_MIN_X = -7400.f;
	const float LANDMINE_2_START_POINT_MAX_X = -7300.f;

	const float LANDMINE_2_START_POINT_MIN_Y = 180.f;
	const float LANDMINE_2_START_POINT_MAX_Y = 220.f;

	const float LANDMINE_3_START_POINT_MIN_X = -6700.f;
	const float LANDMINE_3_START_POINT_MAX_X = -6600.f;

	const float LANDMINE_3_START_POINT_MIN_Y = 180.f;
	const float LANDMINE_3_START_POINT_MAX_Y = 220.f;

	const float LANDMINE_4_START_POINT_MIN_X = -6000.f;
	const float LANDMINE_4_START_POINT_MAX_X = -5900.f;

	const float LANDMINE_4_START_POINT_MIN_Y = 180.f;
	const float LANDMINE_4_START_POINT_MAX_Y = 220.f;

	const float LANDMINE_5_START_POINT_MIN_X = -5600.f;
	const float LANDMINE_5_START_POINT_MAX_X = -5500.f;

	const float LANDMINE_5_START_POINT_MIN_Y = 180.f;
	const float LANDMINE_5_START_POINT_MAX_Y = 220.f;

	const float LANDMINE_6_START_POINT_MIN_X = -5200.f;
	const float LANDMINE_6_START_POINT_MAX_X = -5100.f;

	const float LANDMINE_6_START_POINT_MIN_Y = 180.f;
	const float LANDMINE_6_START_POINT_MAX_Y = 220.f;

	const float LANDMINE_7_START_POINT_MIN_X = -5000.f;
	const float LANDMINE_7_START_POINT_MAX_X = -4900.f;

	const float LANDMINE_7_START_POINT_MIN_Y = 180.f;
	const float LANDMINE_7_START_POINT_MAX_Y = 220.f;

	// 3. Bee Enemy
	const float BEE_1_START_POINT_MIN = 1414.f;
	const float BEE_1_START_POINT_MAX = 1681.f;

	const float BEE_2_START_POINT_MIN = 2558.f;
	const float BEE_2_START_POINT_MAX = 2800.f;

	const float BEE_3_START_POINT_MIN = 2991.f;
	const float BEE_3_START_POINT_MAX = 3591.f;

	// 4. Bomb Enemy
	const float BOMB_1_START_POINT_MIN = 4665.f + 700.f;
	const float BOMB_1_START_POINT_MAX = 4665.f + 1000.f;

	const float BOMB_2_START_POINT_MIN = 5280.f + 700.f;
	const float BOMB_2_START_POINT_MAX = 5280.f + 1000.f;

	const float BOMB_3_START_POINT_MIN = 5625.f + 700.f;
	const float BOMB_3_START_POINT_MAX = 5625.f + 1000.f;

	// 5. Car Enemy
	const float CAR_1_LEFT = 6081.f;
	const float CAR_1_RIGHT = 6802.f;

	const float CAR_2_LEFT = 6682.f;
	const float CAR_2_RIGHT = 7444.f;

	const float CAR_3_LEFT = 7728.f;
	const float CAR_3_RIGHT = 8500.f;

	const float CAR_ENEMY_Y = 100.f;

	// 6. SubBoss Enemy
	const float SUBBOSS_1_X = -2646.f;
	const float SUBBOSS_1_Y = 150.f;
	const float SUBBOSS_1_TARGET = -3488.f;

	const float SUBBOSS_2_X = -750.f;
	const float SUBBOSS_2_Y = 50.f;
	const float SUBBOSS_2_TARGET = -1500.f;

	// 7. Boss Enemy
	const float BOSS_X = 11780.f;
	const float BOSS_Y = -200.f;
	const float BOSS_TARGET = 11150.f;
};