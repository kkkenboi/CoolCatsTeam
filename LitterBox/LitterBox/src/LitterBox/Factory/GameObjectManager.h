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
	};

	class GameObjectManager
	{
	public:

		GameObjectManager();
		~GameObjectManager();

		std::vector<GameObject*> m_GameObjects;
		std::vector<GameObject> m_Pool;

	private:


	};

	//extern GameObjectManager* GOMANAGER;
}

