#pragma once
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	class CPPSVFXManager : public CPPBehaviour
	{
	public :
		void Start() override;
		void Update() override;
		void Destroy() override;

		void SpawnExplosion(Vec2<float> pos, float scale = 1.0f);
		void SpawnHitAnim(Vec2<float> pos);
		void SpawnPoofAnim(Vec2<float> pos, float scaleMult);
	private:
		
	};
	REGISTER_SCRIPT(CPPSVFXManager)
}