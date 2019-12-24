//#include "pch.h"
#include <tchar.h>
#include "../CompEngine/CompEngine.h"
#include "../CompEngine/CompEngine.cpp"
#include <string>

//#include "../googletest-release-1.8.1/googletest/

using namespace CompEngine;
using namespace std;
//using namespace std;

TEST(TestCompEngine, EngineTest_1) {
	EngineCore* engine = new EngineCore;
	GameObject mainManager("Managers");

	GameManager* gameManager = new GameManager;
	mainManager.AddComponent((Component*)gameManager);
	PlayerController* playerController = new PlayerController;
	mainManager.AddComponent(playerController);
	LevelManager* levelManager = new LevelManager;
	mainManager.AddComponent(levelManager);
	UIManager* uiManager = new UIManager;
	mainManager.AddComponent(uiManager);
	GameLogicManager* gameLogicManager = new GameLogicManager();
	mainManager.AddComponent(gameLogicManager);
	engine->Instantiate(mainManager);


	GameObject player("Soldier");
	player.SetTag(GameValues::playerTag);
	Unit* a = new Unit();
	a->Initialize("Soldier", '^', 100, 10, 25, 10, 1);
	Shoot* shoot = new Shoot();

	player.AddComponent(a);
	engine->Instantiate(player);
	GameManager gm;
	GameManager* gmPtr = (GameManager*)engine->FindObjectOfType(gm);

	EXPECT_TRUE(typeid(*gmPtr) == typeid(*(Component*)gameManager));

	//cout << gmPtr->GetGameState();
}

TEST(TestCompEngine, EngineTest_2) {
	EngineCore* engine = new EngineCore;

	GameObject player("Soldier");
	player.SetTag(GameValues::playerTag);
	Unit* a = new Unit();
	a->Initialize("Soldier", 100, 10, 25, 10, 1);

	player.AddComponent(a);
	for (int i = 0; i < 10; i++)
	{
		engine->Instantiate(player);
	}
	vector<GameObject*> playerPtrs = engine->FindObjectsWithTag(GameValues::playerTag);
	for (int i = 0; i < playerPtrs.size(); i++)
	{
		GameObject* playerPtr = playerPtrs[i];
		EXPECT_TRUE(typeid(player) == typeid(*playerPtr));
	}
	//cout << gmPtr->GetGameState();
}
TEST(TestCompEngine, EngineTest_3) {
	EngineCore* engine = new EngineCore;

	GameObject player("Soldier");
	player.SetTag(GameValues::playerTag);
	Unit* a = new Unit();
	a->Initialize("Soldier", '^', 100, 10, 25, 10, 1);

	player.AddComponent(a);
	vector<GameObject*> players;
	for (int i = 0; i < 10; i++)
	{
		player.name = "Soldier" + i;
		players.push_back(engine->Instantiate(player));
	}
	vector<GameObject*> playerPtrs = engine->FindObjectsWithTag(GameValues::playerTag);
	for (int i = 0; i < playerPtrs.size(); i++)
	{
		GameObject* playerPtr = playerPtrs[i];
		EXPECT_TRUE((players[i]->name) == (playerPtr->name));
	}
	//cout << gmPtr->GetGameState();
}

TEST(TestCompEngine, EngineTest_4) {
	EngineCore* engine = new EngineCore;

	GameObject player("Soldier");
	player.SetTag(GameValues::playerTag);
	Unit* a = new Unit();
	a->Initialize("Soldier", '^', 100, 10, 25, 10, 1);

	player.AddComponent(a);
	vector<GameObject*> players;
	for (int i = 0; i < 5; i++)
	{
		string soldier = "Soldier";
		soldier.push_back(char(i + 48));
		player.name = soldier;
		engine->Instantiate(player);
	}
	GameObject* playerPtr = engine->Find("Soldier4");
	EXPECT_TRUE("Soldier4" == (playerPtr->name));
}

