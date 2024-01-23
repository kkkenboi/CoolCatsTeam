/*!************************************************************************
 \file				SpriteSheet.cpp
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "SpriteSheet.h"

namespace LB
{
	// SPRITE

	bool Sprite::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		Value indexValue;
		data.AddMember("Index", m_index, alloc);
		Value positionValue;
		if (m_pos.Serialize(positionValue, alloc)) 
		{
			data.AddMember("Position", positionValue, alloc);
		}
		Value sizeValue;
		if (m_size.Serialize(sizeValue, alloc))
		{
			data.AddMember("Size", sizeValue, alloc);
		}

		return true; 
	}

	bool Sprite::Deserialize(const Value& data)
	{
		bool HasIndex = data.HasMember("Index");
		bool HasPosition = data.HasMember("Position");
		bool HasSize = data.HasMember("Size");
		
		if (data.IsObject())
		{
			if (HasIndex && HasPosition && HasSize)
			{
				const Value& indexValue = data["Index"];
				const Value& positionValue = data["Position"];
				const Value& sizeValue = data["Size"];

				m_index = indexValue.GetInt();
				m_pos.Deserialize(positionValue);
				m_size.Deserialize(sizeValue);

				return true;
			}
		}
		return false;
	}

	// SPRITESHEET

	SpriteSheet::SpriteSheet(std::string const& name, std::string const& PNGName) : m_name{name}, m_pngName{PNGName} { }

	bool SpriteSheet::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		Value nameValue(m_name.c_str(), alloc);
		data.AddMember("Name", nameValue, alloc);

		Value pngValue(m_pngName.c_str(), alloc);
		data.AddMember("PNG", nameValue, alloc);

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

	bool SpriteSheet::Deserialize(const Value& data)
	{
		// Make sure sprites vector is clear
		m_sprites.clear();

		bool HasName = data.HasMember("Name");
		bool HasPNG = data.HasMember("PNG");
		bool HasSprites = data.HasMember("Sprites");

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
				m_name = pngValue.GetString();
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
			return true;
		}

		return false; 
	}

	void SpriteSheet::Slice(Vec2<int> pos, int width, int height)
	{
		m_sprites.emplace_back(Size(), pos, width, height);
	}

	Sprite& SpriteSheet::At(int index)
	{
		return m_sprites[index];
	}

	Sprite& SpriteSheet::operator[](int index)
	{
		return m_sprites[index];
	}

	Sprite const& SpriteSheet::operator[](int index) const
	{
		return m_sprites[index];
	}

	std::vector<Sprite> const& SpriteSheet::Sprites() const
	{
		return m_sprites;
	}

	std::string const& SpriteSheet::GetPNGRef() const
	{
		return m_pngName;
	}

	void SpriteSheet::SetPNGRef(std::string const& newPNG)
	{
		m_pngName = newPNG;
	}

	std::string const& SpriteSheet::GetName() const
	{
		return m_name;
	}

	void SpriteSheet::SetName(std::string const& newName)
	{
		m_name = newName;
	}

	size_t SpriteSheet::Size()
	{
		return m_sprites.size();
	}
}