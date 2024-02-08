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
		
		float mEmitterRate{ 0.05 };
		float mTimeSinceLastEmit{};

		// Velocities
		Vec2<float> mEmitterVelocity{};
		float mEmitterVariationMinX{};
		float mEmitterVariationMaxX{};
		float mEmitterVariationMinY{};
		float mEmitterVariationMaxY{};

		float mEmitterRadialSpeed{ 100.f };
		// Left : -X, 0
		// Right : X, 0
		// Up : 0, Y
		// Down : 0, -Y

		// Sizes
		float mEmitterSizeBegin{ 1 };
		float mEmitterSizeEnd{ 1 };
		
		float mEmitterLifetime{ 1 };
		float mEmitterLifetimeRemaining{};

		// Particle
		float mParticleLifetime{1};

		// For Radial Emitters
		int mRadialParticles{ 10 };

		bool mIsLooping{ false };
		bool mIsActive{ false };
	};
}