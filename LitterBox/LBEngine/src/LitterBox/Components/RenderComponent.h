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
#include <queue>

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
		const Renderer::Renderer_Types					renderer_id;
		unsigned int									quad_id;
		unsigned int									frame;
		float											time_elapsed;
		std::queue<std::pair<const  Renderer::Animation*, bool>>	animation;
		Renderer::index									indices;
		bool initialized{ false };

	public:
		float						w;
		float						h;
		float						rotation;
		int							texture;
		bool						activated;
		Vec2<float>					scal;
		Vec2<float>					position;
		Vec3<float>					col;
		std::array<Vec2<float>, 4>	uv; //bot left, bot right, top right, top left



		CPTransform* transform;
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
		/*!***********************************************************************
		\brief
		 Component object destructor
		*************************************************************************/
		~CPRender();

		ComponentTypeID GetType() override
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
		bool Serialize(Value& data, Document::AllocatorType& alloc) override
		{
			data.SetObject();
			Value textureName;
			data.AddMember("Texture", texture, alloc);
			return true;
		}
		bool Deserialize(const Value& data) override
		{
			bool HasTexture = data.HasMember("Texture");
			if (data.IsObject())
			{
				if (HasTexture)
				{
					const Value& textureValue = data["Texture"];
					texture = textureValue.GetInt();
					return true;
				}
			}
			return false;
		}
		/*!***********************************************************************
		\brief
		 Gets the quad_id of the component that was given to it during construction.

		\return
		 The id of the render component.
		*************************************************************************/
		inline const unsigned int get_index() const { return quad_id; }
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
		void UpdateTexture(int _texture)
		{
			texture = _texture;
			uv[0] = { 0.f, 0.f };
			uv[1] = { 1.f, 0.f };
			uv[2] = { 1.f, 1.f };
			uv[3] = { 0.f, 1.f };
		}

		/*!***********************************************************************
		\brief
		 Function that pulls data from the transform component of the game object
		*************************************************************************/
		inline void get_transform_data() {
			position = initialized ? transform->GetPosition() : position;
			rotation = initialized ? transform->GetRotation() : rotation;
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
	};
}