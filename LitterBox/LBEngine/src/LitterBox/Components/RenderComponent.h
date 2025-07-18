/*!************************************************************************
 \file				RenderComponent.h
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan\@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief
 This file contains functions of the CPRender class.

 CPRender is a component that allows the GameObject to be rendered on screen.
 Without this render component, the game object will be invisible.
 This component also handles the playing of animations.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "TransformComponent.h"
#include "LitterBox/Renderer/ForwardDeclerators.h"
#include "LitterBox/Animation/SpriteSheet.h"
//#include "LitterBox/Serialization/AssetManager.h"
#include <queue>


const float deg_to_rads{ static_cast<float>(PI) / 180.f };

namespace LB
{
	/*!***********************************************************************
	\brief
	 CPRender is the render object that is the component to add into a game
	 object to get it to show something on the screen.
	*************************************************************************/
	class CPRender : public IComponent
	{
	private:
		Renderer::Renderer_Types					renderer_id;
		unsigned int								quad_id;
		unsigned int								frame;
		float										time_elapsed;
		std::queue<std::pair
			<const  Renderer::Animation*, bool>
		>											animation;
		Renderer::index								indices;
		bool initialized{ false };
		bool destroyed{ false };

	public:
		float						w;
		float						h;
		float						rotation, z_val;
		int							texture;
		Vec2<float>					scal;
		Vec2<float>					position;
		Vec3<float>					col;
		std::array<Vec2<float>, 4>	uv; //bot left, bot right, top right, top left
		std::string					spriteSheetName{ "None" };
		int							spriteIndex{ -1 };
		bool						activated;


		CPTransform* transform;
		GameObject* parent;
		/*!***********************************************************************
		\brief
		 Constructor for the component. Sets all the appropriate values and
		 calls the function to place itself into the appropriate Renderer classes
		*************************************************************************/
		CPRender(
			Vec2<float>	 pos = { 0.f, 0.f },
			float width = 100.f,
			float height = 100.f,
			Vec2<float> scale = { 1.f, 1.f },
			Vec3<float>	 color = { 0.f,0.f,0.f },
			std::array<Vec2<float>, 4> uv = {},
			int texture = 0,
			bool active = true,
			Renderer::Renderer_Types rend_type = Renderer::Renderer_Types::RT_OBJECT);

		~CPRender();

		/*!***********************************************************************
		\brief
		 Returns the component of the Gameobject as a CPRender type
		*************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPRender;
		}

		/*!***********************************************************************
		\brief
		 Function that gets the transform component tied to a game object and
		 sets the initialized flag to true so the component does not start reading
		 from an unitilized or nullptr.
		*************************************************************************/
		void Initialise() override
		{
			transform = gameObj->GetComponent<CPTransform>();
			initialized = true;
		}

		/*!************************************************************************
		 * \brief
		 * Saves the texture name to file
		 * \return
		 * Returns if the serialization succeeded
		**************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc) override;

		/*!************************************************************************
		 * \brief
		 * Reads the texture name and sets the ID appropriately
		 * \return
		 * If the Deserialization succeeded
		**************************************************************************/
		bool Deserialize(const Value& data) override;

		/*!***********************************************************************
		 \brief
		 Toggles the ACTUAL active state for this component
		*************************************************************************/
		void ToggleActive(bool isActive) override;

		/*!***********************************************************************
		\brief
		 Gets the quad_id of the component that was given to it during construction.

		\return
		 The id of the render component.
		*************************************************************************/
		inline const unsigned int get_index() const { return quad_id; }
		/*!***********************************************************************
		\brief
		 Set the index of the render object.
		 NOTE: Do not touch this ever. Only here because layering system is annoying.

		\param
		 The new index we want to give the obj
		*************************************************************************/
		void set_index(unsigned int idx) { quad_id = idx; }
		/*!***********************************************************************
		\brief
		 Update the indices of the object for active inactive
		 NOTE: Do not touch this ever. Only here because layering system is annoying.
		*************************************************************************/
		void update_indices() {
			unsigned short idx{ (unsigned short)(quad_id * 4) };

			indices = Renderer::index{ std::array<unsigned short, 6>{idx, (unsigned short)(idx + 1), (unsigned short)(idx + 2),
					(unsigned short)(idx + 2), (unsigned short)(idx + 3), idx} };
		}
		/*!***********************************************************************
		\brief
		 Gets the number of animations queued for the specific game object.

		\return
		 The number of animations queued in the component
		*************************************************************************/
		inline const size_t get_queue_size() const { return animation.size(); }
		/*!***********************************************************************
		\brief
		 Gets the type of rendering object/component and also the target Renderer.

		\return
		 The rendering type of the component default is RT_OBJECT
		*************************************************************************/
		inline const Renderer::Renderer_Types get_r_type() const { return renderer_id; }

		/*!***********************************************************************
		\brief
		 Updates which rendering layer the object is in.

		 NOTE: This function does not change the rendering layer

		\param
		 The new rendering layer that the object has been placed in
		*************************************************************************/
		inline void set_r_type(Renderer::Renderer_Types type) { renderer_id = type; }

		/*!***********************************************************************
		\brief
		 Overloaded comparison operator for the lambda function for removing
		 render objects from a renderer's active object list.

		\return
		 bool whether this object and the compared object has the same unique_id
		*************************************************************************/
		inline bool operator==(const CPRender& rhs) const {
			return quad_id == rhs.quad_id;
		}
		/*!***********************************************************************
		\brief
		 Loads the animation into the queue with a flag that tells the animation
		 function to play that animation on repeat.
		*************************************************************************/
		void play_repeat(const std::string& name);
		/*!***********************************************************************
		\brief
		 Loads an animation into the queue and makes it wait till queue pops all
		 the other animations infront of it.
		*************************************************************************/
		void play_next(const std::string& name);
		/*!***********************************************************************
		\brief
		 Pops every animtion currently in the queue before pushing the given animation
		 into the queue.
		*************************************************************************/
		void play_now(const std::string& name);
		/*!***********************************************************************
		\brief
		 Pops of all animation currently in the animation queue.
		*************************************************************************/
		void stop_anim();

		/*!***********************************************************************
		\brief
		 Sets the texture variable to a provided texture slot and sets the UV
		 coorindates to be defaulted.

		 NOTE: the texture slot may or may not be bound. The ounus is on the
		 developer to ensure the _texture argument is a valid texture
		*************************************************************************/
		void UpdateTexture(int _texture, int width, int height, Vec2<float>min = { 0.f, 0.f }, Vec2<float>max = { 1.f, 1.f })
		{
			texture = _texture;
			w = (float)width;
			h = (float)height;
			if (_texture == -1)
			{
				uv[0] = min;//{ 0.f, 0.f };
				uv[1] = min;//{ 0.f, 0.f };
				uv[2] = min;//{ 0.f, 0.f };
				uv[3] = min;//{ 0.f, 0.f };
			}
			else
			{
				uv[0] = min;//{ 0.f, 0.f };
				uv[1] = { max.x, min.y };//{ 1.f, 0.f };
				uv[2] = max;//{ 1.f, 1.f };
				uv[3] = { min.x, max.y };//{ 0.f, 1.f };
			}
		}

		/*!***********************************************************************
		\brief
		 Sets the sprite of the render component based on the spritesheet and index
		*************************************************************************/
		void SetSpriteTexture(std::string SpriteSheetName, int index);

		/*!***********************************************************************
		\brief
		 Function that pulls data from the transform component of the game object
		*************************************************************************/
		inline void get_transform_data() {
			position = initialized ? transform->GetPosition() : position;
			rotation = initialized ? transform->GetRotation() * deg_to_rads : rotation;
			scal = initialized ? transform->GetScale() : scal;
		};

		/*!***********************************************************************
		\brief
		 Function that gets the indices of the quad the CPRender object is using
		*************************************************************************/
		inline const Renderer::index get_indices() const {
			return indices;
		}

		/*!***********************************************************************
		\brief
		 Function that is incharge of play the animation in the front of the queue
		 based on time and not frames.
		*************************************************************************/
		void animate();

		/*!***********************************************************************
		\brief
		 Turns on or off the rendering of the particular instance of the class.
		 Depending on the previous state of the object.
		*************************************************************************/
		void set_active();

		/*!***********************************************************************
		\brief
		 Destroy acts as the destructor for the CPRender object and is the API for
		 deallocating the resources given to the component from the RenderSystem
		 side.
		*************************************************************************/
		inline void Destroy() override;
	};
}