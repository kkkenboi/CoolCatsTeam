#pragma once
#include <Litterbox/Engine/Message.h>
#include <LitterBox/Debugging/Memory.h>
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Physics/Collisions.h"
#include "LitterBox/Renderer/ForwardDeclerators.h"
#include "Platform/Windows/Windows.h"
#include <queue>
//#include "LitterBox/Physics/RigidBody.h"

namespace LB
{
	// --can be changed to be serialised--
	enum ComponentTypeID
	{
		C_CPNone = 0,
		C_CPRigidBody,
		C_CPTransform,
		C_CPRender
	};

	// Interface for derived components to use as a base
	class IComponent
	{
	public:
		virtual void Initialise() {};
		virtual void Serialise() {};
		virtual void Update() {};
		virtual void Deserialise() {};
		virtual void Destroy() {};
		// To destruct all other derived components

		ComponentTypeID GetType()	{ return TypeID; }

		ComponentTypeID TypeID		{ C_CPNone };
		GameObject* gameObj			{ nullptr };
	//protected:
	};

	class CPTransform : public IComponent
	{
	public:
		void Initialise() override
		{
			std::cout << "Initialising Transform\n";
		}
		void Serialise() override
		{
			std::cout << "Serialising Transform\n";
		}
		void Deserialise() override
		{
			std::cout << "Deserialising Transform\n";
		}
		void Destroy() override
		{
			std::cout << "Destroying Transform\n";
		}

		Vec2<float> GetPosition() const
		{
			return pos;
		}

		void SetPosition(Vec2<float> const& newPos)
		{
			pos = newPos;
		}

		Vec2<float> GetScale() const
		{
			return scale;
		}

		void SetScale(Vec2<float> const& newScale)
		{
			scale = newScale;
		}

		float GetRotation() const
		{
			return angle;
		}

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
		/*!***********************************************************************
		\brief
		 Function that gets the transform component tied to a game object and
		 sets the initialized flag to true so the component does not start reading
		 from an unitilized or nullptr.
		*************************************************************************/
		void Initialise() override {
			transform = gameObj->GetComponent<CPTransform>("CPTransform");
			initialized = true;
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
		 Function that is incharge of play the animation in the front of the queue
		 based on time and not frames.
		*************************************************************************/
		void animate();
	};


	class CPRigidBody : public IComponent {


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
			Start();
		}

		void Start (LB::Vec2<float> position = LB::Vec2<float>{ 0.f,0.f },
			LB::Vec2<float> prevposition = LB::Vec2<float>{ 0.f, 0.f },
			LB::Vec2<float> velocity = LB::Vec2<float>{ 0.f, 0.f },
			LB::Vec2<float> acceleration = LB::Vec2<float>{ 0.f, 0.f },
			float rotation = 0.0f, float rotationvelocity = 0.0f, float density = 0.0f, float mass = 0.0f, float invmass = 0.0f, float restitution = 0.0f,
			float area = 0.0f, float friction = 0.0f, bool isstatic = false, bool isactive = false, float radius = 0.0f, float width = 0.0f, float height = 0.0f,
			SHAPETYPE shape = SHAPETYPE::NONE);


	public:

		LB::Vec2<float> getPos();

		// Force is added upon object's velocity
		void addForce(LB::Vec2<float> force);

		// Just to move the RigidBody with a vector
		void Move(LB::Vec2<float> vec);

		// Move the RigidBody to a specific position
		void MoveTo(LB::Vec2<float> position);

		// Updates the TransformedVertices of the RigidBody
		// Accounts for positional changes and rotational changes
		void UpdateRigidBodyBoxVertices();

		void UpdateRigidBodyAABB();

		// Updates the position of the RigidBody
		void UpdateRigidBodyPos(float time);

		// Updates the velocities of the RigidBody
		void UpdateRigidBodyVel(float time);

		// Updates the RigidBody with all the steps
		// Using a semi-implicit euler system, updating velocity first
		// then updating positions
		// Side note: Might be impulsed based system instead
		// therefore accel = force / mass should be introduced later
		void FixedUpdate();

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

#define CreateComponentMaker(ComponentType) FACTORY->InitCM ( #ComponentType, new ComponentMakerType<ComponentType>( C_##ComponentType ) );