/*!************************************************************************
 \file				SpriteSheet.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin@digipen.edu
 \par Course:		CSD2401A
 \date				7-1-2024
 \brief

 This header file contains the index min and max of the teture of the sprite, 
 serialising and deserialising the spritesheet and getting it.
 Declaration.

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <string>
#include "LitterBox/Serialization/Serializer.h"

namespace LB
{
	/*!***********************************************************************
	  \brief
	  This struct stores the Sprite information. The sprite will have a:
	  constructor of
	  m_index - index of the sprite
	  m_min - min value of the sprite
	  m_max - max value of the sprite
	  it also stores the serialise and deserialise data
	*************************************************************************/
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

	/*!***********************************************************************
	  \brief
	  This class SpriteSheet have all the declarations of the what would a SpriteSheet
	  needs.
	*************************************************************************/
	class SpriteSheet
	{
	public:
		/*!***********************************************************************
		  \brief
		  construct the name of the spritesheet, pngName, row and col
		*************************************************************************/
		SpriteSheet() : m_name{ "Unnamed Sheet" }, m_pngName{ "No PNG" }, m_row{ 0 }, m_col{ 0 } {}

		/*!***********************************************************************
		  \brief
		  SpriteSheet copy constructor string name and pngName
		*************************************************************************/
		SpriteSheet(std::string const& name);
		SpriteSheet(std::string const& name, std::string const& PNGName);

		/*!***********************************************************************
		  \brief
		  serialising to save the spritesheet
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc); //to save 

		/*!***********************************************************************
		  \brief
		  deserialising the sprites to load it
		*************************************************************************/
		bool Deserialize(const Value& data); //to load

		/*!***********************************************************************
		  \brief
		  Slice function tto store in the vector
		*************************************************************************/
		void Slice(Vec2<float> min, Vec2<float> max);

		/*!***********************************************************************
		  \brief
		  At the sprite index
		*************************************************************************/
		Sprite& At(int index);

		/*!***********************************************************************
		  \brief
		  This is so you can access operator [] in the sprite
		*************************************************************************/
		Sprite& operator[](int index);

		/*!***********************************************************************
		  \brief
		  This is so you can access operator [] in the sprite
		*************************************************************************/
		Sprite const& operator[](int index) const;
		std::vector<Sprite>& Sprites();

		/*!***********************************************************************
		  \brief
		  Getting the Png reference and returns a string name
		*************************************************************************/
		std::string const& GetPNGRef() const;
		void SetPNGRef(std::string const& newPNG);

		/*!***********************************************************************
		  \brief
		  Getting the name of the sprite
		*************************************************************************/
		std::string const& GetName() const;

		/*!***********************************************************************
		  \brief
		  Setting the name of the sprite
		*************************************************************************/
		void SetName(std::string const& newName);

		/*!***********************************************************************
		  \brief
		  Size of the Sprites vector
		*************************************************************************/
		size_t Size();

		// TO BE REFACTORED
		/*!***********************************************************************
		  \brief
		  rows and cols of the spritesheet
		*************************************************************************/
		int m_row, m_col;

	private:
		std::string m_name, m_pngName; //getting the string of the name
		std::vector<Sprite> m_sprites; //store width and height of each sprite
	};
}