TEST(TestCompEngine, EngineTest_5) {
	EngineCore* engine = new EngineCore;

	GameObject player("Soldier");
	player.SetTag(GameValues::playerTag);
	Unit* a = new Unit();
	a->Initialize("Soldier", 100, 10, 25, 10, 1);

	player.AddComponent(a);
	for (int i = 0; i < 5; i++)
	{
		string soldier = "Soldier";
		soldier.push_back(char(i + 48));
		player.name = soldier;
		engine->Instantiate(player);
	}
	GameObject* playerPtr;
	playerPtr = engine->Find("Soldier14");
	EXPECT_TRUE((playerPtr) == nullptr);
}

TEST(TestCompEngine, EngineTest_6) {
	EngineCore* engine = new EngineCore;
	GameObject mainManager("Managers");

	GameManager* gameManager = new GameManager;
	mainManager.AddComponent((Component*)gameManager);
	PlayerController* playerController = new PlayerController;
	mainManager.AddComponent(playerController);
	LevelManager* levelManager = new LevelManager;
	mainManager.AddComponent(levelManager);
	UIManager* uiManager = new UIManager;
	mainManager.AddComponent(uiManager);
	GameLogicManager* gameLogicManager = new GameLogicManager();
	mainManager.AddComponent(gameLogicManager);
	GameObject* go = engine->Instantiate(mainManager);

	PlayerController pc;
	PlayerController* playerControllerPtr = (PlayerController*)(go->GetEngineCore()->FindObjectOfType(pc));

	EXPECT_TRUE((playerControllerPtr) != nullptr);
}

TEST(TestCompEngine, EngineTest_7) {
	EngineCore* engine = new EngineCore;
	GameObject mainManager("Managers");

	GameManager* gameManager = new GameManager;
	mainManager.AddComponent((Component*)gameManager);
	PlayerController* playerController = new PlayerController;
	mainManager.AddComponent(playerController);
	LevelManager* levelManager = new LevelManager;
	mainManager.AddComponent(levelManager);
	UIManager* uiManager = new UIManager;
	mainManager.AddComponent(uiManager);
	GameLogicManager* gameLogicManager = new GameLogicManager();
	mainManager.AddComponent(gameLogicManager);
	engine->Instantiate(mainManager);


	GameObject player("Soldier");
	player.SetTag(GameValues::playerTag);
	Unit* a = new Unit();
	a->Initialize("Soldier", '^', 100, 10, 25, 10, 1);

	player.AddComponent(a);
	engine->Instantiate(player);
	GameLogicManager glm;
	GameLogicManager* glmPtr = (GameLogicManager*)engine->FindObjectOfType(glm);

	EXPECT_TRUE(typeid(*glmPtr) == typeid(*(Component*)gameLogicManager));

	//cout << gmPtr->GetGameState();
}

TEST(TestGame, GameTest_1) {
	EngineCore* engine = new EngineCore;
	GameObject mainManager("Managers");

	GameManager* gameManager = new GameManager;
	mainManager.AddComponent((Component*)gameManager);
	PlayerController* playerController = new PlayerController;
	mainManager.AddComponent(playerController);
	LevelManager* levelManager = new LevelManager;
	mainManager.AddComponent(levelManager);
	UIManager* uiManager = new UIManager;
	mainManager.AddComponent(uiManager);
	GameLogicManager* gameLogicManager = new GameLogicManager();
	mainManager.AddComponent(gameLogicManager);
	engine->Instantiate(mainManager);


	GameObject player("Soldier");
	player.SetTag(GameValues::playerTag);
	Unit* a = new Unit();
	a->Initialize("Soldier", '^', 100, 10, 25, 10, 1);

	player.AddComponent(a);
	engine->Instantiate(player);
	GameLogicManager glm;
	GameLogicManager* glmPtr = (GameLogicManager*)engine->FindObjectOfType(glm);
	EXPECT_TRUE(true);
}

