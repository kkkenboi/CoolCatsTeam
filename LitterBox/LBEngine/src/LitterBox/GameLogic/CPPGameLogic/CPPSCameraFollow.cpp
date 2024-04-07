/*!************************************************************************
 * \file				CPPSCameraFollow.cpp
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD
 * \date				15/03/2024
 * \brief 				Source file for camera follow logic
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "CPPSCameraFollow.h"
#include "CPPSMouse.h"
#include "CPPGameManager.h"

#include "LitterBox/Renderer/Renderer.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Finds the player to follow
	*************************************************************************/
	void CPPSCameraFollow::Start()
	{
		mPlayer = GOMANAGER->FindGameObjectWithName("MainChar");
		m_Mouse = GOMANAGER->FindGameObjectWithName("MouseCursor");

	}

	/*!***********************************************************************
	\brief
	Updates the camera to follow the player
	*************************************************************************/
	void CPPSCameraFollow::Update()
	{
		// TO REFACTOR
		static bool init{ false };
		if (!init)
		{
			// Update the camera bounds on level change
			GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->onNewMapStart.Subscribe(UpdateCamMaxBounds);
			UpdateCamMaxBounds();
			init = false;
		}

		//First we just store our current position so that it's easier to use
		currentPos = GetComponent<CPTransform>()->GetPosition();

		// Calculate movement of camera when exploring with the mouse
		extraDist = m_Mouse->GetComponent<CPPSMouse>()->GetMouseWorld()->GetComponent<CPTransform>()->GetPosition() - mPlayer->GetComponent<CPTransform>()->GetPosition();
		extraDist.Clamp(-200.f, 200.f);

		//Forgive me father for I have sinned
		//This sort of lerping is technically incorrect but it will do... 
		currentPos = Lerp(currentPos, mPlayer->GetComponent<CPTransform>()->GetPosition() + extraDist, static_cast<float>(TIME->GetDeltaTime() * 2.5f));
		//std::cout << "player Pos : " << mPlayer->GetComponent<CPTransform>()->GetPosition().x << ", " << mPlayer->GetComponent<CPTransform>()->GetPosition().y;
		//std::cout << ", current pos : " << currentPos.x << ", " << currentPos.y << '\n';

		// Then we clamp the camera to the map bounds
		currentPos.x = Clamp(currentPos.x, maxBoundsX.x, maxBoundsX.y);
		currentPos.y = Clamp(currentPos.y, maxBoundsY.x, maxBoundsY.y);


		//Now we set this thing's position
		GetComponent<CPTransform>()->SetPosition(currentPos);
		//why won't you move!!! ;__;
		//DebuggerLog("Renderer:\n");

		//DebuggerLogFormat("ExtraDist X: %f, ExtraDist Y: %f", extraDist.x, extraDist.y);
		/*Renderer::GRAPHICS->get_cam()->get_cam_x() = currentPos.x = 960.f;
		Renderer::GRAPHICS->get_cam()->get_cam_y() = currentPos.y = 540.f;*/
		currentPos.x -= 960.f;
		currentPos.y -= 540.f;
		Renderer::GRAPHICS->get_cam()->update_ortho_cam(currentPos);
		//Renderer::GRAPHICS->get_cam()->update_cam();
		cameraPos.x = Renderer::GRAPHICS->get_cam()->get_cam_x();
		cameraPos.y = Renderer::GRAPHICS->get_cam()->get_cam_y();
		//DebuggerLogFormat("CameraFollow Position: %f, Camera Position: %f, Player Position: %f, Mouse Position: %f", currentPos.x, Renderer::GRAPHICS->get_cam()->get_cam_x(), mPlayer->GetComponent<CPTransform>()->GetPosition().x, INPUT->GetMousePos().x);
		
		//Renderer::GRAPHICS->get_cam().update_ortho_cam(currentPos);
		//Renderer::GRAPHICS->get_cam().free_cam_move(currentPos);
		//Renderer::GRAPHICS->update_cam(currentPos.x, currentPos.y);		//doesn't work, moves scene cam to some wacky pos instead
		
	}
	void CPPSCameraFollow::Destroy()
	{
	}

	/*!***********************************************************************
	\brief
	 Returns true if the object is visible to the camera
	*************************************************************************/
	bool CPPSCameraFollow::IsVisible(CPTransform const* obj) const
	{
		Vec2<float> objPos = obj->GetPosition();

		if (objPos.x < cameraPos.x - 75.f || objPos.x > cameraPos.x + 1995.f
			|| objPos.y < cameraPos.y - 75.f || objPos.y > cameraPos.y + 1155.f)
		{
			return false;
		}
		return true;
	}

	/*!***********************************************************************
	\brief
	 Event to update the camera bounds based on map
	*************************************************************************/
	void UpdateCamMaxBounds()
	{
		MapDetails currentMap = GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->m_currentMap;
		CPPSCameraFollow* camFollow = GOMANAGER->FindGameObjectWithName("CameraFollow")->GetComponent<CPPSCameraFollow>();
		camFollow->maxBoundsX.x = currentMap.m_xbounds.x + 960.f;
		camFollow->maxBoundsX.y = currentMap.m_xbounds.y - 960.f;
		camFollow->maxBoundsY.x = currentMap.m_ybounds.x + 540.f;
		camFollow->maxBoundsY.y = currentMap.m_ybounds.y - 540.f;

		// If not in a main map
		if (GOMANAGER->FindGameObjectWithName("GameManager")->GetComponent<CPPSGameManager>()->GetCurrentWave() == 0)
		{
			camFollow->maxBoundsX.x = -630.f + 960.f;
			camFollow->maxBoundsX.y = 2560.f - 960.f;
			camFollow->maxBoundsY.x = -465.f + 540.f;
			camFollow->maxBoundsY.y = 1560.f - 540.f;
		}
	}
}	