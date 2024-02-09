#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Engine/Time.h"

namespace LB {

	enum EmitterType
	{
		TRAIL = 0,
		RADIAL = 1
	};

	class CPParticle : public IComponent {
	public:
		// IComponent stuff
		/*!***********************************************************************
		\brief
		Initialises the ParticleManager class by making sure all the indexes for
		the EmitterPool and the ParticlePool are correct as well as the
		types of emitters
		*************************************************************************/
		void Initialise() override;

		void Update() override;

		void Destroy() override;
		static ComponentTypeID GetType();

		bool Serialize(Value& data, Document::AllocatorType& alloc) override;
		bool Deserialize(const Value& data) override;
		// CPParticle stuff
		std::string GetEmitterType();

		void RestartLifetime();

	public:
		CPTransform* mTransform;
		CPRender* mRender;

		EmitterType mEmitterType = TRAIL;

		Vec2<float> mEmitterPos{};
		
		float mEmitterRate{ 0.05f };
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

		// Sizes
		float mEmitterSizeBegin{ 1.f };
		float mEmitterSizeEnd{ 1.f };
		
		float mEmitterLifetime{ 1.f };
		float mEmitterLifetimeRemaining{1.f};

		// Particle
		float mParticleLifetime{1.f};

		// For Radial Emitters
		int mRadialParticles{ 10 };

		bool mIsLooping{ false };
		bool mIsActive{ false };
	};
}