#include <HAPI_lib.h>
#include <HAPI_InputCodes.h>
#include <memory>
#include <random>
#include <iostream>
#include <fstream>
#include <ctime>
#include "World.h"
#include "Vector3.h"
#include "Object.h"
#include "PlayerObject.h"
#include "BulletObject.h"
#include "WallObject.h"
#include "DoorObject.h"
#include "EnemyObject.h"
#include "ChasingEnemyObject.h"
#include "BossEnemyObject.h"
#include "ExplosionObject.h"
#include "Visualisation.h"
#include "Map.h"
#include "Room.h"
#include "Audio.h"
#include "StatBar.h"
#include "Button.h"

World::World()
{
	//Create Player
	m_playerObject = std::make_shared<PlayerObject>(std::pair<int, int>(32, 64), "Player", 301.0f, 301.0f, 0.0f, 2, ObjectTag::E_FRIENDLY, true);
	//Create Doors
	m_worldObjects.push_back(std::make_shared<DoorObject>(DoorDirection::E_UP, std::pair<int, int>(64, 64), "Door", 480.0f, 64.0f, 0.0f, 0, ObjectTag::E_DOOR, true));
	m_worldObjects.push_back(std::make_shared<DoorObject>(DoorDirection::E_DOWN, std::pair<int, int>(64, 64), "Door", 480.0f, 640.0f, 0.0f, 0, ObjectTag::E_DOOR, true));
	m_worldObjects.push_back(std::make_shared<DoorObject>(DoorDirection::E_LEFT, std::pair<int, int>(64, 64), "Door", 64.0f, 384.0f, 0.0f, 0, ObjectTag::E_DOOR, true));
	m_worldObjects.push_back(std::make_shared<DoorObject>(DoorDirection::E_RIGHT, std::pair<int, int>(64, 64), "Door", 896.0f, 384.0f, 0.0f, 0, ObjectTag::E_DOOR, true));

	//Create world objects for game
	for (size_t i = 0; i < 16; i++) {
		m_worldObjects.push_back(std::make_shared<WallObject>(std::pair<int, int>(64, 64), "Wall", 64.0f * i, 0.0f, 0.0f, 0, ObjectTag::E_NEUTRAL, true));
		m_worldObjects.push_back(std::make_shared<WallObject>(std::pair<int, int>(64, 64), "Wall", 64.0f * i, 704.0f, 0.0f, 0, ObjectTag::E_NEUTRAL, true));
	}
	for (size_t i = 1; i < 11; i++) {
		m_worldObjects.push_back(std::make_shared<WallObject>(std::pair<int, int>(64, 64), "Wall", 0.0f, 64.0f * i, 0.0f, 0, ObjectTag::E_NEUTRAL, true));
		m_worldObjects.push_back(std::make_shared<WallObject>(std::pair<int, int>(64, 64), "Wall", 960.0f, 64.0f * i, 0.0f, 0, ObjectTag::E_NEUTRAL, true));
	}

	//Creates and fills the object pools for the lasers and the explosions
	for (size_t i = 0; i < 200; i++) {
		m_bulletPool.push_back(std::make_shared<BulletObject>(std::pair<int, int>(32, 32), "Laser", 0.0f, 0.0f, 0.0f, 0, ObjectTag::E_FRIENDLY_BULLET, false));
		m_bulletPool.push_back(std::make_shared<BulletObject>(std::pair<int, int>(32, 32), "Laser", 0.0f, 0.0f, 0.0f, 0, ObjectTag::E_ENEMY_BULLET, false));
	}
	for (size_t i = 0; i < 20; i++) {
		m_explosionPool.push_back(std::make_shared<ExplosionObject>(std::pair<int, int>(96, 96), "Explosion", 300.0f, 100.0f, 0.0f, 4, ObjectTag::E_PARTICLE, false));
	}

	Map myMap;
	m_map = std::make_shared<Map>(myMap);
	m_chasingEnemyPool.push_back(std::make_shared<ChasingEnemyObject>(std::pair<int, int>(64, 64), "ChasingEnemy", 800.0f, 300.0f, 0.0f, 0, ObjectTag::E_ENEMY, false));
	m_chasingEnemyPool.push_back(std::make_shared<ChasingEnemyObject>(std::pair<int, int>(64, 64), "ChasingEnemy", 800.0f, 300.0f, 0.0f, 0, ObjectTag::E_ENEMY, false));
	m_chasingEnemyPool.push_back(std::make_shared<ChasingEnemyObject>(std::pair<int, int>(64, 64), "ChasingEnemy", 800.0f, 300.0f, 0.0f, 0, ObjectTag::E_ENEMY, false));
	m_chasingEnemyPool.push_back(std::make_shared<ChasingEnemyObject>(std::pair<int, int>(64, 64), "ChasingEnemy", 800.0f, 300.0f, 0.0f, 0, ObjectTag::E_ENEMY, false));
	m_chasingEnemyPool.push_back(std::make_shared<ChasingEnemyObject>(std::pair<int, int>(64, 64), "ChasingEnemy", 800.0f, 300.0f, 0.0f, 0, ObjectTag::E_ENEMY, false));
	m_bossEnemy = std::make_shared<BossEnemyObject>(std::pair<int, int>(80, 80), "BossEnemy", 300.0f, 100.0f, 0.0f, 16, ObjectTag::E_ENEMY, false);

	m_audio = std::make_shared<Audio>(Audio());

	m_playerHealthBar = std::make_shared<StatBar>(Vector3(10.0f, 738.0f, 0.0f), m_playerObject->GetHealth(), HAPI_TColour::RED);
	m_resumeButton = std::make_shared<Button>(Vector3(437.0f, 100.0f, 0.0f), 150, 100, "Resume");
	m_restartButton = std::make_shared<Button>(Vector3(437.0f, 300.0f, 0.0f), 150, 100, "Restart");
	m_quitButton = std::make_shared<Button>(Vector3(437.0f, 500.0f, 0.0f), 150, 100, "Quit");
	m_easyButton = std::make_shared<Button>(Vector3(437.0f, 100.0f, 0.0f), 150, 100, "Easy");
	m_normalButton = std::make_shared<Button>(Vector3(437.0f, 300.0f, 0.0f), 150, 100, "Normal");
	m_hardButton = std::make_shared<Button>(Vector3(437.0f, 500.0f, 0.0f), 150, 100, "Hard");
	m_worldStartSize = m_worldObjects.size();

	std::ifstream reader("Data\\HighScores.txt");
	if (!reader.is_open()) {
		HAPI.UserMessage("Could not open file for save data", "ERROR");
		HAPI.Close();
		return;
	}

	reader >> m_highscore;
	reader.close();

	m_cameraPosition = Vector3(0.0f, 0.0f, 0.0f);
	m_lastUpdate = std::chrono::steady_clock::now();
	m_lastAnimation = std::chrono::steady_clock::now();
}

