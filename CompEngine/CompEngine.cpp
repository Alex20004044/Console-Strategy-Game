// CompEngine.cpp : Определяет функции для статической библиотеки.
//


#pragma once
//#include "stdafx.h"
// "pch.h"
//#include "framework.h"
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <algorithm>
//#include "Unit.h"
#include <ctime>
#include <iostream>
#include"CompEngine.h"
#include <stdlib.h>
using namespace std;

namespace CompEngine
{
	void Component::SetActive(bool b)
	{
		isActive = b;
	}
	bool Component::IsActive()
	{
		return isActive;
	}
	GameObject* Component::GetGameObject()
	{
		return gameObject;
	}
	void Component::SetGameObject(GameObject* _gameObject)
	{
		gameObject = _gameObject;
	}

	Vector2Int::Vector2Int()
	{
		x = 0;
		y = 0;
	}
	Vector2Int::Vector2Int(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Vector2Int Vector2Int::Zero()
	{
		return Vector2Int(0, 0);
	}
	int Vector2Int::X()
	{
		return x;
	}
	int Vector2Int::Y()
	{
		return y;
	}
	float Vector2Int::Length()
	{
		return sqrt(x * x + y * y);
	}
	Vector2Int Vector2Int::Increase(Vector2Int r)
	{
		return Vector2Int(x - r.x, y - r.y);
	}
	bool Vector2Int::Compare(const Vector2Int& r)
	{
		if (x == r.x && y == r.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Transform::Transform(GameObject* go)
	{
		position = Vector2Int();
		gameObject = go;
	}
	Vector2Int Transform::GetPosition()
	{
		return position;
	}
	bool Transform::SetPosition(Vector2Int _position)
	{
		if (LevelManager::instance->TryMoveObject(_position, this))
		{
			position = _position;
			return true;
		}
		else
		{
			return false;
		}
	}
	GameObject* Transform::GetGameObject()
	{
		return gameObject;
	}
	void Transform::SetGameObject(GameObject* _gameObject)
	{
		gameObject = _gameObject;
	}


	void GameObject::SetActive(bool b)
	{
		isActive = b;
	}
	bool const GameObject::IsActive()
	{
		return isActive;
	}
	GameObject::GameObject(string _name = "Empty")//const EngineCore& engine, string _name)
	{
		name = _name;
		SetActive(true);
		transform = new Transform(this);
		//engine.AddGameObject(this);
	}
	GameObject::GameObject(const GameObject& obj)//const EngineCore& engine, string _name)
	{
		name = obj.name;
		tag = obj.tag;
		transform = new Transform(this);
		//transform->SetPosition(obj.transform->GetPosition());
		SetActive(obj.isActive);
		for (int i = 0; i < obj.components.size(); i++)
		{
			//Component comp = *obj.components[i];
			Component* compPtr = obj.components[i]->Copy();
			compPtr->SetGameObject(this);
			components.push_back(compPtr);
		}
		//engine.AddGameObject(this);
	}
	GameObject::~GameObject()
	{
		delete(transform);
		for (int i = 0; i < components.size(); i++)
		{
			delete(components[i]);
			components[i] = nullptr;
		}
	}
	void GameObject::AddComponent(Component* component)
	{
		component->SetGameObject(this);
		components.push_back(component);
	}
	Transform* GameObject::GetTransform()
	{
		return transform;
	}
	void GameObject::Update()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i]->IsActive())
			{
				components[i]->Update();
			}
		}
	}
	void GameObject::Awake()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i]->IsActive())
			{
				components[i]->Awake();
			}
		}
	}
	void GameObject::Start()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i]->IsActive())
			{
				components[i]->Start();
			}
		}
	}
	/*
	Component* GameObject::FindComponentOfType(Component component)
	{

	}*/
	string GameObject::GetTag()
	{
		return tag;
	}
	void GameObject::SetTag(string _tag)
	{
		tag = _tag;
	}
	EngineCore* GameObject::GetEngineCore()
	{
		return engineCore;
	}
	void GameObject::SetEngineCore(EngineCore* _engineCore)
	{
		engineCore = _engineCore;
	}

	TileType Tile::GetTileType()
	{
		return tileType;
	}

	bool Tile::IsTileAvailableToStep()
	{
		if (tileType != wall && tileType != obstacle)
		{
			for (int i = 0; i < gameObjects.size(); i++)
			{
				Unit u;
				Unit* unit = (Unit*)gameObjects[i]->FindComponentOfType(u);
				if (unit != nullptr)
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}
	void Tile::AddGameObject(GameObject* go)
	{
		/*for (int i = 0; i < gameObjects.size(); i++)
		{
		if (gameObjects[i] == go)
		{
		//throw "Trying to add existing object";
		return;
		}
		}*/

		gameObjects.push_back(go);
	}
	void Tile::RemoveGameObject(GameObject* go)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i] == go)
			{
				gameObjects.erase(gameObjects.begin() + i);
				return;
			}
		}
		//if(go->)
		//throw "Trying to remove upset object";

	}
	char Tile::GetTileImage()
	{
		char image = '.';
		Unit u;
		Unit* unit;
		for (int i = 0; i < gameObjects.size(); i++)
		{
			unit = (Unit*)gameObjects[i]->FindComponentOfType(u);
			if (unit != nullptr)
			{
				return unit->GetUnitSymbol();
			}
		}

		switch (tileType)
		{
		case(empty) :
		{
						return '.';
						break;
		}
		case(obstacle) :
		{
						   return '*';
						   break;
		}
		case(wall) :
		{
					   return '#';
					   break;
		}
		default:
			return '0';
			break;
		}
	}
	void Tile::DamageTile(int damage)
	{
		if (tileType == wall)
		{
			return;
		}
		else if (tileType == obstacle)
		{
			tileType = empty;
		}
		else
		{
			Unit* unit = nullptr;
			Unit u;
			for (int i = 0; i < gameObjects.size(); i++)
			{
				unit = (Unit*)gameObjects[i]->FindComponentOfType(u);
				if (unit != nullptr)
				{
					unit->TakeDamage(damage);
				}
			}
		}
	}
	void Tile::SetTileType(TileType _tileType)
	{
		tileType = _tileType;
	}

	Component* Unit::Copy()
	{
		return CreateCopy(this);
	}
	void Unit::Initialize(string _unitName, char _unitSymbol, int _maxHealth, int _maxTimePoint, int _timePointPerStep, int _sightRadius, float _accuracy)
	{
		unitName = _unitName;
		maxHealth = _maxHealth;
		health = maxHealth;
		maxTimePoint = _maxTimePoint;
		timePointPerStep = _timePointPerStep;
		sightRadius = _sightRadius;
		accuracy = _accuracy;
		unitSymbol = _unitSymbol;;
	}
	bool Unit::TryMove(Vector2Int position)
	{
		Transform* transform = GetGameObject()->GetTransform();
		if (position.X() != transform->GetPosition().X() ||
			position.Y() != transform->GetPosition().Y())
		{
			if (timePoint >= timePointPerStep)
			{
				if (transform->SetPosition(position))
				{
					DecreaseTimePoint(timePointPerStep);
					return true;
				}
			}

		}
		return false;
	}
	void Unit::TakeDamage(int damage)
	{
		health -= damage;
		if (health < 0)
		{
			Destruct();
		}
	}
	void Unit::Destruct()
	{
		GetGameObject()->GetEngineCore()->Destroy(GetGameObject());

		//Set
	}
	char Unit::GetUnitSymbol()
	{
		return unitSymbol;
	}
	string Unit::GetUnitName()
	{
		return unitName;
	}
	int Unit::GetHealth()
	{
		return health;
	}
	int Unit::GetMaxHealth()
	{
		return maxHealth;
	}
	int Unit::GetTimePoint()
	{
		return timePoint;
	}
	int Unit::GetMaxTimePoint()
	{
		return maxTimePoint;
	}
	int Unit::GetTimePointPerStep()
	{
		return timePointPerStep;
	}
	bool Unit::DecreaseTimePoint(int value)
	{
		timePoint -= value;
		return true;
	}
	void Unit::NewTurn()
	{
		timePoint = maxTimePoint;
	}


	Component* Shoot::Copy()
	{
		Shoot* s = (Shoot*) CreateCopy(this);
		s->gun = new Gun();
		*(s->gun) = *gun;
		return s;
	}
	void Shoot::Start()
	{
		Unit u;
		unit = (Unit*)GetGameObject()->FindComponentOfType(u);
	}
	bool Shoot::TryShoot(Vector2Int position)
	{
		if ((unit->GetTimePoint() >= gun->shootTimePoint) && IsCanShootToTile(position))
		{
			unit->DecreaseTimePoint(gun->shootTimePoint);
			float distance = position.Increase(GetGameObject()->GetTransform()->GetPosition()).Length();
			int damage = CalculateDamage(distance);
			gun->currentAmmo--;
			LevelManager::instance->ShootToTile(position, damage);
		}
		return false;
	}
	int Shoot::CalculateDamage(float distance)
	{

		int sign = (rand() % 2);
		if (sign == 0)
		{
			sign = -1;
		}
		else
		{
			sign = 1;
		}
		float random = ((float)(rand() % 25) / 100);
		return (1 + sign * random) * (unit->accuracy * gun->accuracy) * (1 - distance / gun->shootDistance) * gun->damage;
	}
	bool Shoot::IsCanShootToTile(Vector2Int to)
	{
		Vector2Int position = GetGameObject()->GetTransform()->GetPosition();
		if (to.Compare(position))
		{
			return false;
		}
		if (gun->currentAmmo <= 0 || unit->GetTimePoint() < gun->shootTimePoint)
		{
			return false;
		}
		Tile* tile = LevelManager::instance->GetTile(to.X(), to.Y());
		if (tile == nullptr || tile->GetTileType() == wall)
		{
			return false;
		}
		/*
		float distance = position.Increase(GetGameObject()->GetTransform()->GetPosition()).Length();
		if (distance > unit->sightRadius)
		{

		}
		else
		{
		return false;
		}*/
		return true;
	}
	void Shoot::SetGun(Gun* _gun)
	{
		gun = _gun;
	}
	const Gun* Shoot::GetGun()
	{
		return gun;
	}

	void PlayerController::Awake()
	{
		currentPlayerInd = 0;
	}
	Component* PlayerController::Copy()
	{
		return CreateCopy(this);
	}
	void PlayerController::Start()
	{
		GameObject* go = GetGameObject();
		players = go->GetEngineCore()->FindObjectsWithTag(GameValues::playerTag);

		for (int i = 0; i < players.size(); i++)
		{
			players[i]->GetTransform()->SetPosition(Vector2Int(5 + i, 5));
		}
		/*
		players[0]->GetTransform()->SetPosition(Vector2Int(5, 5));
		players[1]->GetTransform()->SetPosition(Vector2Int(6, 6));
		players[2]->GetTransform()->SetPosition(Vector2Int(7, 6));
		players[2]->GetTransform()->SetPosition(Vector2Int(8, 6));*/
	}
	void PlayerController::MakeTurn()
	{
		Unit u;
		Unit* unit;
		for (int i = 0; i < players.size(); i++)
		{
			unit = (Unit*)players[i]->FindComponentOfType(u);
			unit->NewTurn();
		}

		//Цикл, обрабатывающий ввод игрока и отдающий приказы	
		while (true)
		{
			UIManager::instance->UpdateDisplay();
			Command command = UIManager::instance->GetInput();

			if (command == endTurn)
			{
				break;
			}
		}
		GameManager::instance->SetGameState(enemyTurn);
	}
	bool PlayerController::TryMove(MoveDirection moveDirection)
	{
		Vector2Int position = players[currentPlayerInd]->GetTransform()->GetPosition();
		switch (moveDirection)
		{
		case moveUp:
		{
					   position = Vector2Int(position.X(), position.Y() - 1);
					   break;
		}
		case moveDown:
		{
						 position = Vector2Int(position.X(), position.Y() + 1);
						 break;
		}
		case moveLeft:
		{
						 position = Vector2Int(position.X() - 1, position.Y());
						 break;
		}
		case moveRight:
		{
						  position = Vector2Int(position.X() + 1, position.Y());
						  break;
		}
		}
		Unit u;
		Unit* unit = (Unit*)players[currentPlayerInd]->FindComponentOfType(u);
		return unit->TryMove(position);

	}

	void PlayerController::TryShoot(int x, int y)
	{
		Vector2Int position = players[currentPlayerInd]->GetTransform()->GetPosition();
		Vector2Int point(position.X() + x, position.Y() - y);

		Shoot* shoot;
		Shoot s;
		shoot = (Shoot*)players[currentPlayerInd]->FindComponentOfType(s);;
		//Vector2Int point = Vector2Int(0, 0);
		if (shoot->IsCanShootToTile(point))
		{
			shoot->TryShoot(point);
		}
		else
		{
			cout << "Cant shoot \n";
			string str;
			cin >> str;
		}
	}
	int PlayerController::GetActivePlayersNum()
	{
		return players.size();
	}
	string PlayerController::GetPlayerStats(int i)
	{
		Unit u;
		Unit* unit = (Unit*)players[i]->FindComponentOfType(u);

		Shoot s;
		Shoot* shoot = (Shoot*)players[i]->FindComponentOfType(s);

		string stats(to_string(i + 1) + ")" + unit->GetUnitName());
		string uSymbol;
		uSymbol = (unit->GetUnitSymbol());
		stats += " '" + uSymbol + "' Health: " + to_string(unit->GetHealth()) + "/" + to_string(unit->GetMaxHealth()) +
			" Ammo: " + to_string(shoot->GetGun()->currentAmmo) + '/' + to_string(shoot->GetGun()->maxAmmo) + "\n";
		stats += "___Time Points: " + to_string(unit->GetTimePoint()) + "/" + to_string(unit->GetMaxTimePoint()) + " TimePerStep: "
			+ to_string(unit->GetTimePointPerStep()) + " TimePerShoot: " + to_string(shoot->GetGun()->shootTimePoint);
		return stats;
	}
	int PlayerController::GetCurrentPlayerInd()
	{
		return currentPlayerInd;
	}
	void PlayerController::ChooseNextPlayer()
	{
		currentPlayerInd++;
		currentPlayerInd %= GetActivePlayersNum();
	}

	GameManager* GameManager::instance;
	void GameManager::Awake()
	{
		if (instance == nullptr)
		{
			instance = this;
		}
		else
		{
			GetGameObject()->SetActive(false);
		}
		SetGameState(playerTurn);
	}
	Component* GameManager::Copy()
	{
		return CreateCopy(this);
	}
	void GameManager::SetGameState(GameState _gameState)
	{
		gameState = _gameState;
	}
	GameState GameManager::GetGameState()
	{
		return gameState;
	}

	LevelManager* LevelManager::instance;
	void LevelManager::Awake()
	{
		if (instance == nullptr)
		{
			instance = this;
		}
		else
		{
			GetGameObject()->SetActive(false);
		}
		LoadLevel();
	}
	Component* LevelManager::Copy()
	{
		return CreateCopy(this);
	}
	void LevelManager::LoadLevel()
	{
		//Attention



		srand(time(NULL));
		int width = 20;
		int height = 10;
		string map_strings[10] = {
			{ "####################" },
			{ "#..................#" },
			{ "#..****......**....#" },
			{ "#.....*......**....#" },
			{ "#..................#" },
			{ "#..................#" },
			{ "#.*..........**....#" },
			{ "#..*......****.....#" },
			{ "#..*...............#" },
			{ "####################" }
		};
		Tile** tiles;
		tiles = new Tile *[height];
		for (int i = 0; i < height; i++)
		{
			tiles[i] = new Tile[width];
		}

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Tile tile;
				tile.position = Vector2Int(i, j);
				tile.SetTileType(DefineTyleType(map_strings[i][j]));
				tiles[i][j] = tile;
			}
		}
		map.height = height;
		map.width = width;
		map.tiles = tiles;
	}
	TileType LevelManager::DefineTyleType(char ch)
	{
		switch (ch)
		{
		case('.') :
		{
					  return empty;
					  break;
		}
		case('*') :
		{
					  return obstacle;
					  break;
		}
		case('#') :
		{
					  return wall;
					  break;
		}

		default:
			return empty;
			break;
		}
	}
	bool LevelManager::TryMoveObject(Vector2Int position, Transform* _transform)
	{
		if (!(position.X() >= 0 && position.X() < map.width
			&& position.Y() >= 0 && position.Y() < map.height))
		{
			return false;
		}
		if (!map.tiles[position.Y()][position.X()].IsTileAvailableToStep())
		{
			return false;
		}
		map.tiles[_transform->GetPosition().Y()][_transform->GetPosition().X()].RemoveGameObject(_transform->GetGameObject());
		map.tiles[position.Y()][position.X()].AddGameObject(_transform->GetGameObject());
		//_transform->SetPosition(position);
		return true;
	}
	bool LevelManager::ShootToTile(Vector2Int to, int damage)
	{
		map.tiles[to.Y()][to.X()].DamageTile(damage);
		return true;
	}
	Tile* LevelManager::GetTile(int x, int y)
	{
		if (x >= 0 && x < map.width
			&& y >= 0 && y <= map.height)
		{
			return &map.tiles[y][x];
		}
		return nullptr;
	}

	UIManager* UIManager::instance;
	void UIManager::Awake()
	{
		if (instance == nullptr)
		{
			instance = this;
		}
		else
		{
			GetGameObject()->SetActive(false);
		}
		SetDisplayMode(baseMode);
	}
	Component* UIManager::Copy()
	{
		return CreateCopy(this);
	}
	void UIManager::UpdateDisplay()
	{
		system("cls");
		Map* map = &(LevelManager::instance->map);

		for (int i = 0; i < map->height; i++)
		{
			for (int j = 0; j < map->width; j++)
			{
				cout << DefineTileChar(map->tiles[i][j]);
			}
			cout << endl;
		}
		if (GameManager::instance->GetGameState() == playerTurn)
		{
			DisplayPlayerStats();
			if (displayMode == baseMode)
			{
				DisplayPlayerBaseMenu();
			}
			else if (displayMode == shootMode)
			{
				DisplayPlayerShootMenu();
			}
			else if (displayMode == inventoryMode)
			{
				DisplayPlayerInventoryMenu();
			}
		}
	}
	void UIManager::DisplayPlayerBaseMenu()
	{
		cout << endl;
		cout << "WASD. To move" << endl;
		cout << "F.ShootMode" << endl;
		cout << "I.InventoryMode" << endl;
		cout << "C.Change Soldier" << endl;
		cout << "E.End turn" << endl;
	}
	void UIManager::DisplayPlayerShootMenu()
	{
		cout << endl;
		cout << "B.BaseMode" << endl;
		cout << "Enter X and Y displacement from soldier to shoot. Enter 0 reinput coords" << endl;
	}
	void UIManager::DisplayPlayerInventoryMenu()
	{
		cout << endl;
		cout << "WASD. To move" << endl;

	}

	Command UIManager::GetInput()
	{
		Command c;
		while (true)
		{
			string str;
			cin >> str;


			c = DefineEnteredCommand(str);
			if (c != error)
			{
				switch (c)
				{
				case shoot:
				{
							  InShootMenu();
							  return base;
							  break;
				}
				case inventory:
				{
								  InInventoryMenu();
								  return base;
								  break;
				}
				default:
				{
						   return c;
				}
				}

			}
			else if (str.size() == 1)
			{
				InBaseMenu(str);
				return base;
				break;
			}
			else
			{
				InputErrorMessage();
			}
		}
	}
	bool UIManager::IsGoToMoveMenu(string str)
	{
		if (str.size() == 1 && tolower(str[0]) == 'b')
		{
			return true;
		}
		return false;
	}

	void UIManager::InBaseMenu(string str = "")
	{
		SetDisplayMode(baseMode);
		UpdateDisplay();

		while (true)
		{
			if (str.empty())
			{
				cin >> str;
			}
			if (str.size() != 1)
			{
				InputErrorMessage();
				return;
			}
			else
			{
				char ch = tolower(str[0]);
				MoveDirection moveDirection;
				switch (ch)
				{
				case 'w':
				{
							moveDirection = moveUp;
							break;
				}
				case 's':
				{
							moveDirection = moveDown;
							break;
				}
				case 'a':
				{
							moveDirection = moveLeft;
							break;
				}
				case 'd':
				{
							moveDirection = moveRight;
							break;
				}
				case 'c':
				{
							playerController->ChooseNextPlayer();
							return;
							break;
				}
				default:
				{
						   InputErrorMessage();
						   return;
				}
				}

				if (playerController->TryMove(moveDirection))
				{

				}
				else
				{
					cout << "Can't move\n";
					char a;
					cin >> a;
				}

				return;
			}
		}
	}

	void UIManager::InShootMenu()
	{
		SetDisplayMode(shootMode);
		UpdateDisplay();
		string str;
		while (true)
		{
			cout << "X: ";
			cin >> str;
			if (IsGoToMoveMenu(str))
			{
				SetDisplayMode(baseMode);
				return;
			}
			int x = atoi(str.c_str());

			while (true)
			{
				//cout << "X: " << x << endl;
				cout << "Y: ";
				cin >> str;
				if (str.size() == 1 && tolower(str[0]) == 'b')
				{
					SetDisplayMode(baseMode);
					return;
				}
				int y = atoi(str.c_str());
				if (y == 0 && x == 0)
				{
					InputErrorMessage();
					break;
				}
				else
				{

					playerController->TryShoot(x, y);
					//
					SetDisplayMode(baseMode);
					return;
				}
			}

		}
	}
	void UIManager::InInventoryMenu()
	{
		/*
		SetDisplayMode(inventoryMode);
		UpdateDisplay();
		string str;
		while (true)
		{
		cin >> str;
		if (str.size() == 1 && tolower(str[0]) == 'b')
		{
		SetDisplayMode(baseMode);
		}
		int x = atoi(str.c_str());
		if (x != 0)
		{
		while (true)
		{
		cin >> str;
		if (str.size() == 1 && tolower(str[0]) == 'b')
		{
		SetDisplayMode(baseMode);
		}
		int y = atoi(str.c_str());
		if (y != 0)
		{
		PlayerController pc;
		PlayerController* playerController = (PlayerController*)GetGameObject()->GetEngineCore()->FindObjectOfType(pc);
		playerController->Shoot(x, y);

		return;
		}
		}
		}
		else
		{
		cout << "Incorrect Input. Try again." << endl;
		}
		}*/
	}
	void UIManager::Start()
	{
		PlayerController pc;
		playerController = (PlayerController*)GetGameObject()->GetEngineCore()->FindObjectOfType(pc);
	}

	void UIManager::DisplayPlayerStats()
	{
		cout << "---" << endl;
		for (int i = 0; i < playerController->GetActivePlayersNum(); i++)
		{
			cout << playerController->GetPlayerStats(i) << endl;
			cout << "---" << endl;
		}
		cout << "__CurrentPlayer: " << playerController->GetCurrentPlayerInd() + 1 << endl;
		cout << endl;
	}

	DisplayMode UIManager::GetDisplayMode()
	{
		return displayMode;
	}
	void UIManager::SetDisplayMode(DisplayMode mode)
	{
		displayMode = mode;
	}

	char UIManager::DefineTileChar(Tile tile)
	{
		return tile.GetTileImage();
	}

	Command UIManager::DefineEnteredCommand(string str)
	{
		if (str.size() == 1)
		{
			char ch = tolower(str[0]);
			switch (ch)
			{
			case'b':
			{

					   return base;

			}
			case'f':
			{

					   return shoot;

			}
			case'i':
			{

					   return inventory;

			}
			case 'e':
			{
						return endTurn;
			}
			default:
				return error;
				break;
			}
		}
	}
	void UIManager::InputErrorMessage()
	{
		string s;
		cout << "Incorrect Input. Try again." << endl;
		cin >> s;
	}

	GameObject* EngineCore::Instantiate(const GameObject& prefab)
	{
		GameObject* go = new GameObject(prefab);
		go->SetEngineCore(this);
		gameObjects.push_back(go);

		return go;
	}
	void EngineCore::Destroy(GameObject* go)
	{
		go->SetActive(false);
		//
	}
	void EngineCore::Update()
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i]->IsActive())
			{
				gameObjects[i]->Update();
			}
		}
	}
	GameObject* EngineCore::Find(string name)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i]->name == name)
			{
				return gameObjects[i];
			}
		}
		return nullptr;
	}
	vector<GameObject*> EngineCore::FindObjectsWithTag(string _tag) const
	{
		vector<GameObject*> objects;
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i]->GetTag() == _tag)
			{
				objects.push_back(gameObjects[i]);
			}
		}
		return objects;
	}
	/*
	Component* EngineCore::FindObjectOfType(Component component)
	{

	}*/
	void EngineCore::StartGame()
	{
		TestActivator();

		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i]->IsActive())
			{
				gameObjects[i]->Awake();
			}
		}
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i]->IsActive())
			{
				gameObjects[i]->Start();
			}
		}

		while (true)
		{
			Update();
		}

	}
	void EngineCore::TestActivator()
	{
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

		Instantiate(mainManager);




		GameObject player("Soldier");
		player.SetTag(GameValues::playerTag);
		Unit* a = new Unit();
		a->Initialize("Rambo", 'R', 120, 12, 3, 6, 0.8);



		Gun* gun = new Gun();
		gun->accuracy = 0.9f;
		gun->currentAmmo = 10;
		gun->damage = 40;
		gun->maxAmmo = 10;
		gun->reloadTimePoint = 2;
		gun->shootDistance = 3;
		gun->shootTimePoint = 3;

		Shoot* shoot = new Shoot();
		shoot->SetGun(gun);

		player.AddComponent(a);
		player.AddComponent(shoot);
		Instantiate(player);

		a->Initialize("Terminator", 'T', 240, 12, 5, 10, 1);
		Instantiate(player);

		a->Initialize("JChan", 'C', 80, 16, 3, 4, 0.5);
		Instantiate(player);

		a->Initialize("Droid", 'D', 12, 20, 1, 7, 0.1);
		Instantiate(player);
	}


	void GameLogicManager::Awake()
	{
		PlayerController pc;
		playerController = (PlayerController*)(GetGameObject()->GetEngineCore()->FindObjectOfType(pc));
	}
	Component* GameLogicManager::Copy()
	{
		return CreateCopy(this);
	}
	void GameLogicManager::Update()
	{
		GameManager::instance->SetGameState(playerTurn);
		playerController->MakeTurn();
		GameManager::instance->SetGameState(enemyTurn);
		//enemyTurn
	}

	Command DefineCommand(char ch)
	{

		ch = tolower(ch);
		switch (ch)
		{

		case 'f':
		{
					return shoot;
					break;
		}
		case 'i':
		{
					return inventory;
					break;
		}
		case 'c':
		{
					return changePlayer;
					break;
		}
		default:
		{
				   return error;
		}
		}
	}
	Command GetCommand()
	{
		string str;
		cin >> str;
		if (str.size() != 1)
		{
			return error;
		}
		else
		{
			return DefineCommand(str[0]);
		}

	}

}