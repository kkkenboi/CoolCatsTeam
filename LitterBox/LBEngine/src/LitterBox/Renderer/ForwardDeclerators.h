/*!************************************************************************
 \file                ForwardDeclerators.h
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                29-09-2023
 \brief

 Header file declaring animation class and Animation_Manager. This was done
 because it was originally in Renderer.h but then it was difficult to include
 Renderer.h in other systems.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "LitterBox/Serialization/Serializer.h"
#include <utility>

namespace Renderer {

	//----------------------------------------ANIMATION--------------------------------
	//Have function that will take in a render_Object and change the uv to the uv in containers
	/*!***********************************************************************
	\brief

	Animation class that holds all the relevant information needed to play the
	animation.
	*************************************************************************/
	class Animation {
	private:
		const float playback;
		const float increment;
		const int frame_count;
		const std::array<LB::Vec2<float>, 4>* arr;
	public:
		bool repeat{ false };
		/*!***********************************************************************
		\brief
		 Animation object constructor that takes in data and assigns them to its
		 member variables.
		*************************************************************************/
		Animation(const float pb, const int fc, const std::array<LB::Vec2<float>, 4>* ptr) : playback{ pb }, increment{ pb / (float)fc }, frame_count{ fc }, arr{ ptr } {}
		/*!***********************************************************************
		\brief
		 Function to get the runtime of the animation in seconds.

		\return
		 The runtime of the animation in seconds.
		*************************************************************************/
		inline const float get_length() const { return playback; }
		/*!***********************************************************************
		\brief
		 Function to get the number of frames in the animation.

		\return
		 The number of frames in the animation.
		*************************************************************************/
		inline const int get_frame_count() const { return frame_count; }
		/*!***********************************************************************
		\brief
		 Function to get the 4 UV coordinates of a specific frame in the animation.

		 NOTE: no boundary checking is done in the function. The onous is on you.

		\return
		 The 4 UV coordinates of the specified frame.
		*************************************************************************/
		inline const std::array<LB::Vec2<float>, 4>* get_uv(int offset = 0) const { return arr + offset; }
		/*!***********************************************************************
		\brief
		 Gets the increment which is how long the animator should stay on each frame
		 for.

		\return
		 Incrementor which states how long a
		*************************************************************************/
		inline const float get_inc() const { return increment; }
	};

	/*!***********************************************************************
	\brief

	Animation _Manager class keeps track and stores all the animation ojects
	and data for the lifetime of the program.

	NOTE: The array of the animation is just a pointer for now so you have
	to ensure the data exists while the specific animation is still playing.
	*************************************************************************/
	class Animation_Manager {
		std::map<std::string, const Animation> animations;
	public:

		/*!***********************************************************************
		\brief
		 load_anim is the API for the animatino manager to create and store the
		 animation objects with the right data.
		*************************************************************************/
		void load_anim(const std::string& animation_name, const std::array<LB::Vec2<float>, 4>* data, const float anim_time, const int number_of_frames);
		/*!***********************************************************************
		\brief
		 find_animation is the API the Animation_Manager provides to search through
		 it's map and retun the appropriate animation.

		\return
		 If the key and animation exists then it returns a pointer to the aniamtion
		 but if the key and value do not exist then returns a nullptr.
		*************************************************************************/
		const Animation* find_animation(const std::string& name) const { return animations.find(name) != animations.end() ? &(animations.find(name)->second) : nullptr; }
	};
	//----------------------------------------ANIMATION--------------------------------
	/*!***********************************************************************
	\brief
	 Enum for the render objects to determine what type its going to be.
	*************************************************************************/
	enum class Renderer_Types {
		RT_OBJECT,
		RT_BACKGROUND,
		RT_DEBUG,
		RT_UI,
		Last
	};

	Renderer_Types& operator++(Renderer_Types& rt);

	//NOTE: because index data type is unsigned short
	//the maximum number of vectors allowed is 65535
	//therefore maximum number of quads allowed is 16383

	/*!***********************************************************************
	\brief
	 Struct of the indices required to draw one quad.
	*************************************************************************/
	struct index {
		std::array<unsigned short, 6> indexes;
	};

	class RenderSystem;

	/*!***********************************************************************
	\brief
	 message is a structure that contains the data necessary to print custom
	 text at specified locations on the screen with changeable color.
	*************************************************************************/
	struct message {
		std::string text{"Hello World"};
		float x{}, y{}, scale{1.0f}, xbound{100.0f}, scaleSaved{};
		LB::Vec3<float> color{};
		std::string font_file_name_wo_ext{"ZillaSlab-Regular"};

		/*!***********************************************************************
		 \brief
		 Serializes all the rendering information
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc)
		{
			DebuggerLog("Serializing message");
			data.SetObject();
			Value textValue(text.c_str(), alloc);
			data.AddMember("text", textValue, alloc);
			data.AddMember("x", x, alloc);
			data.AddMember("y", y, alloc);
			data.AddMember("scale", scale, alloc);
			Value colorValue;
			if (color.Serialize(colorValue, alloc))
			{
				data.AddMember("Color", colorValue, alloc);
			}
			else return false;
			Value fontNameValue(font_file_name_wo_ext.c_str(), alloc);
			data.AddMember("Font name", fontNameValue, alloc);
			return true;

		}

		/*!***********************************************************************
		 \brief
		 Deserializes all the rendering information
		*************************************************************************/
		bool Deserialize(const Value& data)
		{
			bool HasText = data.HasMember("text");
			bool HasX = data.HasMember("x");
			bool HasY = data.HasMember("y");
			bool HasScale = data.HasMember("scale");
			bool HasColor = data.HasMember("Color");
			bool HasFontName = data.HasMember("Font name");
			if (HasText)
			{
				const Value& textValue = data["text"];
				text = textValue.GetString();
			}
			if (HasX && HasY && HasScale && HasColor)
			{
				const Value& xValue = data["x"];
				const Value& yValue = data["y"];
				const Value& scaleValue = data["scale"];
				const Value& colorValue = data["Color"];
				x = xValue.GetFloat(); y = yValue.GetFloat();
				scale = scaleValue.GetFloat();
				scaleSaved = scale;
				color.Deserialize(colorValue);
			}
			if (HasFontName)
			{
				const Value& textValue = data["Font name"];
				font_file_name_wo_ext = textValue.GetString();
			}
			return true;
		}
	};
}