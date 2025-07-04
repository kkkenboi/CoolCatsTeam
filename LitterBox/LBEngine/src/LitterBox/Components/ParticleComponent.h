/*!************************************************************************
 \file				ParticleComponent.h
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2451A
 \date				09-02-2024
 \brief
  This file contains the ParticleManager class which contains
  all the declarations of the class' functions. It handles
  the Particle Pool and the Emitter Pool by updating the
  positions and stats of Particles that were emitted by an
  Emitter.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Engine/Time.h"

namespace LB {

	enum EmitterType
	{
		TRAIL = 0,
		RADIAL = 1,
		INVERSERADIAL = 2
	};

	class CPParticle : public IComponent {
	public:
		// IComponent stuff
		/*!***********************************************************************
		\brief
		Initialises some of the variables in the CPParticle class as well
		as adding the newly created CPParticle into the Emitter Pool within
		the Particle Manager
		*************************************************************************/
		void Initialise() override;
		/*!***********************************************************************
			\brief
			Updates the CPParticle's position and other stats
		*************************************************************************/
		void Update() override;
		/*!***********************************************************************
			\brief
			Removes the emitter from the Emitter Pool in the ParticleManager class
		*************************************************************************/
		void Destroy() override;
		/*!***********************************************************************
			\brief
			Gets the ComponentTypeID of the CPParticle class
		*************************************************************************/
		static ComponentTypeID GetType();

		/*!***********************************************************************
			\brief
			Serializes the CPParticle component
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc) override;
		/*!***********************************************************************
			\brief
			Deserializes the CPParticle component
		*************************************************************************/
		bool Deserialize(const Value& data) override;
		// CPParticle stuff
		/*!***********************************************************************
			\brief
			Returns the current emitter type of the CPParticle component as a string
		*************************************************************************/
		std::string GetEmitterType();

		/*!***********************************************************************
			\brief
			Restarts the lifetime remaining of the CPParticle component
		*************************************************************************/
		void RestartLifetime();

		void SetDeletion(bool state);

		void SetDeletionTime(float time);
	public:
		CPTransform* mTransform;
		CPRender* mRender;

		EmitterType mEmitterType = TRAIL;

		Vec2<float> mEmitterPos{};
		
		float mEmitterRate{ 0.05f };

		bool mEmitterRateRandomness{ false };
		float mEmitterRateRandomnessMin{ 0.05f };
		float mEmitterRateRandomnessMax{ 0.15f };

		float mTimeSinceLastEmit{0.f};

		// Velocities
		Vec2<float> mEmitterVelocity{0.f,0.f};
		float mEmitterVariationMinX{0.f};
		float mEmitterVariationMaxX{0.f};
		float mEmitterVariationMinY{0.f};
		float mEmitterVariationMaxY{0.f};

		float mEmitterRadialSpeed{ 100.f };
		// Left : -X, 0
		// Right : X, 0
		// Up : 0, Y
		// Down : 0, -Y

		// Gravity
		bool mIsGravityOn{ false };

		// Sizes
		float mEmitterSizeBegin{ 1.f };
		float mEmitterSizeEnd{ 1.f };
		
		// Lifetimes
		float mEmitterLifetime{ 1.f };
		float mEmitterLifetimeRemaining{1.f};
		float mEmitterLifetimeDelayMin{ 0.1f };
		float mEmitterLifetimeDelayMax{ 0.1f };

		// Particle
		float mParticleLifetime{1.f};
		bool mParticleLifetimeRandomness{ false };
		float mParticleLifetimeMin{ 0.5f };
		float mParticleLifetimeMax{ 1.5 };

		// For Radial Emitters
		int mRadialParticles{ 10 };

		bool mIsLooping{ false };
		bool mIsActive{ false };

		// Deletion
		float mEmitterDeletionLifetime{ 1.f };
		float mEmitterDeletionLifetimeRemaining{ 1.f };
		bool mWillDelete{ false };

		// Rotation randonmization
		float mRotationSpeed{ 0.f };
		float mRotationSpeedVariationMin{ -500.f };
		float mRotationSpeedVariationMax{ 500.f };

		// Inverse Radial Emitters
		// Need how far away from the center
		bool mEmitterRadialSpawnCircle{ false };
		float mInvRadDistanceMin{ 0.f };
		float mInvRadDistanceMax{ 0.f };
	};
}