/*!************************************************************************
 \file				GameObjectManager.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief

 This file contains functions definition of the GameObject and GameObjectManager 
 class. The GameObject acts as a container to hold all of the different components 
 for the components to interact with each other during the gameloop.

 The GameObjectManager manages all of the GameObject's lifetime throughout the
 entire application.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "GameObjectManager.h"
#include "Components.h"

namespace LB
{
	/***************************************************************************************************
	*
	* Game Object
	*
	***************************************************************************************************/
	
	/*!***********************************************************************
	 \brief
	 Creates a GameObject
	*************************************************************************/
	GameObject::GameObject() : m_Components{}, m_IsActive{ false }, m_ID{} {}

	/*!***********************************************************************
	 \brief
	 Creates a GameObject with an ID
	*************************************************************************/
	GameObject::GameObject(int ID) : m_Components{}, m_IsActive{ false }, m_ID{ ID } {}

	/*!***********************************************************************
	 \brief
	 Destroys a GameObject
	*************************************************************************/
	GameObject::~GameObject() {}

	/*!***********************************************************************
	 \brief
	 Gets all the components of the GameObject

	 \return
	 Map to the GameObject's components
	*************************************************************************/
	std::unordered_map<std::string, IComponent*> GameObject::GetComponents()
	{
		return m_Components;
	}

	/*!***********************************************************************
	 \brief
	 Adds a component to the GameObject

	 \return
	 Nothing
	*************************************************************************/
	void GameObject::AddComponent(std::string name, IComponent* component)
	{
		component->gameObj = this;
		m_Components[name] = component;
	}

	/*!***********************************************************************
	 \brief
	 Initialises all of the components in the GameObject

	 \return
	 Nothing
	*************************************************************************/
	void GameObject::StartComponents()
	{
		for (auto const& component : m_Components)
		{
			component.second->Initialise();
		}
	}

	/*!***********************************************************************
	 \brief
	 Gets the ID of the GameObject

	 \return
	 ID of the GameObject
	 *************************************************************************/
	int GameObject::GetID() const
	{
		return m_ID;
	}

	/*!***********************************************************************
	 \brief
	 Sets the ID of the GameObject

	 \return
	 Nothing
	*************************************************************************/
	void GameObject::SetID(int ID)
	{
		m_ID = ID;
	}

	/***************************************************************************************************
	*
	* Game Object Manager
	*
	***************************************************************************************************/

	GameObjectManager* GOMANAGER = nullptr;

	/*!***********************************************************************
	 \brief
	 Creates the GameObjectManager
	*************************************************************************/
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

	/*!***********************************************************************
	 \brief
	 Destroys all of the GameObjects and signifies end of lifecycle of
	 GameObjectManager

	 \return
	 Nothing
	*************************************************************************/
	void GameObjectManager::Destroy()
	{
		DestroyAllGOs();
		std::cout << "GOM destructed\n";
	}

	/*!***********************************************************************
	 \brief
	 Gets all of the current GameObjects the GameObjectManager is managing

	 \return
	 All current GameObjects
	*************************************************************************/
	std::vector<GameObject*> GameObjectManager::GetGameObjects() const
	{
		return m_GameObjects;
	}

	/*!***********************************************************************
	 \brief
	 Adds a GameObject to the current pool of GameObjects

	 \return
	 Nothing
	*************************************************************************/
	void GameObjectManager::AddGameObject(GameObject* gameObject)
	{
		m_GameObjects.push_back(gameObject);
	}

	/*!***********************************************************************
	 \brief
	 Destroys all of the GameObjects

	 \return
	 Nothing
	*************************************************************************/
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
		size_t gameObjSize = m_GameObjects.size();
		for (size_t i{}; i < gameObjSize; ++i)
		{
			delete m_GameObjects[i];
		}

		// Set size of game objects to empty
		m_GameObjects.clear();

		std::cout << m_GameObjects.size() << std::endl;

		std::cout << "All GOs deleted\n";
	}
}