#pragma once
#include "pch.h"
#include "LitterBox/Core/System.h"

namespace LB
{
	class IComponent;
	class GameObject
	{
	public:
		GameObject();
		GameObject(int ID);
		~GameObject();

		IComponent* GetComponent(std::string name);
		std::map<std::string, IComponent*> GetComponents();
		void AddComponent(std::string name, IComponent* component);

		void StartComponents();

		int GetID() const;
		void SetID(int ID);

	private:
		int m_ID;
		std::map<std::string, IComponent*> m_Components;
		bool isActive;
	};

	class GameObjectManager : public ISystem
	{
	public:

		GameObjectManager();
		void Destroy() override;

		std::vector<GameObject*> GetGameObjects() const;
		void AddGameObject(GameObject* gameObject);

		void DestroyAllGOs();
	private:
		std::vector<GameObject*> m_GameObjects;
	};

	extern GameObjectManager* GOMANAGER;
}

