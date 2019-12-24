#pragma once
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
//#include "pch.h"
//#include "Unit.h"
#include <ctime>
#include <iostream>

using namespace std;
//using namespace Operation;
namespace CompEngine
{
	/*
	class Unit;
	class PlayerController;
	class LevelManager;
	class GameManager;
	class UIManager;*/
	class GameObject;
	class EngineCore;
	class Transform;
	class Vector2Int;

	namespace GameValues
	{

		static const string playerTag = "Player";

	};
	enum TileType { empty, obstacle, wall };
	enum GameState { playerTurn, enemyTurn };
	enum Command { error, base, shoot, inventory, changePlayer, endTurn };
	enum MoveDirection { stay, moveUp, moveDown, moveLeft, moveRight };
	enum DisplayMode { mainMenu, baseMode, shootMode, inventoryMode };

	class Component
	{
	public:
		void SetActive(bool);
		bool IsActive();
		GameObject* GetGameObject();
		//Att
		void SetGameObject(GameObject*);
		virtual void Awake() {
			//name = typeid(*this).name();
			return;
		}
		virtual void Start() { return; }
		virtual void Update() { return; }
		virtual Component* Copy() = 0;
	protected:
		template <typename T>
		Component* CreateCopy(T* component)
		{
			T* copy = new T;
			*copy = *component;
			return (Component*)copy;
		}
	private:
		bool isActive = true;
		GameObject* gameObject;
	};

	class  Vector2Int
	{
	public:
		Vector2Int();
		Vector2Int(int, int);
		static Vector2Int Zero();
		int X();
		int Y();
		float Length();
		Vector2Int Increase(Vector2Int);
		bool Compare(const Vector2Int&);
	private:
		int x;
		int y;
	};
	class  Transform
	{
	public:
		Transform(GameObject*);
		Vector2Int GetPosition();
		bool SetPosition(Vector2Int);
		GameObject* GetGameObject();
		//att




	private:
		void SetGameObject(GameObject*);
		Vector2Int position;
		GameObject* gameObject;
	};
	class GameObject
	{
	public:
		string name;

		void SetActive(bool);
		bool const IsActive();
		GameObject(string);
		GameObject(const GameObject& obj);// const EngineCore&, string = "GameObject");
		~GameObject();
		void AddComponent(Component*);
		Transform* GetTransform();
		void Update();
		void Awake();
		void Start();

		template <typename T>
		Component* FindComponentOfType(T component)
		{
			for (int i = 0; i < components.size(); i++)
			{
				string leftName = typeid(*(components[i])).name();
				string rightname = typeid(component).name();

				if (typeid(*(components[i])).name() == typeid(component).name())
				{
					return components[i];
				}
			}
			return nullptr;
		}

		string GetTag();
		void SetTag(string);
		EngineCore* GetEngineCore();
		void SetEngineCore(EngineCore*);
	private:
		string tag;
		bool isActive;
		Transform* transform;
		vector<Component*> components;
		EngineCore* engineCore;
	};
	class Tile
	{
	public:
		Vector2Int position;

		void SetTileType(TileType);
		TileType GetTileType();
		bool IsTileAvailableToStep();
		void AddGameObject(GameObject*);
		void RemoveGameObject(GameObject*);
		char GetTileImage();
		void DamageTile(int damage);
	private:
		vector<GameObject*> gameObjects;
		TileType tileType;
	};
	class Map
	{
	public:
		Tile** tiles;
		int width;
		int height;
	};

	class EngineCore
	{
	public:
		GameObject* Instantiate(const GameObject& prefab);
		void Destroy(GameObject*);

		void StartGame();
		GameObject* Find(string);
		vector<GameObject*> FindObjectsWithTag(string) const;

		template <typename T>
		Component* FindObjectOfType(T component)
		{
			for (int i = 0; i < gameObjects.size(); i++)
			{
				Component* comp = gameObjects[i]->FindComponentOfType(component);
				if (comp != nullptr)
				{
					return comp;
				}
			}
			return nullptr;
		}

	private:
		void Update();
		void TestActivator();
		vector<GameObject*> gameObjects;
	};





	class Unit : public Component
	{
	public:
		bool TryMove(Vector2Int);
		void TakeDamage(int);
		void Initialize(string _unitName, char _unitSymbol, int _maxHealth, int _maxTimePoint, int _timePointPerStep, int _sightRadius, float _accuracy);
		void Destruct();
		Component* Copy();


		char GetUnitSymbol();
		string GetUnitName();
		int GetHealth();
		int GetMaxHealth();
		int GetTimePoint();
		int GetMaxTimePoint();
		bool DecreaseTimePoint(int);
		int GetTimePointPerStep();
		void NewTurn();

		int sightRadius;
		float accuracy;
	private:
		string unitName;
		char unitSymbol = '?';
		int health;
		int maxHealth;
		int timePoint;
		int maxTimePoint;
		int timePointPerStep;


	};