void World::Run()
{
	std::srand((unsigned int)time(NULL));
	//Initialize screen dimensions and eyes distance from the screen
	int width{ 1024 };
	int height{ 768 };

	//Creates variables for the stars
	Visualisation::Stars stars[1000];
	float cx = (width / 2.0f);
	float cy = (height / 2.0f);

	m_map->GenerateMap();

	if (!HAPI.Initialise(width, height, "Space Dungeon Crawler Game Demo"))
		return;

	BYTE* screen = HAPI.GetScreenPointer();

	//Load in all needed sprites
	Visualisation vis(width, height);
	vis.GenerateSprite("Data\\playerSprite.tga", "Player", 64, 64, true, 64, 64, true);
	vis.GenerateSprite("Data\\turret.png", "RoamingEnemy", 64, 64, true, 64, 64, false);
	vis.GenerateSprite("Data\\enemyShip.png", "ChasingEnemy", 64, 64, true, 64, 64, false);
	vis.GenerateSprite("Data\\bossShip.png", "BossEnemy", 80, 50, true, 80, 50, true);
	vis.GenerateSprite("Data\\space.png", "Background", 256, 256, false, 256, 256, false);
	vis.GenerateSprite("Data\\laser.png", "Laser", 32, 32, true, 32, 32, false);
	vis.GenerateSprite("Data\\portal.png", "Door", 64, 64, true, 64, 64, false);
	vis.GenerateSprite("Data\\target.png", "Target", 64, 64, true, 64, 64, false);
	vis.GenerateSprite("Data\\explosion.png", "Explosion", 96, 96, true, 384, 96, true);
	vis.GenerateSprite("Data\\wall.png", "Wall", 64, 64, true, 64, 64, false);

	m_audio->LoadSound("Data\\Audio\\shot.wav", "Shoot", false, 0.0f, 1.0f, 0.1f);
	m_audio->LoadSound("Data\\Audio\\explode.wav", "Explosion", false, 0.0f, 1.0f, 1.0f);
	m_audio->LoadSound("Data\\Audio\\hit.wav", "Hit", false, 0.0f, 1.0f, 1.0f);

	HAPI.SetShowFPS(true);
	HAPI.LimitFrameRate(60);

	//Main loop
	while (HAPI.Update()) {
		//Fills the star structure with positions for the stars to generate
		for (int i = 0; i < 1000; i++) {
			stars[i].x = rand() % 1024;
			stars[i].y = rand() % 768;
			stars[i].z = rand() % 500;
		}
		//Clears screen to given colour
		vis.ClearToColour(HAPI_TColour(19, 34, 97), 1024, 768);
		vis.RenderStars(width, height,cx, cy, stars, screen);

		if (m_isPlaying == false) {
			m_easyButton->SetIsActive(true);
			m_normalButton->SetIsActive(true);
			m_hardButton->SetIsActive(true);

			HAPI.RenderText(10, 738, HAPI_TColour::BLACK, "Highscore: " + std::to_string(m_highscore), 20);

			HAPI_TMouseData mouseData = HAPI.GetMouseData();

			//Checks to see where the cursor is and executes a button function if it is over a certain button
			if (mouseData.leftButtonDown == true) {
				Vector3 mousePos;
				mousePos.SetX((float)mouseData.x);
				mousePos.SetY((float)mouseData.y);

				bool isEasyClicked = m_easyButton->HasClick(mousePos);
				bool isNormalClicked = m_normalButton->HasClick(mousePos);
				bool isHardClicked = m_hardButton->HasClick(mousePos);

				if (isEasyClicked == true) {
					m_difficulty = Difficulty::E_EASY;
					m_easyButton->SetIsActive(false);
					m_normalButton->SetIsActive(false);
					m_hardButton->SetIsActive(false);
					m_isPlaying = true;
					SpawnEnemies();
				}
				else if (isNormalClicked == true) {
					m_difficulty = Difficulty::E_NORMAL;
					m_easyButton->SetIsActive(false);
					m_normalButton->SetIsActive(false);
					m_hardButton->SetIsActive(false);
					m_isPlaying = true;
					SpawnEnemies();
				}
				else if (isHardClicked == true) {
					m_difficulty = Difficulty::E_HARD;
					m_easyButton->SetIsActive(false);
					m_normalButton->SetIsActive(false);
					m_hardButton->SetIsActive(false);
					m_isPlaying = true;
					SpawnEnemies();
				}
			}

			MasterRender(vis, 0.0f);
			continue;
		}
		else {
			//Does a check to see if the game is either over or paused
			if (m_playerObject->GetHealth() <= 0) {
				SavePoints();
				m_restartButton->SetIsActive(true);
				m_quitButton->SetIsActive(true);

				HAPI_TMouseData mouseData = HAPI.GetMouseData();

				//Checks to see where the cursor is and executes a button function if it is over a certain button
				if (mouseData.leftButtonDown == true) {
					Vector3 mousePos;
					mousePos.SetX((float)mouseData.x);
					mousePos.SetY((float)mouseData.y);
					bool isRestartClicked = m_restartButton->HasClick(mousePos);
					bool isQuitClicked = m_quitButton->HasClick(mousePos);
					if (isRestartClicked == true) {
						std::cout << "Button Pressed" << std::endl;
						m_points = 0;
						ResetWorld();
					}
					else if (isQuitClicked == true) {
						HAPI.Close();
					}
				}
			}
			else if (CheckAllEnemiesDead() == true && m_map->GetCurrentRoom().GetIsBossRoom() == true && m_bossEnemy->GetIsActive() == false) {
				m_restartButton->SetIsActive(true);
				m_quitButton->SetIsActive(true);

				HAPI_TMouseData mouseData = HAPI.GetMouseData();

				//Checks to see where the cursor is and executes a button function if it is over a certain button
				if (mouseData.leftButtonDown == true) {
					Vector3 mousePos;
					mousePos.SetX((float)mouseData.x);
					mousePos.SetY((float)mouseData.y);
					bool isRestartClicked = m_restartButton->HasClick(mousePos);
					bool isQuitClicked = m_quitButton->HasClick(mousePos);
					if (isRestartClicked == true) {
						std::cout << "Button Pressed" << std::endl;
						GainPoints(500);
						ResetWorld();
					}
					else if (isQuitClicked == true) {
						GainPoints(500);
						SavePoints();
						HAPI.Close();
					}
				}
			}
			else if (m_isPaused == true) {
				m_restartButton->SetIsActive(true);
				m_quitButton->SetIsActive(true);
				m_resumeButton->SetIsActive(true);

				HAPI_TMouseData mouseData = HAPI.GetMouseData();

				//Checks to see where the cursor is and executes a button function if it is over a certain button
				if (mouseData.leftButtonDown == true) {
					Vector3 mousePos;
					mousePos.SetX((float)mouseData.x);
					mousePos.SetY((float)mouseData.y);
					bool isRestartClicked = m_restartButton->HasClick(mousePos);
					bool isQuitClicked = m_quitButton->HasClick(mousePos);
					bool isResumeClicked = m_resumeButton->HasClick(mousePos);
					if (isRestartClicked == true) {
						std::cout << "Button Pressed" << std::endl;
						SavePoints();
						m_points = 0;
						ResetWorld();
					}
					else if (isQuitClicked == true) {
						SavePoints();
						HAPI.Close();
					}
					else if (isResumeClicked == true) {
						m_restartButton->SetIsActive(false);
						m_quitButton->SetIsActive(false);
						m_resumeButton->SetIsActive(false);
						m_isPaused = false;
					}
				}
			}
			else {
				//Main update loop
				HAPI_TKeyboardData keyboardData = HAPI.GetKeyboardData();
				HAPI_TControllerData controllerData = HAPI.GetControllerData(0);

				//Pauses the game if escape/the start button is pressed
				if (controllerData.isAttached == true) {
					if (controllerData.digitalButtons[HK_DIGITAL_START] == true) {
						m_isPaused = true;
					}
				}
				else {
					if (keyboardData.scanCode[HK_ESCAPE] == true) {
						std::cout << "Paused" << std::endl;
						m_isPaused = true;
					}
				}

				m_playerObject->CheckCollision(m_worldObjects, m_playerObject, *this);

				//Checks collisions for all world objects
				for (size_t i = 0; i < m_worldObjects.size(); i++) {
					if (m_worldObjects[i]->GetIsActive() == true) {
						m_worldObjects[i]->CheckCollision(m_worldObjects, m_playerObject, *this);
					}
				}


				//Will only run object update functions after a set period of time
				std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();
				float timePassed = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(time - m_lastUpdate).count();

				if (timePassed >= 20000000.0f) {
					CleaUpRuntimeObjects();
					m_lastUpdate = std::chrono::steady_clock::now();
					m_playerObject->Update(*this);

					for (size_t i = 0; i < m_worldObjects.size(); i++) {
						if (m_worldObjects[i]->GetIsActive() == true) {
							m_worldObjects[i]->Update(*this);
						}
					}

					m_playerHealthBar->SetValue(m_playerObject->GetHealth());
				}
			}

			//Renders each object once per frame
			std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();

			float timeElapsed = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(time - m_lastUpdate).count();
			float fTime = timeElapsed / 20000000.0f;

			m_cameraPosition = *m_playerObject->GetPosition() - Vector3((float)width / 2, (float)height / 2, 0.0f);
			m_cameraPosition.Lerp(m_cameraPosition, m_cameraPosition + (*m_playerObject->GetVelocity() * m_playerObject->GetMoveSpeed()), fTime);

			MasterRender(vis, fTime);
		}
	}

		
}

