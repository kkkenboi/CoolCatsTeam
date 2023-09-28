#pragma once
#include "pch.h"
#include "LitterBox/Core/System.h"
#include <initializer_list>

namespace LB
{
	class IComponent;
	class GameObject
	{
	public:
		GameObject();
		GameObject(int ID);
		~GameObject();

		template <typename T>
		T* GetComponent(std::string name)
		{
			return static_cast<T*>(m_Components[name]);
		}

		std::unordered_map<std::string, IComponent*> GetComponents();
		void AddComponent(std::string name, IComponent* component);

		void StartComponents();

		int GetID() const;
		void SetID(int ID);

	private:
		int m_ID;
		std::unordered_map<std::string, IComponent*> m_Components;
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

