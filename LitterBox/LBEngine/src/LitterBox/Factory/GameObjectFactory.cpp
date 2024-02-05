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
#include "LitterBox/Scene/SceneManager.h"

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
			FACTORY = this;
		else
			DebuggerLogErrorFormat("Factory already exist");

		m_LastObjID = 0;

		// Deserialise the data file and initialise ComponentMakers
		// 
		CreateComponentMaker(CPTransform,	C_CPTransform);
		CreateComponentMaker(CPRender,		C_CPRender);
		CreateComponentMaker(CPRigidBody,	C_CPRigidBody);
		CreateComponentMaker(CPCollider,	C_CPCollider);
		CreateComponentMaker(CPScript,		C_CPScript);
		CreateComponentMaker(CPScriptCPP,	C_CPScriptCPP);
		CreateComponentMaker(CPAudioSource, C_CPAudioSource);
		CreateComponentMaker(CPText, 		C_CPText);
		CreateComponentMaker(CPAnimator,	C_CPAnimator);
		CreateComponentMaker(CPParticle,	C_CPParticle);

		DebuggerLog("Factory Initialised");
	}

	/*!***********************************************************************
	 \brief
	 Initialise the factory and sets the system's name
	*************************************************************************/
	void FactorySystem::Initialize()
	{ 
		SetSystemName("GameObject Factory System"); 

		// Update scene loaded
		SCENEMANAGER->onNewSceneLoad.Subscribe(LB::UpdateLoadedScene);
	}

	/*!***********************************************************************
	 \brief
	 Updates the factory
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
	*************************************************************************/
	void FactorySystem::Destroy()
	{
		DeleteAllCMs();
	}

	/*!***********************************************************************
	 \brief
	 Initialise this ComponentMaker of their type, eg. Physics / Graphics
	*************************************************************************/
	void FactorySystem::InitCM(const ComponentTypeID& name, ComponentMaker* newComponent)
	{
		m_ComponentMakers[name] = newComponent;
	}

	/*!***********************************************************************
	 \brief
	 Delete all ComponentMakers from all GameObjects
	*************************************************************************/
	void FactorySystem::DeleteAllCMs()
	{
		for (std::map<ComponentTypeID, ComponentMaker*>::iterator it = m_ComponentMakers.begin(); it != m_ComponentMakers.end(); ++it)
		{
			delete it->second;
		}

		DebuggerLog("ComponentMakers all deleted");
	}

	/*!***********************************************************************
	 \brief
	 Spawns a GameObject with no components but specified position
	*************************************************************************/
	GameObject* FactorySystem::SpawnGameObject(Vec2<float> pos, GOSpawnType spawnType)
	{
		return SpawnGameObject({}, pos, spawnType);
	}

	/*!***********************************************************************
	 \brief
	 Spawns a GameObject with components but no specified position
	*************************************************************************/
	GameObject* FactorySystem::SpawnGameObject(std::initializer_list<ComponentTypeID> components, GOSpawnType spawnType)
	{
		return SpawnGameObject(components, { WINDOWSSYSTEM->GetWidth() * 0.5f , WINDOWSSYSTEM->GetHeight() * 0.5f }, spawnType);
	}

	/*!***********************************************************************
	 \brief
	 Spawns a GameObject with specified components and position
	*************************************************************************/
	GameObject* FactorySystem::SpawnGameObject(std::initializer_list<ComponentTypeID> components, Vec2<float> pos, GOSpawnType spawnType)
	{
		// Creating the game object
		GameObject* gameObj = FACTORY->CreateGameObject();
		if (gameObj->GetID() == 0) 		// ID only starts at 1
		{
			gameObj->SetID(FACTORY->GetLastObjID());
		}

		// Every gameobject must have a transform!!!
		gameObj->AddComponent(C_CPTransform, FACTORY->GetCMs()[C_CPTransform]->Create());
		gameObj->GetComponent<CPTransform>()->SetPosition(pos);

		if (spawnType == GOSpawnType::SCENE_BOUNDED)
		{
			// Add GO to current loaded scene
			if (m_loadedScene)
			{
				gameObj->GetComponent<CPTransform>()->SetParent(m_loadedScene->GetRoot());
				m_loadedScene->GetRoot()->AddChild(gameObj->GetComponent<CPTransform>());
			}
			else
			{
				DebuggerLogWarning("Tried spawning Game Object onto an unloaded Scene!");
			}
			// Sends game object to the Game Object Manager
			// For now, push back in the function to go manager,
			// However, in the future we might need to change when we need to render a lot in one go
			// or send a equal amount of game objects at one go
			// Might be redundant too because we should initialize a pool at the start
			GOMANAGER->AddGameObject(gameObj);
		}
		else if (spawnType == GOSpawnType::FREE_FLOATING)
		{
			GOMANAGER->AddDDOLGameObject(gameObj);
		}

		for (ComponentTypeID component : components)
		{
			gameObj->AddComponent(component, FACTORY->GetCMs()[component]->Create());
		}
		gameObj->StartComponents();

		return gameObj;
	}

	/*!***********************************************************************
	 \brief
	 Spawns a GameObject with the exact same components as the provided GameObject
	*************************************************************************/
	GameObject* FactorySystem::SpawnGameObject(GameObject* prefab, GOSpawnType spawnType)
	{
		GameObject* clone = FACTORY->CreateGameObject();
		if (clone->GetID() == 0) 		// ID only starts at 1
		{
			clone->SetID(FACTORY->GetLastObjID());
		}
		//We loop through all the components in the prefab
		for (auto& elem : prefab->GetAllComponents())
		{	//Then we add it to our clone
			clone->AddComponent(elem.first,FACTORY->GetCMs()[elem.first]->Create());
		}

		if (spawnType == GOSpawnType::SCENE_BOUNDED) 
		{
			// Add GO to current loaded scene
			if (m_loadedScene)
			{
				clone->GetComponent<CPTransform>()->SetParent(m_loadedScene->GetRoot());
				m_loadedScene->GetRoot()->AddChild(clone->GetComponent<CPTransform>());
			}
			else
			{
				DebuggerLogWarning("Tried spawning Game Object onto an unloaded Scene!");
			}
			GOMANAGER->AddGameObject(clone);
		}
		else if (spawnType == GOSpawnType::FREE_FLOATING)
		{
			GOMANAGER->AddDDOLGameObject(clone);
		}

		//This copies the data from our prefab components over to the clone
		clone->SetComponents(prefab->GetAllComponents());
		//Then we initialise the data for the clone
		clone->StartComponents();
		return clone;
	}

	/*!***********************************************************************
	 \brief
	 Creates a empty GameObject with no components
	*************************************************************************/
	GameObject* FactorySystem::CreateGameObject()
	{
		++m_LastObjID;

		DebuggerLog("GO" + std::to_string(m_LastObjID) + " has been created");

		// Original return
		return DBG_NEW GameObject(FACTORY->GetLastObjID());
	}

	/*!***********************************************************************
	 \brief
	 Gets all of the ComponentMakers
	*************************************************************************/
	std::map<ComponentTypeID, ComponentMaker*>& FactorySystem::GetCMs()
	{
		return m_ComponentMakers;
	}

	/*!***********************************************************************
	 \brief
	 Gets the last created GameObject's ID.
	*************************************************************************/
	int FactorySystem::GetLastObjID() const
	{
		return m_LastObjID;
	}

	/*!***********************************************************************
	 \brief
	 Updates the current scene loaded to add the GameObjects to
	*************************************************************************/
	void FactorySystem::UpdateLoadedScene(Scene* loadedScene)
	{
		m_loadedScene = loadedScene;
	}

	/*!***********************************************************************
	 \brief
	 Same function but for Event Subscription
	*************************************************************************/
	void UpdateLoadedScene(Scene* loadedScene)
	{
		FACTORY->UpdateLoadedScene(loadedScene);
	}
}