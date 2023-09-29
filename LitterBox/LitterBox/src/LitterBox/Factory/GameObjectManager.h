/*!************************************************************************
 \file				GameObjectFactory.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				This file contains functions declarations of the
					FactorySystem class that creates a GameObject with
					different components.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "pch.h"
#include "LitterBox/Core/System.h"
#include <initializer_list>

namespace LB
{
	class IComponent; // Forward Declaration

	/*!***********************************************************************
	 \brief
	 GameObject class contains components that contains different functionalities
	 and will be updated by their own systems.
	*************************************************************************/
	class GameObject
	{
	public:
		/*!***********************************************************************
		 \brief
		 Creates a GameObject
		*************************************************************************/
		GameObject();

		/*!***********************************************************************
		 \brief
		 Creates a GameObject with an ID
		*************************************************************************/
		GameObject(int ID);

		/*!***********************************************************************
		 \brief
		 Destroys a GameObject
		*************************************************************************/
		~GameObject();

		/*!***********************************************************************
		 \brief
		 Gets a specified component within the GameObject
		*************************************************************************/
		template <typename T>
		T* GetComponent(std::string name)
		{
			return static_cast<T*>(m_Components[name]);
		}

		/*!***********************************************************************
		 \brief
		 Gets all the components of the GameObject
		*************************************************************************/
		std::unordered_map<std::string, IComponent*> GetComponents();

		/*!***********************************************************************
		 \brief
		 Adds a component to the GameObject
		*************************************************************************/
		void AddComponent(std::string name, IComponent* component);

		/*!***********************************************************************
		 \brief
		 Initialises all of the components in the GameObject
		*************************************************************************/
		void StartComponents();

		/*!***********************************************************************
		 \brief
		 Gets the ID of the GameObject
		 *************************************************************************/
		int GetID() const;

		/*!***********************************************************************
		 \brief
		 Sets the ID of the GameObject
		*************************************************************************/
		void SetID(int ID);

	private:
		std::unordered_map<std::string, IComponent*>	m_Components;
		int												m_ID;
		bool											m_IsActive;
	};

	/*!***********************************************************************
	 \brief
	 GameObjectManager class is a system that manages all of the game objects.
	*************************************************************************/
	class GameObjectManager : public ISystem
	{
	public:
		/*!***********************************************************************
		 \brief
		 Creates the GameObjectManager4
		*************************************************************************/
		GameObjectManager();

		/*!***********************************************************************
		 \brief
		 Destroys the GameObjectManager
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		std::vector<GameObject*> GetGameObjects() const;

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void AddGameObject(GameObject* gameObject);

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void DestroyAllGOs();

	private:
		/*!***********************************************************************
		 \brief

		*************************************************************************/
		std::vector<GameObject*> m_GameObjects;
	};

	/*!***********************************************************************
	 \brief

	*************************************************************************/
	extern GameObjectManager* GOMANAGER;
}

