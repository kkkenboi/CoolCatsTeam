/*!************************************************************************
 * \file				RenderComponent.cpp
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2400A
 * \date				19/11/2023
 * \brief 				Source file containing the implementation for
 * 						Serialising and Deserialising RenderObjs
 * 						This is here to fix circular dependency issues
 *  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	/*!************************************************************************
	 * \brief 
	 * 
	 * \param data 
	 * \param alloc 
	 * \return true 
	 * \return false 
	**************************************************************************/
	bool CPRender::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		Value textureName(ASSETMANAGER->GetTextureName(texture).c_str(),alloc);
		data.AddMember("Width", w, alloc);
		data.AddMember("Height", h, alloc);
		data.AddMember("Texture", textureName, alloc);
		return true;
	}
	bool CPRender::Deserialize(const Value& data)
	{
		bool HasTexture = data.HasMember("Texture");
		bool HasWidth = data.HasMember("Width");
		bool HasHeight = data.HasMember("Height");
		if (data.IsObject())
		{
			if (HasWidth) w = data["Width"].GetFloat();
			if (HasHeight) h = data["Height"].GetFloat();
			if (HasTexture)
			{
				const Value& textureValue = data["Texture"];
				if (textureValue.IsInt())
				{
					std::string textureName = ASSETMANAGER->GetTextureName(textureValue.GetInt());
					UpdateTexture(textureValue.GetInt(), ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width, ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height);
					//texture = textureValue.GetInt();
					return true;
				}
				std::string textureName = textureValue.GetString();

				UpdateTexture(ASSETMANAGER->GetTextureUnit(textureName), ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->width, ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].first->height);

				//texture = ASSETMANAGER->GetTextureUnit(textureValue.GetString());
				return true;
			}
		}
		return false;
	}
}