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
	SpriteSheet::SpriteSheet(std::string const& name, std::string const& PNGName) : m_name{name}, m_pngName{PNGName} { }

	bool SpriteSheet::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		return true;
	}

	bool SpriteSheet::Deserialize(const Value& data)
	{
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