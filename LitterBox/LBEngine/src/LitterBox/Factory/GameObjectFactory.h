/*!************************************************************************
 \file				GameObjectFactory.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				
 This file contains functions declarations of the FactorySystem class that 
 creates a GameObject with different components.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "pch.h"
#include "Components.h"
#include "Platform/Windows/Windows.h"
#include "GameObjectManager.h"
#include "LitterBox/Debugging/Memory.h"
#include "LitterBox/Core/System.h"

namespace LB
{
	class Scene;

	enum class GOSpawnType
	{
		SCENE_BOUNDED,	// GO is destroyed when scene is destroyed
		FREE_FLOATING	// GO is not tied to any scene
	};

	/*!***********************************************************************
	 \brief
	 FactorySystem class allows the creation of GameObjects with components
	 such as CPTransform, CPRender and more, allowing for modularity.
	*************************************************************************/
	class FactorySystem : public ISystem
	{
	public:
		/*!***********************************************************************
		 \brief
		 Initialises the factory with known component types to make ComponentMakers
		 of those types
		*************************************************************************/
		FactorySystem();

		/*!***********************************************************************
		 \brief
		 Initialise the factory and sets the system's name
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		 \brief
		 Updates the factory
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		 \brief
		 Destroys the factory
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		 \brief
		 Initialise this ComponentMaker of their type, eg. Physics / Graphics
		*************************************************************************/
		void InitCM(const ComponentTypeID& name, ComponentMaker* newComponent);

		/*!***********************************************************************
		 \brief
		 Delete all ComponentMakers from all GameObjects 
		*************************************************************************/
		void DeleteAllCMs();

		/*!***********************************************************************
		 \brief
		 Spawns a GameObject with no components but specified position
		*************************************************************************/
		GameObject* SpawnGameObject(Vec2<float> pos = { WINDOWSSYSTEM->GetWidth() * 0.5f , WINDOWSSYSTEM->GetHeight() * 0.5f }, GOSpawnType spawnType = GOSpawnType::SCENE_BOUNDED);

		/*!***********************************************************************
		 \brief
		 Spawns a GameObject with components but no specified position, and specified
		 spawn type
		*************************************************************************/
		GameObject* SpawnGameObject(std::initializer_list<ComponentTypeID> components, GOSpawnType spawnType);

		/*!***********************************************************************
		 \brief
		 Spawns a GameObject with specified components and position
		*************************************************************************/
		GameObject* SpawnGameObject(std::initializer_list<ComponentTypeID> components,
									Vec2<float> pos = { WINDOWSSYSTEM->GetWidth() * 0.5f , WINDOWSSYSTEM->GetHeight() * 0.5f }, GOSpawnType spawnType = GOSpawnType::SCENE_BOUNDED);

		/*!***********************************************************************
		 \brief
		 Spawns a GameObject with the exact same components as the provided GameObject
		*************************************************************************/
		GameObject* SpawnGameObject(GameObject* prefab, GOSpawnType spawnType = GOSpawnType::SCENE_BOUNDED);

		/*!***********************************************************************
		 \brief
		 Gets all of the ComponentMakers
		*************************************************************************/
		std::map<ComponentTypeID, ComponentMaker*>& GetCMs();

		/*!***********************************************************************
		 \brief
		 Gets the last created GameObject's ID.
		*************************************************************************/
		int GetLastObjID() const;

		void UpdateLoadedScene(Scene* loadedScene);

	private:
		/*!***********************************************************************
		 \brief
		 SpawnGameObject uses this to create the empty GameObject shell
		*************************************************************************/
		GameObject* CreateGameObject();

		Scene* m_loadedScene{ nullptr };

		std::map<ComponentTypeID, ComponentMaker*>	m_ComponentMakers;
		std::vector<GameObject*>					m_WaitingList;

		int											m_LastObjID{};
		bool										m_ToUpdate{};
	};

	void UpdateLoadedScene(Scene* loadedScene);

	extern FactorySystem* FACTORY;
}

