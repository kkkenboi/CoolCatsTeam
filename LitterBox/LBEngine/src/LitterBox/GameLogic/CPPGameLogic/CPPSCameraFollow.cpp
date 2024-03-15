#include "CPPSCameraFollow.h"
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
	}

	/*!***********************************************************************
	\brief
	Updates the camera to follow the player
	*************************************************************************/
	void CPPSCameraFollow::Update()
	{
		//First we just store our current position so that it's easier to use
		currentPos = GetComponent<CPTransform>()->GetPosition();
		//Forgive me father for I have sinned
		//This sort of lerping is technically incorrect but it will do... 
		currentPos = Lerp(currentPos, mPlayer->GetComponent<CPTransform>()->GetPosition(), static_cast<float>(TIME->GetDeltaTime()));
		//std::cout << "player Pos : " << mPlayer->GetComponent<CPTransform>()->GetPosition().x << ", " << mPlayer->GetComponent<CPTransform>()->GetPosition().y;
		//std::cout << ", current pos : " << currentPos.x << ", " << currentPos.y << '\n';
		//Now we set this thing's position
		GetComponent<CPTransform>()->SetPosition(currentPos);
		//why won't you move!!! ;__;
		//DebuggerLog("Renderer:\n");
		Renderer::GRAPHICS->get_cam()->get_cam_x() = currentPos.x - 960.f;
		Renderer::GRAPHICS->get_cam()->get_cam_y() = currentPos.y - 540.f;
		Renderer::GRAPHICS->get_cam()->update_cam();
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
}	