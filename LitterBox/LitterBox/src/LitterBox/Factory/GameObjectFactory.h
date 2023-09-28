/*!************************************************************************
 \file
 \author(s)
 \par DP email(s):
 \par Course:		CSD2401A
 \date
 \brief

**************************************************************************/

#pragma once
#include "pch.h"
#include "Components.h"
#include "GameObjectManager.h"
#include "LitterBox/Debugging/Memory.h"
#include "LitterBox/Core/System.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 This class creates ComponentMakers to create Components

	*************************************************************************/
	class FactorySystem : public ISystem
	{
	public:
		/*!***********************************************************************
		 \brief
		 Let the factory initialize the known component types listed in the file
		 and/or start creating game objects from a data file
		*************************************************************************/
		FactorySystem();

		/*!***********************************************************************
		 \brief
		 Based on event/messaging system, how many game objects to be created/destroyed
		 will be updated here
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		 \brief
		 Communicate with other systems
		*************************************************************************/
		void SendMessage(Message* message) override;

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void Initialize() override { SetSystemName("GameObject Factory System"); }

		/*!***********************************************************************
		 \brief
		 Serialise all known ComponentMakers
		*************************************************************************/
		void SerialiseGameObjs(int json_thing);

		/*!***********************************************************************
		 \brief
		 Create all known ComponentMaker of their type, eg. Physics / Graphics
		*************************************************************************/
		void InitCM(const std::string& name, ComponentMaker* newComponent);

		/*!***********************************************************************
		 \brief
		 Since the 
		*************************************************************************/
		void DeleteAllCMs(std::map<std::string, ComponentMaker*> ComponentMakers);

		/*!***********************************************************************
		 \brief
		 Use these functions to spawn game objects for now, TODO: refactor
		*************************************************************************/
		GameObject* SpawnGameObject(Vec2<float> pos = { WINDOWSSYSTEM->GetWidth() * 0.5f , WINDOWSSYSTEM->GetHeight() * 0.5f });

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		GameObject* SpawnGameObject(std::initializer_list<std::string> components, Vec2<float> pos = { WINDOWSSYSTEM->GetWidth() * 0.5f , WINDOWSSYSTEM->GetHeight() * 0.5f });

		/*!***********************************************************************
		 \brief
		 Creates a game object with a component list, if component list is empty, no components
		 will be tied to the game object
		*************************************************************************/
		GameObject* CreateGameObject();

		GameObject* SpawnGameObject(GameObject* prefab);

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		std::map<std::string, ComponentMaker*> GetCMs() const;

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		int GetLastObjID() const;

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		void Destroy() override;

	private:

		/*!***********************************************************************
		 \brief
		 Needs to hold all of the componentmakers to know what available componentmakers are there
		 Does not need to have the game objects because that is the gameobjmanager job to hold
		 Strictly only to create game objects and let go from there on

		 For now we use map to hold the componentmakers, might change in the future

		*************************************************************************/
		int m_LastObjID;

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		std::map<std::string, ComponentMaker*> m_ComponentMakers;

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		std::vector<GameObject*> m_WaitingList;

		/*!***********************************************************************
		 \brief

		*************************************************************************/
		bool toUpdate = false;

	};

	/*!***********************************************************************
	 \brief

	*************************************************************************/
	extern FactorySystem* FACTORY;
}

