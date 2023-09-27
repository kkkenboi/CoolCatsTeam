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

		std::vector<IComponent*> GetComponents() const;
		void AddComponent(IComponent* component);

		void StartComponents();

		int GetID() const;
		void SetID(int ID);

	private:
		int m_ID;
		std::vector<IComponent*> m_Components;
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