TEST(TestGame, GameTest_2) {
	EngineCore* engine = new EngineCore;
	GameObject mainManager("Managers");

	GameManager* gameManager = new GameManager;
	mainManager.AddComponent((Component*)gameManager);
	PlayerController* playerController = new PlayerController;
	mainManager.AddComponent(playerController);
	LevelManager* levelManager = new LevelManager;
	mainManager.AddComponent(levelManager);
	UIManager* uiManager = new UIManager;
	mainManager.AddComponent(uiManager);
	GameLogicManager* gameLogicManager = new GameLogicManager();
	mainManager.AddComponent(gameLogicManager);
	engine->Instantiate(mainManager);


	GameObject player("Soldier");
	player.SetTag(GameValues::playerTag);
	Unit* a = new Unit();
	a->Initialize("Soldier", '^', 100, 10, 25, 10, 1);

	player.AddComponent(a);
	engine->Instantiate(player);
	GameLogicManager glm;
	GameLogicManager* glmPtr = (GameLogicManager*)engine->FindObjectOfType(glm);
	EXPECT_TRUE(true);
}
TEST(TestQueue, QueueTest_1) {
	Queue<int> q;

	for (int i = 0; i < 10; i++)
	{
		q.Push(i);
	}
	EXPECT_TRUE(q.Size() == 10);
	for (int i = 0; i < q.Size(); i++)
	{
		EXPECT_TRUE(q.Pop() == i);
	}

}

TEST(TestQueue, QueueTest_2) {
	Queue<int> q(2);

	for (int i = 0; i < 5; i++)
	{
		q.Push(i);
	}
	EXPECT_TRUE(q.Size() == 5);
	for (int i = 0; i < q.Size(); i++)
	{
		int t = q.Pop();
		EXPECT_TRUE(t == i);
	}

}
TEST(TestQueue, QueueTest_3) {
	Queue<int> q(2);
	ASSERT_THROW(q.Pop(), underflow_error);
}
TEST(TestQueue, QueueTest_4) {
	Queue<int> q(2);

	for (int i = 0; i < 5; i++)
	{
		q.Push(i);
		q.Pop();
	}
	for (int i = 0; i < 5; i++)
	{
		q.Push(i);
	}
	EXPECT_TRUE(q.Size() == 5);
	for (int i = 0; i < q.Size(); i++)
	{
		int t = q.Pop();
		EXPECT_TRUE(t == i);
	}
}
TEST(TestQueue, QueueTest_5) {
	Queue<int> q(2, 1);

	for (int i = 0; i < 5; i++)
	{
		q.Push(i);
		q.Pop();
	}
	for (int i = 0; i < 5; i++)
	{
		q.Push(i);
	}
	EXPECT_TRUE(q.Size() == 5);
	for (int i = 0; i < q.Size(); i++)
	{
		int t = q.Pop();
		EXPECT_TRUE(t == i);
	}
}
TEST(TestQueue, QueueTest_6) {
	Queue<int> q(1, 1);

	for (int i = 0; i < 5; i++)
	{
		q.Push(i);
		q.Pop();
	}
	for (int i = 0; i < 5; i++)
	{
		q.Push(i);
	}
	EXPECT_TRUE(q.Size() == 5);
	for (int i = 0; i < q.Size(); i++)
	{
		int t = q.Pop();
		EXPECT_TRUE(t == i);
	}
}
TEST(TestQueue, QueueTest_7) {

	ASSERT_THROW(Queue<int> q(1, 0), invalid_argument);
}
TEST(TestQueue, QueueTest_8) {

	ASSERT_THROW(Queue<int> q(0, 0), invalid_argument);
}
TEST(TestQueue, QueueTest_9) {

	ASSERT_THROW(Queue<int> q(-1, 0), invalid_argument);
}
TEST(TestQueue, QueueTest_10) {

	ASSERT_THROW(Queue<int> q(0, 1), invalid_argument);
}
TEST(TestQueue, QueueTest_11) {

	ASSERT_THROW(Queue<int> q(-1, 1), invalid_argument);
}
TEST(TestQueue, QueueTest_12) {

	ASSERT_THROW(Queue<int> q(1, -1), invalid_argument);
}
TEST(TestQueue, QueueTest_13) {

	ASSERT_THROW(Queue<int> q(4000000000000000000, 2), invalid_argument);
}
TEST(TestQueue, QueueTest_14) {

	ASSERT_THROW(Queue<int> q(INT32_MAX, 2), bad_alloc);
}
TEST(TestQueue, QueueTest_15) {

	Queue<int> q(1, INT_MAX);
	q.Push(1);
	ASSERT_THROW(q.Push(1), bad_alloc);
}
int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}