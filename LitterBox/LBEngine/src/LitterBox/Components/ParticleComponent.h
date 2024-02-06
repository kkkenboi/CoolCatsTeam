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

		// CPParticle stuff
		std::string GetEmitterType();

		void RestartLifetime();
		

	public:
		CPTransform* mTransform;
		CPRender* mRender;

		EmitterType mEmitterType;

		Vec2<float> mEmitterPos;
		
		float mEmitterRate;
		float mTimeSinceLastEmit;

		// Velocities
		Vec2<float> mEmitterVelocity;
		float mEmitterVariationMinX;
		float mEmitterVariationMaxX;
		float mEmitterVariationMinY;
		float mEmitterVariationMaxY;

		// Left : -X, 0
		// Right : X, 0
		// Up : 0, Y
		// Down : 0, -Y

		// Sizes
		float mEmitterSizeBegin;
		float mEmitterSizeEnd;
		
		float mEmitterLifetime;
		float mEmitterLifetimeRemaining;

		// Texture here

		bool mIsActive;
	};
}