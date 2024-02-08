/*!************************************************************************
 \file				SpriteSheet.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief

 This header file contains the implmentation of the the index min and max of
 the teture of the sprite, serialising and deserialising the spritesheet and 
 getting it. Definition.

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "SpriteSheet.h"

namespace LB
{
	// SPRITE
	
	/*!***********************************************************************
	  \brief
	  Serialsing the index, min and max value or she sprite
	*************************************************************************/
	bool Sprite::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		Value indexValue;
		data.AddMember("Index", m_index, alloc);
		Value minValue;
		if (m_min.Serialize(minValue, alloc))
		{
			data.AddMember("Min", minValue, alloc);
		}
		Value maxValue;
		if (m_max.Serialize(maxValue, alloc))
		{
			data.AddMember("Max", maxValue, alloc);
		}

		return true; 
	}

	/*!***********************************************************************
	  \brief
	  Deserialsing the index min and max, if it exist
	*************************************************************************/
	bool Sprite::Deserialize(const Value& data)
	{
		bool HasIndex = data.HasMember("Index");
		bool HasMin = data.HasMember("Min");
		bool HasMax = data.HasMember("Max");
		
		if (data.IsObject())
		{
			if (HasIndex && HasMin && HasMax)
			{
				const Value& indexValue = data["Index"];
				const Value& minValue = data["Min"];
				const Value& maxValue = data["Max"];

				m_index = indexValue.GetInt();
				m_min.Deserialize(minValue);
				m_max.Deserialize(maxValue);

				return true;
			}
		}
		return false;
	}

	// SPRITESHEET

	/*!***********************************************************************
	  \brief
	  copy constructor name and pngname
	*************************************************************************/
	SpriteSheet::SpriteSheet(std::string const& name, std::string const& PNGName) : m_name{name}, m_pngName{PNGName} {}

	/*!***********************************************************************
	  \brief
	  Serialsing the data of the the spritesheet
	*************************************************************************/
	bool SpriteSheet::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		Value nameValue(m_name.c_str(), alloc);
		data.AddMember("Name", nameValue, alloc);

		Value pngValue(m_pngName.c_str(), alloc);
		data.AddMember("PNG", pngValue, alloc);

		// TO BE REFACTORED
		data.AddMember("Rows", m_row, alloc);
		data.AddMember("Cols", m_col, alloc);

		Value spriteArray(rapidjson::kArrayType);
		for (auto& sprite : m_sprites)
		{
			Value spriteValue;
			if (sprite.Serialize(spriteValue, alloc))
			{
				spriteArray.PushBack(spriteValue, alloc);
			}
		}
		data.AddMember("Sprites", spriteArray, alloc);

		return true;
	}

	/*!***********************************************************************
	  \brief
	  Deserialsing the sprites
	*************************************************************************/
	bool SpriteSheet::Deserialize(const Value& data)
	{
		// Make sure sprites vector is clear
		m_sprites.clear();

		bool HasName = data.HasMember("Name");
		bool HasPNG = data.HasMember("PNG");
		bool HasSprites = data.HasMember("Sprites");

		// TO BE REFACTORED
		bool HasRows = data.HasMember("Rows");
		bool HasCols = data.HasMember("Cols");

		if (data.IsObject())
		{
			if (HasName)
			{
				const Value& nameValue = data["Name"];
				m_name = nameValue.GetString();
			}
			if (HasPNG)
			{
				const Value& pngValue = data["PNG"];
				m_pngName = pngValue.GetString();
			}
			if (HasSprites)
			{
				Sprite newSprite{};
				const Value& spritesValue = data["Sprites"].GetArray();
				for (rapidjson::SizeType i{}; i < spritesValue.Size(); ++i)
				{
					newSprite.Deserialize(spritesValue[i]);
					m_sprites.push_back(newSprite);
				}
			}
			// TO BE REFACTORED
			if (HasRows)
			{
				const Value& rowValue = data["Rows"];
				m_row = rowValue.GetFloat();
			}
			if (HasCols)
			{
				const Value& colValue = data["Cols"];
				m_col = colValue.GetFloat();
			}
			return true;
		}

		return false; 
	}

	/*!***********************************************************************
	  \brief
	  Storing the sprites into the vector after it was sliced into sprites
	*************************************************************************/
	void SpriteSheet::Slice(Vec2<float> min, Vec2<float> max)
	{
		m_sprites.emplace_back(Size(), min, max);
	}

	/*!***********************************************************************
	  \brief
	  At is getting the sprite that you want in the vector
	*************************************************************************/
	Sprite& SpriteSheet::At(int index)
	{
		return m_sprites[index];
	}

	/*!***********************************************************************
	  \brief
	  Operator [] so can access the elements in the vector
	*************************************************************************/
	Sprite& SpriteSheet::operator[](int index)
	{
		return m_sprites[index];
	}

	/*!***********************************************************************
	  \brief
	  Operator [] so can access the elements in the vector
	*************************************************************************/
	Sprite const& SpriteSheet::operator[](int index) const
	{
		return m_sprites[index];
	}

	/*!***********************************************************************
	  \brief
	  A vector that stores the sprites
	*************************************************************************/
	std::vector<Sprite>& SpriteSheet::Sprites()
	{
		return m_sprites;
	}

	/*!***********************************************************************
	  \brief
	  Getting the PNG reference, returning a string
	*************************************************************************/
	std::string const& SpriteSheet::GetPNGRef() const
	{
		return m_pngName;
	}

	/*!***********************************************************************
	  \brief
	  Setting the name of the png
	*************************************************************************/
	void SpriteSheet::SetPNGRef(std::string const& newPNG)
	{
		m_pngName = newPNG;
	}

	/*!***********************************************************************
	  \brief
	  Getting the name of the spritesheet
	*************************************************************************/
	std::string const& SpriteSheet::GetName() const
	{
		return m_name;
	}

	/*!***********************************************************************
	  \brief
	  Setting the name of the spritesheet
	*************************************************************************/
	void SpriteSheet::SetName(std::string const& newName)
	{
		m_name = newName;
	}

	/*!***********************************************************************
	  \brief
	  Getting the size of the vector
	*************************************************************************/
	size_t SpriteSheet::Size()
	{
		return m_sprites.size();
	}
}