//Loops through the bullet pool to spawn a bullet at a set position and velocity with a set tag
void World::SpawnBullet(Vector3& p, Vector3& v, ObjectTag t)
{
	for (size_t i = 0; i < m_bulletPool.size(); i++) {
		if (m_bulletPool[i]->GetIsActive() == false && m_bulletPool[i]->GetTag() == t) {
			m_bulletPool[i]->SetPosition(p);
			m_bulletPool[i]->SetVelocity(v);
			m_bulletPool[i]->SetActive(true);
			m_bulletPool[i]->ResetTimer();
			m_worldObjects.push_back(m_bulletPool[i]);
			m_audio->PlaySound("Shoot");
			m_otherExtraInstanceCount++;
			break;
		}
	}
}

//Spawns an explosion in at a set position
void World::SpawnExplosion(const Vector3& p)
{
	for (size_t i = 0; i < m_explosionPool.size(); i++) {
		if (m_explosionPool[i]->GetIsActive() == false) {
			m_explosionPool[i]->SetPosition(p);
			m_explosionPool[i]->SetActive(true);
			m_explosionPool[i]->SetCurrentFrame(0);
			m_worldObjects.push_back(m_explosionPool[i]);
			m_audio->PlaySound("Explosion");
			m_otherExtraInstanceCount++;
			break;
		}
	}
}

