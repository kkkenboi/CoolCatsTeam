/*!************************************************************************
 \file				SpriteSheet.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief

 This header file

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
		Sprite() : m_index{ 0 }, m_min{}, m_max{} {}
		Sprite(int index, Vec2<float> min, Vec2<float> max) : m_index{ index }, m_min{ min }, m_max{ max } { }
		Sprite(int index, Vec2<float> min, float max_x, float max_y) : m_index{ index }, m_min{ min }, m_max{ Vec2<float>{max_x, max_y} } { }

		bool Serialize(Value& data, Document::AllocatorType& alloc); //to save 
		bool Deserialize(const Value& data); //to load

		int m_index;
		Vec2<float> m_min, m_max;
	};

	class SpriteSheet
	{
	public:
		SpriteSheet() : m_name{ "Unnamed Sheet" }, m_pngName{ "No PNG" }, m_row{ 0 }, m_col{ 0 } {}
		SpriteSheet(std::string const& name, std::string const& PNGName);

		bool Serialize(Value& data, Document::AllocatorType& alloc); //to save 
		bool Deserialize(const Value& data); //to load

		void Slice(Vec2<float> min, Vec2<float> max);

		Sprite& At(int index);
		Sprite& operator[](int index);
		Sprite const& operator[](int index) const;
		std::vector<Sprite>& Sprites();

		std::string const& GetPNGRef() const;
		void SetPNGRef(std::string const& newPNG);

		std::string const& GetName() const;
		void SetName(std::string const& newName);

		size_t Size();

		// TO BE REFACTORED
		int m_row, m_col;

	private:
		std::string m_name, m_pngName;
		std::vector<Sprite> m_sprites; //store width and height of each sprite
	};
}