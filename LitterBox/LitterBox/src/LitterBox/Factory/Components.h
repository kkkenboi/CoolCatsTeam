/*!************************************************************************
 \file				Components.h
 \author(s)			Kenji Brannon Chong, Ryan Tan Jian Hao
 \par DP email(s):	kenjibrannon.c@digipen.edu, ryanjianhao.tan\@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				
 This file contains functions of the IComponent, CPTransform, CPRender, 
 CPRigidBody, ComponentMaker and ComponentMakerType class.

 IComponent acts as the interface for CPTransform, CPRender, CPRigidBody to 
 build off from to create components to be attached to GameObjects. These 
 derived components are then updated by their own systems.
					
 ComponentMaker acts as the interface for different ComponentMakerType 
 classes to create ComponentMaker of those types. ComponentMakers are 
 incharge to create components of that type.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include <Litterbox/Engine/Message.h>
#include <LitterBox/Debugging/Memory.h>
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Physics/Collisions.h"
#include "LitterBox/Renderer/ForwardDeclerators.h"
#include "Platform/Windows/Windows.h"
#include "LitterBox/Serialization/Serializer.h"
#include <queue>

namespace LB
{
	enum ComponentTypeID
	{
		C_CPNone = 0,
		C_CPRigidBody,
		C_CPTransform,
		C_CPRender,
		C_CPScript
	};

	/*!***********************************************************************
	 \brief
	 IComponent class provides an interface for other components to build off
	 from.
	*************************************************************************/
	class IComponent
	{
	public:
		/*!***********************************************************************
		 \brief
		 Initialises the component
		*************************************************************************/
		virtual void Initialise() {}

		/*!***********************************************************************
		 \brief
		 Updates the component
		*************************************************************************/
		virtual void Update() {}

		/*!***********************************************************************
		 \brief
		 Serialises the components based on its derived member's data
		*************************************************************************/
		virtual bool Serialize(Value&, Document::AllocatorType&) { return false; }

		/*!***********************************************************************
		 \brief
		 Deserialises the components based on its derived member's data
		*************************************************************************/
		virtual bool Deserialize(const Value&) { return false; }

		/*!***********************************************************************
		 \brief
		 Destroys the component
		*************************************************************************/
		virtual void Destroy() {}

		/*!***********************************************************************
		 \brief
		 Gets the type of the component
		*************************************************************************/
		virtual ComponentTypeID GetType() = 0;

		//ComponentTypeID TypeID	{ C_CPNone };
		GameObject*		gameObj		{ nullptr };
	};

	/*!***********************************************************************
	\brief
	 CPTransform is a default component that every game object will have. It
	 contains a position vector, scale vector and rotation vector.
	*************************************************************************/
	class CPTransform : public IComponent
	{
	public:
		ComponentTypeID GetType() override
		{
			return C_CPTransform;
		}

		/*!***********************************************************************
		\brief
		 Function will serialize data from an outer json file to get data.
		
		\return
		 returns true if serialization succeeds and false if not.
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc) override
		{
			std::cout << "Serialising Transform\n";
			data.SetObject();
			Value PositionValue;
			if (pos.Serialize(PositionValue, alloc))
			{
				data.AddMember("Position", PositionValue, alloc);
			}
			else return false;
			Value ScaleValue;
			if (scale.Serialize(ScaleValue, alloc))
			{
				data.AddMember("Scale", ScaleValue, alloc);
			}
			else return false;
			data.AddMember("Rotation", angle, alloc);
			return true;
		}

		/*!***********************************************************************
		\brief
		 Function will save data from scene to json file.
		
		\return
		 returns true if deserialization succeeds and false if not.
		*************************************************************************/
		bool Deserialize(const Value& data) override
		{
			bool HasPosition = data.HasMember("Position");
			bool HasScale = data.HasMember("Scale");
			bool HasRot = data.HasMember("Rotation");
			std::cout << "Deserialising Transform\n";
			if (data.IsObject())
			{
				if (HasPosition && HasScale && HasRot)
				{
					const Value& positionValue = data["Position"];
					const Value& scaleValue = data["Scale"];
					const Value& rotationValue = data["Rotation"];
					pos.Deserialize(positionValue);
					scale.Deserialize(scaleValue);
					angle = rotationValue.GetFloat();
					return true;
				}
			}
			return false;
		}
		
		/*!***********************************************************************
		\brief
		 Component Destroy function
		*************************************************************************/
		void Destroy() override
		{
			std::cout << "Destroying Transform\n";
		}
		
		/*!***********************************************************************
		\brief
		 Getter function to get the position of the game object transform
		
		\return
		 2D vector of the position of the game object(Transform)
		*************************************************************************/
		Vec2<float> GetPosition() const
		{
			return pos;
		}

		/*!***********************************************************************
		\brief
		 Setter function to set the position of the game object transform
		*************************************************************************/
		void SetPosition(Vec2<float> const& newPos)
		{
			pos = newPos;
		}

		/*!***********************************************************************
		\brief
		 Getter function to get the scale of the game object transform
		
		\return
		 2D vector of the x and y scale of the game object(Transform)
		*************************************************************************/
		Vec2<float> GetScale() const
		{
			return scale;
		}

		/*!***********************************************************************
		\brief
		 Setter function to set the x and y scale of the game object transform
		*************************************************************************/
		void SetScale(Vec2<float> const& newScale)
		{
			scale = newScale;
		}

		/*!***********************************************************************
		\brief
		 Getter function to get the rotation of the game object transform
		
		\return
		 Clockwise angle in radians of the game object
		*************************************************************************/
		float GetRotation() const
		{
			return angle;
		}

		/*!***********************************************************************
		\brief
		 Setter function to set the clockwise angle in radians of the game object 
		 transform
		*************************************************************************/
		void SetRotation(float newRotation)
		{
			angle = newRotation;
		}

	private:
		Vec2<float> pos{}, scale{1.0f, 1.0f};
		float angle{};
	};

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
			Vec2<float> scale = {1.f, 1.f},
			Vec3<float>	 color = { 0.f,0.f,0.f },
			std::array<Vec2<float>, 4> uv = {},
			int texture = -1,
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
			data.AddMember("Texture", texture,alloc);
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


	class CPRigidBody : public IComponent 
	{
	public:

		// Vectors here allow the user to know
		// the current and previous location of the object
		LB::Vec2<float> mPosition;
		LB::Vec2<float> mPrevPosition;

		// Vectors here allow the user to know
		// which direection the object is moving towards
		LB::Vec2<float> mVelocity;
		LB::Vec2<float> mAcceleration;

		float mRotation;
		float mRotationalVelocity;

	public:
		CPTransform* transform;

		// These data will not change will be set upon initialization
		float mDensity;
		float mMass; // Used for F = MA
		float mInvMass;
		float mRestitution; // How bouncy the object is
		float mArea;

		float mFriction;

		bool isStatic;
		bool isActive;

		// For storing data for circles or rectangles
		float mRadius;
		float mWidth;
		float mHeight;

		SHAPETYPE mShapeType;

		// =====
		// TEST STUFF
		// =====
		int mNumberID;
		// ==============

		// =====
		//  WRAP THIS SECTION IN A COLLIDER CLASS, separating from RigidBody
		// =====
		// Vertices and AABB

		// Untransformed vertices (Vertices from origin)
		LB::Vec2<float> mVertices[4];
		LB::Vec2<float> mTransformedVertices[4];

		AABB obj_aabb;
		// =====
		// PLS WRAP
		// =====

		bool mUpdateVerticesRequired;
		bool mUpdateAABBRequired;

		// =================
		// Constructor
		// =================
		void Initialise() override
		{
			CreateRigidBody();
		}

		ComponentTypeID GetType() override
		{
			return C_CPRigidBody;
		}

		/*!***********************************************************************
			\brief
			Creates a CPRigidBody with default parameters stated in the function
			definition
		*************************************************************************/
		void CreateRigidBody ();
		bool Serialize(Value& data, Document::AllocatorType& alloc) override
		{
			std::cout << "Serialising RB\n";
			data.SetObject();
			data.AddMember("Width", mWidth, alloc);
			data.AddMember("Height", mHeight, alloc);
			data.AddMember("Density", mDensity,alloc);
			return true;
		}
		bool Deserialize(const Value& data) override
		{
			std::cout << "Deserialising RB\n";
			bool HasWidth = data.HasMember("Width");
			bool HasHeight = data.HasMember("Height");
			bool HasDensity = data.HasMember("Density");
			if (data.IsObject())
			{
				if (HasWidth && HasHeight && HasDensity)
				{
					mWidth = data["Width"].GetFloat();
					mHeight = data["Height"].GetFloat();
					mDensity = data["Density"].GetFloat();
					return true;
				}
			}
			return false;
		}

	public:
		/*!***********************************************************************
			\brief
			Gets the position of the CPRigidBody
		*************************************************************************/
		LB::Vec2<float> getPos();

		/*!***********************************************************************
			\brief
			Adds a Force to the CPRigidBody
		*************************************************************************/
		void addForce(LB::Vec2<float> force);

		/*!***********************************************************************
			\brief
			Adds an Impulse to the CPRigidBody
		*************************************************************************/
		void addImpulse(LB::Vec2<float> force);

		/*!***********************************************************************
			\brief
			Moves the CPRigidBody directly with a vector
		*************************************************************************/
		void Move(LB::Vec2<float> vec);

		/*!***********************************************************************
			\brief
			Moves the CPRigidBody directly to a certain position
		*************************************************************************/
		void MoveTo(LB::Vec2<float> position);

		/*!***********************************************************************
			\brief
			Updates the CPRigidBody Box Vertices within its' data members
		*************************************************************************/
		void UpdateRigidBodyBoxVertices();

		/*!***********************************************************************
			\brief
			Updates the AABB collider in the CPRigidBody's data members
		*************************************************************************/
		void UpdateRigidBodyAABB();

		/*!***********************************************************************
			\brief
			Updates the position of the CPRigidBody
		*************************************************************************/
		void UpdateRigidBodyPos(float time);

		/*!***********************************************************************
			\brief
			Updates the CPRigidBody's velocity
		*************************************************************************/
		void UpdateRigidBodyVel(float time);

		/*!***********************************************************************
			\brief
			Override of the FixedUpdate() of IComponent
			Updates the RigidBody within a fixed timestep
			- Semi-implicit Euler system, updating velocity first then positions
			- Also updates the Transform IComponent that is stored in CPRigidBody
		*************************************************************************/
		void FixedUpdate();

		/*!***********************************************************************
			\brief
			This is the function that calls the debug drawer to draw all the
			RigidBody collision boxes as well as their velocities
		*************************************************************************/
		void DebugDraw();

	};

	// Interface to make components and tag IDs
	class ComponentMaker
	{
	public:
		ComponentMaker(ComponentTypeID typeId) : TypeID(typeId) {}

		ComponentTypeID TypeID;
		///Create the component
		virtual IComponent* Create() = 0;
		virtual ~ComponentMaker() {};

	};

	// Class Template to create different types of ComponentMaker
	// ComponentType here refers to the derived classes of IComponent
	template<typename ComponentType>
	class ComponentMakerType : public ComponentMaker
	{
	public:
		ComponentMakerType(ComponentTypeID typeId) : ComponentMaker(typeId) {}

		IComponent* Create() override
		{
			// Returns a constructor to the derived class of IComponent
			return new ComponentType();
		}
	};

}

#define CreateComponentMaker(ComponentType, ComponentTypeID) FACTORY->InitCM (ComponentTypeID, new ComponentMakerType<ComponentType>( ComponentTypeID ) );