//Moves to the next room if all enemies are dead
void World::MoveRoom(DoorDirection& d)
{
	if (CheckAllEnemiesDead() == false) {
		return;
	}

	//Clears all non-permanent objects from the world vector
	for (int i = 0; i < m_enemyCount + m_otherExtraInstanceCount; i++) {
		if (m_worldObjects.size() <= m_worldStartSize) {
			break;
		}
		else {
			m_worldObjects.pop_back();
		}
	}

	Vector3 playerOffset = *m_playerObject->GetPosition();

	m_map->GetCurrentRoom().SetIsCleared(true);

	m_otherExtraInstanceCount = 0;

	//Game progresses to the next area or "room" depening on which portal or "door" the player goes through
	switch (d) {
	case DoorDirection::E_UP:
		m_map->StepRoom(RoomDirection::E_UP);
		playerOffset = *m_worldObjects[1]->GetPosition() + Vector3(0.0f, -96.0f, 0.0f);
		m_playerObject->SetPosition(playerOffset);
		break;
	case DoorDirection::E_DOWN:
		m_map->StepRoom(RoomDirection::E_DOWN);
		playerOffset = *m_worldObjects[0]->GetPosition() + Vector3(0.0f, 96.0f, 0.0f);
		m_playerObject->SetPosition(playerOffset);
		break;
	case DoorDirection::E_LEFT:
		m_map->StepRoom(RoomDirection::E_LEFT);
		playerOffset = *m_worldObjects[3]->GetPosition() + Vector3(-96.0f, 0.0f, 0.0f);
		m_playerObject->SetPosition(playerOffset);
		break;
	case DoorDirection::E_RIGHT:
		m_map->StepRoom(RoomDirection::E_RIGHT);
		playerOffset = *m_worldObjects[2]->GetPosition() + Vector3(96.0f, 0.0f, 0.0f);
		m_playerObject->SetPosition(playerOffset);
		break;
	default:
		break;
	}

	//Sets each door to active based on which doors are in the new room
	if (m_map->GetCurrentRoom().HasUpDoor() == true) {
		m_worldObjects[0]->SetActive(true);
	}
	else {
		m_worldObjects[0]->SetActive(false);
	}

	if (m_map->GetCurrentRoom().HasDownDoor() == true) {
		m_worldObjects[1]->SetActive(true);
	}
	else {
		m_worldObjects[1]->SetActive(false);
	}

	if (m_map->GetCurrentRoom().HasLeftDoor() == true) {
		m_worldObjects[2]->SetActive(true);
	}
	else {
		m_worldObjects[2]->SetActive(false);
	}

	if (m_map->GetCurrentRoom().HasRightDoor() == true) {
		m_worldObjects[3]->SetActive(true);
	}
	else {
		m_worldObjects[3]->SetActive(false);
	}

	//Spawns in enemies if the player hasn't entered the new room yet
	if (m_map->GetCurrentRoom().GetIsCleared() == false) {
		SpawnEnemies();
	}
	else {
		m_enemyCount = 0;
	}
}