	class Gun
	{
	public:
		int damage;
		float accuracy;
		int shootDistance;
		int reloadTimePoint;
		int shootTimePoint;
		int maxAmmo;
		int currentAmmo;
	private:

	};

	class Shoot : public Component
	{
	public:
		void Start();
		bool TryShoot(Vector2Int position);
		bool IsCanShootToTile(Vector2Int to);
		int CalculateDamage(float distance);
		Component* Copy();

		void SetGun(Gun*);
		const Gun* GetGun();
	private:
		Unit* unit;
		Gun* gun;
	};



	class PlayerController : public Component
	{
	public:
		void Awake();
		void Start();
		Component* Copy();
		//void Update();
		void MakeTurn();
		void TryShoot(int, int);
		bool TryMove(MoveDirection);

		int GetActivePlayersNum();
		string GetPlayerStats(int);
		int GetCurrentPlayerInd();
		void ChooseNextPlayer();
	private:
		int currentPlayerInd;
		vector<GameObject*> players;
	};

	class GameManager : public Component
	{
	public:
		static GameManager* instance;
		void Awake();
		Component* Copy();
		//void Update();
		void SetGameState(GameState _gameState);
		GameState GetGameState();
	private:
		GameState gameState;
	};


	class LevelManager : public Component
	{
	public:
		static LevelManager* instance;
		void Awake();
		Component* Copy();
		void LoadLevel();
		TileType DefineTyleType(char);
		bool TryMoveObject(Vector2Int, Transform*);
		bool ShootToTile(Vector2Int to, int damage);
		Tile* GetTile(int x, int y);
		Map map;
	private:

	};


	class UIManager : public Component
	{
	public:
		void Awake();
		void Start();
		Component* Copy();
		//void Update();
		void UpdateDisplay();
		Command GetInput();
		static UIManager* instance;
		DisplayMode GetDisplayMode();
	private:
		void SetDisplayMode(DisplayMode);
		DisplayMode displayMode;
		void DisplayPlayerBaseMenu();
		void DisplayPlayerShootMenu();
		void DisplayPlayerInventoryMenu();

		void DisplayPlayerStats();

		void InBaseMenu(string str);
		void InShootMenu();
		void InInventoryMenu();
		bool IsGoToMoveMenu(string);
		Command DefineEnteredCommand(string);
		char DefineTileChar(Tile tile);

		void InputErrorMessage();
		PlayerController* playerController;
	};

	class GameLogicManager : public Component
	{
	public:
		void Awake();
		Component* Copy();
		void Update();
	private:
		PlayerController* playerController;
		//EnemyController
	};



	Command DefineCommand(char ch);
	Command GetCommand();

	template<class T>
	class Queue
	{
	public:
		int delta;
		int capacity;

		Queue(const Queue &q)
		{
			capacity = q.capacity;
			delta = q.delta;
			a = new T[capacity];
			start = q.start;
			end = q.end;
			n = q.n;
			for (int i = 0; i < q.Size(); i++)
			{
				a[i] = q[i];
			}
		}
		Queue(Queue &&q)
		{
			capacity = q.capacity;
			delta = q.delta;
			start = q.start;
			end = q.end;
			n = q.n;

			T* temp = a;
			a = q.a;
			q.a = temp;
		}

		Queue(int startCapacity = 10, int _delta = 10)//, invalid_argument, overflow_error, underflow_error;
		{
			if (startCapacity <= 0 || _delta <= 0)
			{
				invalid_argument err("Can't create queue with that start capacity and delta");

				throw err;
			}
			delta = _delta;
			capacity = startCapacity;
			a = new T[startCapacity];
			if (a == nullptr)
			{
				bad_alloc err;// ("Can't create queue");
				throw error;
			}
			start = 0;
			end = 0;
			n = 0;
		}
		~Queue()
		{
			delete[] a;
		}

		void Push(T t)
		{
			if (n < capacity)
			{
				a[end] = t;
				end = (end + 1) % capacity;
				n++;
			}
			else
			{
				T* b = new T[capacity + delta];
				int i = start;
				int j = 0;
				while (n != 0)
				{
					b[j] = a[i];
					j++;
					i = (i + 1) % capacity;
					n--;
				}
				start = 0;
				end = capacity;
				n = capacity;
				capacity += delta;
				delete[]a;
				a = b;

				a[end] = t;
				end = (end + 1) % capacity;
				n++;
			}
		}
		T Pop()
		{
			if (n == 0)
			{
				underflow_error err("Queue is empty");
				throw err;
			}
			else
			{
				T t = a[start];
				start = (start + 1) % capacity;
				n--;
				return t;
			}
		}
		int Size()
		{
			return n;
			if (start == end)
			{
				return 0;
			}
			else if (start < end)
			{
				return (end - start);
			}
			else
			{
				return (capacity - start) + end;
			}
		}

	private:
		int start;
		int end;
		T* a;
		int n;
	};
}