#include "GameObjectManager.h"

namespace LB
{
	GameObjectManager* GOMANAGER = nullptr;

	GameObjectManager::GameObjectManager()
	{
		if (!GOMANAGER)
		{
			GOMANAGER = this;
		}
		else
		{
			std::cerr << "GameObjectManager already exist\n";
		}
	}

	void GameObjectManager::DestroyAllGOs()
	{
		for (size_t i{}; i < m_GameObjects.size(); ++i)
		{
			for (size_t j{}; j < m_GameObjects[i]->m_Components.size(); ++j)
			{
				delete m_GameObjects[i]->m_Components[j];
				std::cout << "One GO component deleted from game object " << i << "\n";
			}

			std::cout << "GO " << i << " has been deleted\n";

			if (i + 1 == m_GameObjects.size())
			{
				std::cout << "GO's components all deleted\n";
			}
		}

		std::cout << "All GOs deleted\n";
	}

	GameObjectManager::~GameObjectManager()
	{
		DestroyAllGOs();
		std::cout << "GO destructed\n";
	}

	GameObject::GameObject() : m_Components{}, isActive{ false }
	{
		std::cout << "GO constructed\n";
	}

	GameObject::GameObject(std::vector<IComponent*> const& componentList) : m_Components{ componentList }, isActive{ false }
	{
		std::cout << "GO parameterized constructor\n";
	};

	GameObject::~GameObject()
	{
		// Should delete all of the components
		std::cout << "GO destructed\n";
	}

}