const Vector3& World::GetEnemyPosition(size_t i)
{
	if (i > m_worldObjects.size()) {
		HAPI.UserMessage("Tried to fetch AI position out of range", "ERROR");
		HAPI.Close();
	}
	return *m_worldObjects[i + 4]->GetPosition();
}

const Vector3& World::GetPlayerPosition()
{
	return *m_playerObject->GetPosition();
}

void World::GainPoints(const size_t p)
{
	size_t multiplier{ 1 };
	switch (m_difficulty) {
	case Difficulty::E_EASY:
		multiplier = 1;
		break;
	case Difficulty::E_NORMAL:
		multiplier = 2;
		break;
	case Difficulty::E_HARD:
		multiplier = 3;
		break;
	default:
		break;
	}

	m_points = m_points + (p * multiplier);
}

void World::PlayHitSound()
{
	m_audio->PlaySound("Hit");
}

void World::MasterRender(Visualisation& v, float s)
{
	if (m_isPlaying == false) {
		m_easyButton->Render(v);
		m_normalButton->Render(v);
		m_hardButton->Render(v);
		return;
	}

	std::chrono::steady_clock::time_point time = std::chrono::steady_clock::now();
	float timePassed = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(time - m_lastAnimation).count();
	//Updates animation if set time has elapsed
	if (timePassed >= 3333333333.33f) {
		//Update animations here
		m_playerObject->SetCurrentFrame(m_playerObject->GetCurrentFrame() + 1);
		for (std::shared_ptr<Object> o : m_worldObjects) {
			o->SetCurrentFrame(o->GetCurrentFrame() + 1);
		}
		std::chrono::steady_clock::time_point m_lastAnimation = std::chrono::steady_clock::now();
	}

	//Renders each object to display on the window
	m_playerObject->Render(v, s, m_cameraPosition);

	for (size_t i = 0; i < m_worldObjects.size(); i++) {
		m_worldObjects[i]->Render(v, s, m_cameraPosition);
	}

	m_playerHealthBar->Render(v);

	m_resumeButton->Render(v);
	m_restartButton->Render(v);
	m_quitButton->Render(v);
	HAPI.RenderText(10, 720, HAPI_TColour::WHITE, "Points: " + std::to_string(m_points), 18);
}

