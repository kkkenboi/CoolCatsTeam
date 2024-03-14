#pragma once
#include "LitterBox/Renderer/Camera.h"
#include "CPPBehaviour.h"
#include "LitterBox/Factory/Components.h"

namespace LB
{
	// Use GameObj to access the gameobj of the class
	class CPPSCameraFollow : public CPPBehaviour
	{
	public:
		/*!***********************************************************************
		\brief
		Inherited Functions from CPPBehaviour that the user must implement
		*************************************************************************/
		void Start() override;
		void Update() override;
		void Destroy() override;

		Vec2<float> cameraPos{};
	private:
		GameObject* mPlayer{ nullptr };
		Vec2<float> currentPos{};
		Vec2<float> cachedPosition{};

	};
	REGISTER_SCRIPT(CPPSCameraFollow)
}