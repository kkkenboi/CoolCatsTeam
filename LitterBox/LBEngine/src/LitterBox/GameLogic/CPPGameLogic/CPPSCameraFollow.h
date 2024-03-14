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
		Finds the player to follow
		*************************************************************************/
		void Start() override;

		/*!***********************************************************************
		\brief
		Updates the camera to follow the player
		*************************************************************************/
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