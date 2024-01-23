/*!************************************************************************
 \file				SpriteSheet.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief


  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include <string>
#include "LitterBox/Serialization/Serializer.h"

namespace LB
{
	struct Sprite
	{
		Sprite(int index, Vec2<int> pos, int width, int height) : m_index{ index }, m_pos{ pos }, m_width{ width }, m_height{ height } { }

		bool Serialize(Value& data, Document::AllocatorType& alloc); //to save 
		bool Deserialize(const Value& data); //to load

		int m_index;
		Vec2<int> m_pos;
		int m_width, m_height;
	};

	class SpriteSheet
	{
	public:
		SpriteSheet() : m_name{ "Unnamed Sheet" }, m_pngName{ "No PNG" } {}
		SpriteSheet(std::string const& name, std::string const& PNGName);

		bool Serialize(Value& data, Document::AllocatorType& alloc); //to save 
		bool Deserialize(const Value& data); //to load

		void Slice(Vec2<int> pos, int width, int height);

		Sprite& At(int index);
		Sprite& operator[](int index);
		Sprite const& operator[](int index) const;
		std::vector<Sprite> const& Sprites() const;

		std::string const& GetPNGRef() const;
		void SetPNGRef(std::string const& newPNG);

		std::string const& GetName() const;
		void SetName(std::string const& newName);

		size_t Size();

	private:
		std::string m_name, m_pngName;
		std::vector<Sprite> m_sprites; //store width and height of each sprite
	};
}