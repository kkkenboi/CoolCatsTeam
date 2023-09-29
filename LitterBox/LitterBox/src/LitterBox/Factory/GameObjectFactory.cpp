/*!************************************************************************
 \file				GameObjectFactory.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				
 This file contains functions definitions of the FactorySystem class that 
 creates a GameObject with different components.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "GameObjectFactory.h"
#include "Components.h"

namespace LB
{
	FactorySystem* FACTORY = nullptr;

	/*!***********************************************************************
	 \brief
	 Initialises the factory with known component types to make ComponentMakers
	 of those types
	*************************************************************************/
	FactorySystem::FactorySystem()
	{
		// Ensure singleton
		if (!FACTORY)
		{
			FACTORY = this;
		}
		else
		{
			std::cerr << "Factory already exist\n";
		}

		m_LastObjID = 0;

		// Deserialise the data file and initialise ComponentMakers
		// 
		CreateComponentMaker(CPTransform);
		CreateComponentMaker(CPRender);
		CreateComponentMaker(CPRigidBody);

		DebuggerLog("Factory Initialised");
	}

	/*!***********************************************************************
	 \brief
	 Initialise the factory and sets the system's name

	 \return
	 Nothing
	*************************************************************************/
	void FactorySystem::Initialize()
	{ 
		SetSystemName("GameObject Factory System"); 
	}

	/*!***********************************************************************
	 \brief
	 Updates the factory

	 \return
	 Nothing
	*************************************************************************/
	void FactorySystem::Update()
	{

		if (m_ToUpdate)
		{


			// Send all of the game objects to the GameObjectManager's vector or something
			// Send message to the GameObjectManager to add objects to its pool of game objects
			// What about pools?

			// For now just make it be the same
			//GOMANAGER->m_Pool = m_WaitingList;
			DebuggerLog("Factory Updated");
		}

		m_ToUpdate = false;
	}


	/*!***********************************************************************
	 \brief
	 Destroys the factory

	 \return
	 Nothing
	*************************************************************************/
	void FactorySystem::Destroy()
	{
		DeleteAllCMs(m_ComponentMakers);
	}

	/*!***********************************************************************
	 \brief
	 Initialise this ComponentMaker of their type, eg. Physics / Graphics

	 \return
	 Nothing
	*************************************************************************/
	void FactorySystem::InitCM(const std::string& name, ComponentMaker* newComponent)
	{
		m_ComponentMakers[name] = newComponent;
	}


	/*!***********************************************************************
	 \brief
	 Delete all ComponentMakers from all GameObjects

	 \return
	 Nothing
	*************************************************************************/
	void FactorySystem::DeleteAllCMs(std::map<std::string, ComponentMaker*> ComponentMakers)
	{
		for (std::map<std::string, ComponentMaker*>::iterator it = ComponentMakers.begin(); it != ComponentMakers.end(); ++it)
		{
			delete it->second;
		}

		DebuggerLog("ComponentMakers all deleted");
	}

	/*!***********************************************************************
	 \brief
	 Spawns a GameObject with no components but specified position

	 \return
	 A pointer to the GameObject
	*************************************************************************/
	GameObject* FactorySystem::SpawnGameObject(Vec2<float> pos)
	{
		return SpawnGameObject({}, pos);
	}

	/*!***********************************************************************
	 \brief
	 Spawns a GameObject with specified components and position

	 \return
	 A pointer to the GameObject
	*************************************************************************/
	GameObject* FactorySystem::SpawnGameObject(std::initializer_list<std::string> components, Vec2<float> pos)
	{
		// Creating the game object
		GameObject* gameObj = FACTORY->CreateGameObject();
		if (gameObj->GetID() == 0) 		// ID only starts at 1
		{
			gameObj->SetID(FACTORY->GetLastObjID());
		}

		// Every gameobject must have a transform!!!
		gameObj->AddComponent("CPTransform", FACTORY->GetCMs()["CPTransform"]->Create());
		gameObj->GetComponent<CPTransform>("CPTransform")->SetPosition(pos);

		for (std::string component : components)
		{
			gameObj->AddComponent(component, FACTORY->GetCMs()[component]->Create());
		}
		gameObj->StartComponents();

		// Sends game object to the Game Object Manager
		// For now, push back in the function to go manager,
		// However, in the future we might need to change when we need to render a lot in one go
		// or send a equal amount of game objects at one go
		// Might be redundant too because we should initialize a pool at the start
		GOMANAGER->AddGameObject(gameObj);
		return gameObj;
	}
	/*!***********************************************************************
	 \brief
	 Spawns a GameObject with the exact same components as the provided GameObject

	 \return
	 A pointer to the GameObject
	*************************************************************************/
	GameObject* FactorySystem::SpawnGameObject(GameObject* prefab)
	{
		GameObject* clone = FACTORY->CreateGameObject();
		if (clone->GetID() == 0) 		// ID only starts at 1
		{
			clone->SetID(FACTORY->GetLastObjID());
		}
		//We loop through all the components in the prefab
		for (auto& elem : prefab->GetComponents())
		{	//Then we add it to our clone
			clone->AddComponent(elem.first,FACTORY->GetCMs()[elem.first]->Create());
		}
		//This copies the data from our prefab components over to the clone
		clone->SetComponents(prefab->GetComponents());
		//Then we initialise the data for the clone
		clone->StartComponents();
		GOMANAGER->AddGameObject(clone);
		return clone;
	}

	/*!***********************************************************************
	 \brief
	 Creates a empty GameObject with no components

	 \return
	 A pointer to an empty GameObject pointer
	*************************************************************************/
	GameObject* FactorySystem::CreateGameObject()
	{
		++m_LastObjID;

		DebuggerLog("GO" + std::to_string(m_LastObjID) + " has been created");

		// Original return
		return new GameObject(FACTORY->GetLastObjID());
	}

	/*!***********************************************************************
	 \brief
	 Gets all of the ComponentMakers

	 \return
	 Map of names of ComponentMakers and ComponentMakers
	*************************************************************************/
	std::map<std::string, ComponentMaker*> FactorySystem::GetCMs() const
	{
		return m_ComponentMakers;
	}

	/*!***********************************************************************
	 \brief
	 Gets the last created GameObject's ID.

	 \return
	 Last created GameObject's ID
	*************************************************************************/
	int FactorySystem::GetLastObjID() const
	{
		return m_LastObjID;
	}

}