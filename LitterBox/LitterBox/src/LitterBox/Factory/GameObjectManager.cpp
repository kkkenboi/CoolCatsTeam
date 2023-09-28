#include "GameObjectManager.h"
#include "Components.h"

namespace LB
{
	/***************************************************************************************************
	*
	* Game Object Manager
	*
	***************************************************************************************************/

	GameObjectManager* GOMANAGER = nullptr;

	GameObjectManager::GameObjectManager()
	{
		SetSystemName("GameObjectManager System");

		if (!GOMANAGER)
		{
			GOMANAGER = this;
		}
		else
		{
			std::cerr << "GameObjectManager already exist\n";
		}
	}

	std::vector<GameObject*> GameObjectManager::GetGameObjects() const
	{
		return m_GameObjects;
	}

	void GameObjectManager::AddGameObject(GameObject* gameObject)
	{
		m_GameObjects.push_back(gameObject);
	}

	void GameObjectManager::DestroyAllGOs()
	{
		std::cout << "Entering!!\n";
		// Destroying components in game objects
		for (size_t i{}; i < m_GameObjects.size(); ++i)
		{
			//int componentSize = m_GameObjects[i]->GetComponents().size();
			for (auto const& component : m_GameObjects[i]->GetComponents())
			{
				component.second->Destroy();
				// Delete the memory allocated for the component
				delete component.second;

				//// --Change this to check the delete for which component--
				std::cout << "One GO component deleted from game object " << m_GameObjects[i]->GetID() << "\n";
			}


			std::cout << "GO " << m_GameObjects[i]->GetID() << " has been deleted\n";

			if (i + 1 == m_GameObjects.size())
			{
				std::cout << "GO's components all deleted\n";
			}
		}

		// Destroying gameobjects
		int gameObjSize = m_GameObjects.size();
		for (int i{}; i < gameObjSize; ++i)
		{
			delete m_GameObjects[i];
		}

		// Set size of game objects to empty
		m_GameObjects.clear();

		std::cout << m_GameObjects.size() << std::endl;

		std::cout << "All GOs deleted\n";
	}

	void GameObjectManager::Destroy()
	{
		DestroyAllGOs();
		std::cout << "GOM destructed\n";
	}

	/***************************************************************************************************
	*
	* Game Object
	*
	***************************************************************************************************/
	GameObject::GameObject(int ID) : m_Components{}, isActive{ false }, m_ID{ ID }
	{
		std::cout << "GO constructed\n";
	}
	GameObject::GameObject() : m_Components{}, isActive{ false }, m_ID{}
	{
		std::cout << "GO constructed\n";
	}

	//GameObject::GameObject(std::vector<IComponent*> const& componentList) : m_Components{ componentList }, isActive{ false }
	//{
	//	std::cout << "GO parameterized constructor\n";
	//};

	GameObject::~GameObject()
	{
		// Should delete all of the components
		std::cout << "GO destructed\n";
	}

	std::unordered_map<std::string, IComponent*> GameObject::GetComponents()
	{
		return m_Components;
	}

	void GameObject::AddComponent(std::string name, IComponent* component)
	{
		component->gameObj = this;
		m_Components[name] = component;
	}

	void GameObject::StartComponents()
	{
		for (auto const& component : m_Components)
		{
			component.second->Initialise();
		}
	}

	int GameObject::GetID() const
	{
		return m_ID;
	}
	void GameObject::SetID(int ID)
	{
		m_ID = ID;
	}



}