/*!************************************************************************
 * \file				RenderComponent.cpp
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2400A
 * \date				07/02/2024
 * \brief 				Source file containing the implementation for
 * 						Serialising and Deserialising RenderObjs
 * 						This is here to fix circular dependency issues
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Serialization/AssetManager.h"

namespace LB
{
	/*!************************************************************************
	 * \brief Serializes the Render Component
	 * 
	**************************************************************************/
	bool CPRender::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		data.AddMember("Active", m_active, alloc);
		data.AddMember("Width", w, alloc);
		data.AddMember("Height", h, alloc);
		data.AddMember("ZLayer", z_val, alloc);
		data.AddMember("Layer", (int)renderer_id, alloc);
		Value textureName(ASSETMANAGER->GetTextureName(texture).c_str(),alloc);
		data.AddMember("Texture", textureName, alloc);
		Value SpriteSheetNameValue(spriteSheetName.c_str(),alloc);
		data.AddMember("SpriteName", SpriteSheetNameValue, alloc);
		data.AddMember("SpriteIndex", spriteIndex, alloc);
		return true;
	}

	/*!************************************************************************
	 * \brief Deserializes the Render Component
	 * 
	**************************************************************************/
	bool CPRender::Deserialize(const Value& data)
	{
		bool HasActive = data.HasMember("Active");
		bool HasTexture = data.HasMember("Texture");
		bool HasWidth = data.HasMember("Width");
		bool HasHeight = data.HasMember("Height");
		bool HasZLayer = data.HasMember("ZLayer");
		bool HasLayer = data.HasMember("Layer");
		bool HasSpriteSheet = data.HasMember("SpriteName");
		bool HasSpriteIndex = data.HasMember("SpriteIndex");
		if (data.IsObject())
		{
			if (HasActive) m_active = data["Active"].GetBool();
			if (HasWidth) w = data["Width"].GetFloat();
			if (HasHeight) h = data["Height"].GetFloat();
			if (HasZLayer) z_val = data["ZLayer"].GetFloat();
			if (HasLayer)
			{
				Renderer::Renderer_Types type = (Renderer::Renderer_Types)data["Layer"].GetInt();
				if (type != renderer_id)
				{
					Renderer::GRAPHICS->swap_object_type(type, this);
				}
			}
			if (HasSpriteIndex)
			{
				spriteIndex = data["SpriteIndex"].GetInt();
			}
			if (HasTexture)
			{
				//Because we have 2 versions, for now we keep this
				//Until all versions have been updated then we can remove this
				const Value& textureValue = data["Texture"];
				if (textureValue.IsInt())
				{
					std::string textureName = ASSETMANAGER->GetTextureName(textureValue.GetInt());
					UpdateTexture(textureValue.GetInt(), static_cast<int>(w), static_cast<int>(h));
					//texture = textureValue.GetInt();
					return true;
				}
				std::string textureName = textureValue.GetString();

				UpdateTexture(ASSETMANAGER->GetTextureUnit(textureName), static_cast<int>(w), static_cast<int>(h));

				//texture = ASSETMANAGER->GetTextureUnit(textureValue.GetString());
				/*return true;*/
			}
			if (HasSpriteSheet)
			{
				spriteSheetName = data["SpriteName"].GetString();
				if (spriteSheetName != "None")
				{
					SetSpriteTexture(spriteSheetName, spriteIndex);
				}
			}
			return true;
		}
		return false;
	}


	/*!***********************************************************************
	 \brief
	 Sets the texture for the render component using a sprite sheet
	 e.g "Upgrades" -> the stemmed name of Upgrades.spritesheet (w/o extension name)
	 The index refers to the index of the sprite. 
	*************************************************************************/
	void CPRender::SetSpriteTexture(std::string SpriteSheetName, int index)
	{
		//Cache the sprite sheet to make it easier on us
		SpriteSheet selectedsheet = ASSETMANAGER->GetSpriteSheet(SpriteSheetName);
		//Update the current render texture with all the data from the sprite sheet
		UpdateTexture(
			ASSETMANAGER->GetTextureUnit(selectedsheet.GetPNGRef()), static_cast<int>(w), static_cast<int>(h),
			selectedsheet.Sprites()[index].m_min, selectedsheet.Sprites()[index].m_max);

		/*UpdateTexture(ASSETMANAGER->GetTextureUnit(selectedsheet.GetPNGRef()),
			ASSETMANAGER->Textures[ASSETMANAGER->assetMap[selectedsheet.GetPNGRef()]].first->width,
			ASSETMANAGER->Textures[ASSETMANAGER->assetMap[selectedsheet.GetPNGRef()]].first->height,
			selectedsheet.Sprites()[index].m_min, selectedsheet.Sprites()[index].m_max);*/
		spriteSheetName = SpriteSheetName;
		spriteIndex = index;
	}

	/*!***********************************************************************
	 \brief
	 Toggles the ACTUAL active state for this component
	*************************************************************************/
	void CPRender::ToggleActive(bool isActive)
	{
		if (isActive && !activated) set_active();
		else if (!isActive && activated) set_active();
	}
}