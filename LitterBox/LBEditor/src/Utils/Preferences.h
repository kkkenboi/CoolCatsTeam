/*!************************************************************************
 \file				Preferences.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				22/11/2023
 \brief



 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <string>
#include "LitterBox/Core/Singleton.h"
#include "LitterBox/Scene/SceneManager.h"
#include "LitterBox/Serialization/Serializer.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	class Preferences : public Singleton<Preferences>
	{
	public:
		void LoadPreferences()
		{
			// Create the preferences file if it doesn't exist
			std::filesystem::path prefLocation = (ASSETMANAGER->_appData / ASSETMANAGER->folderName / std::filesystem::path("Preferences.json")).string();
			if (!std::filesystem::exists(prefLocation)) 
			{
				JSONSerializer::SerializeToFile(prefLocation.string(), *this);;
			}

			JSONSerializer::DeserializeFromFile(prefLocation.string(), *this);
		}

		void SavePreferences()
		{
			JSONSerializer::SerializeToFile((ASSETMANAGER->_appData / ASSETMANAGER->folderName / std::filesystem::path("Preferences.json")).string(), *this);
		}

		bool Serialize(Value& data, Document::AllocatorType& alloc)
		{
			data.SetObject();

			data.AddMember("Fullscreen", m_fullscreen, alloc);

			Value sceneValue(m_inspectedScene.c_str(), alloc);
			data.AddMember("Inspected Scene", sceneValue, alloc);

			return true;
		}

		bool Deserialize(const Value& data)
		{
			bool HasFullscreen = data.HasMember("Fullscreen");
			bool HasScene = data.HasMember("Inspected Scene");

			if (data.IsObject())
			{
				if (HasFullscreen)
				{
					const Value& fullScreenValue = data["Fullscreen"];
					m_fullscreen = fullScreenValue.GetBool();
				}
				if (HasScene)
				{
					const Value& sceneValue = data["Inspected Scene"];
					m_inspectedScene = sceneValue.GetString();
				}
				return true;
			}

			return false;
		}

		bool m_fullscreen{ false };
		std::string m_inspectedScene{"None"};
		std::string m_editoriniFile{"Default.ini"}, m_editoriniFilePath{};
	};
}