//Loops through each enemy in the world to check how many are active
bool World::CheckAllEnemiesDead()
{
	if (m_enemyCount <= 0 || m_map->GetCurrentRoom().GetIsCleared() == true) {
		return true;
	}

	bool allDead{ true };

	int startPoint = (int)m_worldObjects.size() - m_enemyCount - m_otherExtraInstanceCount - 1;

	if (startPoint <= 3) {
		return true;
	}

	int endPoint = (int)m_worldObjects.size() - m_otherExtraInstanceCount;

	for (int i = startPoint; i < endPoint; i++) {
		if (i >= m_worldObjects.size()) {
			break;
		}

		if (m_worldObjects[i]->GetIsActive() == true && m_worldObjects[i]->GetTag() == ObjectTag::E_ENEMY) {
			allDead = false;
			break;
		}
	}

	return allDead;
}

//Spawns enemies into the world
void World::SpawnEnemies()
{
	std::vector<Vector3> points = m_map->GetCurrentRoom().GetSpawnPoints();

	//Spawns in the boss enemy if the player is in a "boss room"
	if (m_map->GetCurrentRoom().GetIsBossRoom() == true) {
		m_enemyCount = 1;
		m_bossEnemy->Reset();
		m_bossEnemy->SetActive(true);
		m_bossEnemy->SetPosition(Vector3(472.0f, 344.0f, 0.0f));
		m_worldObjects.push_back(m_bossEnemy);
		return;
	}

	int enemyCap{ 0 };
	switch (m_difficulty) {
	case Difficulty::E_EASY:
		enemyCap = 2;
		break;
	case Difficulty::E_NORMAL:
		enemyCap = 1;
		break;
	case Difficulty::E_HARD:
		enemyCap = 0;
		break;
	default:
		break;
	}

	//Randomly decides a number of enemies to spawn
	int numberToSpawn = std::rand() % ((int)points.size() - enemyCap);

	//Creates an enemy count based off the number of enemies it will spawn + 2 to spawn a decent number of enemies
	m_enemyCount = numberToSpawn + 2;

	for (size_t i = 0; i <= numberToSpawn; i++) {
		for (size_t j = 0; j < 5; j++){
			if (m_chasingEnemyPool[j]->GetIsActive() == false) {
				m_chasingEnemyPool[j]->Reset();
				m_chasingEnemyPool[j]->SetActive(true);
				m_chasingEnemyPool[j]->SetPosition(points[i]);
				m_worldObjects.push_back(m_chasingEnemyPool[j]);
				break;
			}
		}
	}
}

