/*!************************************************************************
 \file                TextComponent.h
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                03-11-2023
 \brief

 Header file that contains the definitions for the CPText component class.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "Component.h"
#include <string>
#include "LitterBox/Renderer/ForwardDeclerators.h"

namespace LB {
	/*!***********************************************************************
	\brief
	 TextComponent class is the class responsible for being able to print
	 text onto the screen

	 NOTE: the text only prints on the game view and not the scene view.
	 - Implementation is also defined in Renderer.cpp for now
	*************************************************************************/
	class CPText : public IComponent {
	private:
		Renderer::message msg;
	public:
		~CPText() { Destroy(); }
		/*!***********************************************************************
		\brief
		 Initialises the text object component by adding it to the active list of
		 text objects to render.
		*************************************************************************/
		void Initialise() override;
		/*!***********************************************************************
		\brief
		 Removes the text object comopnent from active list in text object renderer
		*************************************************************************/
		void Destroy() override;
		/*!***********************************************************************
		\brief
		 Update will move the text object component based on the game objects
		 transform.

		 NOTE: currently using self-defined positions for UI right now
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		\brief
		 Serializes all the information needed for rendering text
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc);

		/*!***********************************************************************
		\brief
		 Deserializes (loads) all the information needed for rendering text
		*************************************************************************/
		bool Deserialize(const Value& data);

		/*!***********************************************************************
		\brief
		Returns the type of this component for identification
		*************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPText;
		}
		/*!***********************************************************************
		\brief
		 Updates the text to be printed on the screen

		\param str
		 The text to be rendered
		*************************************************************************/
		inline void update_msg_text(const std::string& str);
		/*!***********************************************************************
		\brief
		 Updates the font color of the text

		 NOTE: format is RGB and values go from 0 - 1.f

		\param col
		 The values of the new color in vector format
		*************************************************************************/
		inline void update_msg_color(const LB::Vec3<float>& col);
		/*!***********************************************************************
		\brief
		 Updates the scale of the text

		 NOTE: font_size means scale for font size to be multiplied by. 2.f is double
		 of 1.f and so on.

		\param font_size
		 The new scaling value for the text
		*************************************************************************/
		inline void update_msg_size(float font_size);
		/*!***********************************************************************
		\brief
		 Updates the position of the text on the screen

		 NOTE: position is the bottom left of the text.

		\param pos
		 The new position in vector format
		*************************************************************************/
		inline void update_msg_pos(const LB::Vec2<float>& pos);
		/*!***********************************************************************
		\brief
		 Updates the font that will be used to print the text object message

		 NOTE: The font will be loaded as the file name of the font without the
		 extension. E.g. "Ariel.ttf" file, the font name will be "Ariel"

		\param file_name_wo_ext
		 Font file name without the extension
		*************************************************************************/
		inline void update_msg_font(const std::string& file_name_wo_ext);

		/*!***********************************************************************
		\brief
		 Getter method to get the string text of the message/CPText object
		*************************************************************************/
		inline const std::string& get_msg_text() const;
		/*!***********************************************************************
		\brief
		 Getter method to get the font color of the CPText object in Vec3 format
		 RGB from 0 - 1.f
		*************************************************************************/
		inline const LB::Vec3<float>& get_msg_color() const;
		/*!***********************************************************************
		\brief
		 Getter method to get the scaling value of the CPText object
		*************************************************************************/
		inline const float& get_msg_size() const;
		/*!***********************************************************************
		\brief
		 Getter method to get the string text of the font used by the CPText object
		*************************************************************************/
		inline const std::string& get_msg_font() const;
		/*!***********************************************************************
		\brief
		 Getter method to get the entire message object in CPText object that
		 contains all necessary information for the text to be printed
		*************************************************************************/
		inline Renderer::message& get_msg();

		/*!***********************************************************************
		 \brief
		 Updates the text displayed
		*************************************************************************/
		void set_msg(char* _msg)
		{
			msg.text = _msg;
		}
	};
}