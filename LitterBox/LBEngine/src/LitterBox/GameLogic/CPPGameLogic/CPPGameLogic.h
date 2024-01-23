/*!************************************************************************
 \file				CPPGameLogic.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				29/10/2023
 \brief
 This file handles the initialization of all CPP scripts loaded by
 GameObjects. Scripts only function when the game is playing

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Core/System.h" // For ISystem
#include "LitterBox/Components/CPPScriptComponent.h"

#include <typeindex> // For TypeIndex

namespace LB
{

	#define REGISTER_SCRIPT(scriptClass) \
		struct scriptClass##Registrar { \
			scriptClass##Registrar() { \
				CPPGameLogic::Instance()->RegisterScript<scriptClass>(); \
			} \
		}; static scriptClass##Registrar scriptClass##Registration; \

	class ScriptFactoryBase {
	public:
		virtual CPPBehaviour* CreateInstance() = 0;

		virtual ~ScriptFactoryBase() = default;
	};

	template <typename T>
	class ScriptFactory : public ScriptFactoryBase
	{
	public:
		CPPBehaviour* CreateInstance() override
		{
			return DBG_NEW T();
		}
	};

	class ScriptTypeID
	{
	public:
		ScriptTypeID() : m_type{ typeid(void) } {}
		ScriptTypeID(std::type_index type) : m_type{ type } {}
		std::type_index m_type { typeid(void) };
	};

	class CPScriptCPP;

	/*!***********************************************************************
	 \brief
	 GameLogic class will contain functions that checks whether GameObjects
	 should be active or nonactive after certain interactions and updates their
	 ingame stats as well
	*************************************************************************/
	class CPPGameLogic : public ISystem, public Singleton<CPPGameLogic>
	{
	public:
		/*!***********************************************************************
		 \brief
		 Initalises the Game Logic system
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		 \brief
		 Start for CPPGamelogic manager. Creates and loads the instances of each 
		 script when the scene starts
		*************************************************************************/
		void Start();

		void RegisterAll();

		/*!***********************************************************************
		 \brief
		 Adds a script to the registry of CPPGameLogic manager so that it knows
		 that the script exists.
		*************************************************************************/
		template <typename T>
		void RegisterScript()
		{
			m_scriptRegistry[typeid(T)] = DBG_NEW ScriptFactory<T>();
			m_scriptTypeRegistry[typeid(T).name()] = ScriptTypeID{ typeid(T) };
		}

		/*!***********************************************************************
		 \brief
		 Returns the registry containing all the script types stored by the
		 GameLogic manager.
		*************************************************************************/
		std::map<std::type_index, ScriptFactoryBase*>& GetRegistry();


		/*!***********************************************************************
		 \brief
		 Returns the registry containing all the script types stored by the
		 GameLogic manager.
		*************************************************************************/
		std::map<std::string, ScriptTypeID>& GetTypeRegistry();

		/*!***********************************************************************
		 \brief
		 Loads the CPPScript instance based on the name of the script.
		*************************************************************************/
		void StartScript(CPScriptCPP* script);

		/*!***********************************************************************
		 \brief
		 Adds a new script to the list of scripts to load when scene plays
		*************************************************************************/
		void Load(CPScriptCPP* newScript);

		/*!***********************************************************************
		 \brief
		 Removes the given script from the list of scripts
		*************************************************************************/
		void Unload(CPScriptCPP* scriptToRemove);

		/*!***********************************************************************
		 \brief
		 Updates the script components for all GameObjects
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		 \brief
		 Destroys the GameLogic system and clears all scripts attached
		*************************************************************************/
		void Destroy() override;

	private:
		std::map<std::type_index, ScriptFactoryBase*> m_scriptRegistry;
		std::map<std::string, ScriptTypeID> m_scriptTypeRegistry;

		std::vector<CPScriptCPP*> m_sceneScripts{};	// List of all scripts currently active in the scene
	};

	/*!***********************************************************************
	 \brief
	 For event subscription for when the scene starts
	*************************************************************************/
	void StartScripts(bool isPlaying);

	/*!***********************************************************************
	 \brief
	 A global pointer to our game so that it can be accessed from anywhere.
	*************************************************************************/
	extern CPPGameLogic* CPPGAMELOGIC;
}