//Resets the world to its origninal state to restart the game
void World::ResetWorld()
{
	m_isPaused = false;

	while (m_worldObjects.size() > m_worldStartSize) {
		m_worldObjects.pop_back();
	}

	for (std::shared_ptr<Object> b : m_bulletPool) {
		b->SetActive(false);
	}

	for (std::shared_ptr<Object> e : m_explosionPool) {
		e->SetActive(false);
	}

	for (std::shared_ptr<ChasingEnemyObject> e : m_chasingEnemyPool) {
		e->Reset();
	}

	m_bossEnemy->Reset();

	for (std::shared_ptr<Object> o : m_worldObjects) {
		if (o->GetTag() == ObjectTag::E_DOOR || o->GetTag() == ObjectTag::E_NEUTRAL) {
			o->SetActive(true);
		}
		else {
			o->SetActive(false);
		}
	}

	m_playerObject->SetActive(true);
	m_playerObject->SetPosition(Vector3(301.0f, 301.0f, 0.0f));
	m_playerObject->Reset();

	m_map->GenerateMap();
	m_enemyCount = 0;
	m_otherExtraInstanceCount = 0;
	m_restartButton->SetIsActive(false);
	m_resumeButton->SetIsActive(false);
	m_quitButton->SetIsActive(false);
	SpawnEnemies();
}

//Cleans up all inactive objects in the world
void World::CleaUpRuntimeObjects()
{
	int loopStart = (int)m_worldObjects.size() - m_otherExtraInstanceCount - 1;

	for (size_t i = loopStart; i < m_worldObjects.size(); i++) {
		if (m_worldObjects[i]->GetIsActive() == false && m_worldObjects[i]->GetTag() != ObjectTag::E_DOOR) {
			m_worldObjects.erase(m_worldObjects.begin() + i);
			m_otherExtraInstanceCount--;
			i--;
		}
	}
}

//Saves the score to a text file if the score is higher than the previously stored score (stores it regardless if the file is empty)
void World::SavePoints()
{
	std::ifstream reader("Data\\HighScores.txt");

	if (!reader.is_open()) {
		HAPI.UserMessage("Could not open file for save data", "ERROR");
		HAPI.Close();
		return;
	}

	size_t highscore{ 0 };
	reader >> highscore;
	reader.close();

	if (m_points > highscore) {
		std::cout << "new score" << std::endl;
		std::ofstream writer("Data\\HighScores.txt");

		if (!writer.is_open()) {
			HAPI.UserMessage("Could not open file for save data", "ERROR");
			HAPI.Close();
			return;
		}
		writer << m_points;
		writer.close();
	}
}
