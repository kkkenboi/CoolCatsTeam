#pragma once
#include "pch.h"
#include "LitterBox/Core/System.h"
#include "Components.h"

namespace LB
{
	class GameObject
	{
	public:

		GameObject();
		GameObject(std::vector<IComponent*> const& componentList);
		~GameObject();
		std::vector<IComponent*> m_Components;

	private:
		bool isActive;
	};

	class GameObjectManager : public ISystem
	{
	public:

		GameObjectManager();
		~GameObjectManager();

		std::string GetName() override { return "GameManager"; }

		void DestroyAllGOs();

		std::vector<GameObject*> m_GameObjects;

	private:
	};

	extern GameObjectManager* GOMANAGER;
}

