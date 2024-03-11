/*!************************************************************************
 \file                Camera.cpp
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                28-09-2023
 \brief
 
 File containing definition for Camera class's member functions.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "Camera.h"
#include "Platform\Windows\Windows.h"

namespace Renderer {
	/*!***********************************************************************
	\brief
	 Contrustor for Camera class object that does ortho graphic projection
	 with the left and bottom being 0 and the right and top being screen width
	 and height respectively. Also creates a world->NDC matrix to be used.
	*************************************************************************/
	Camera::Camera() :
		ortho{},
		world_NDC{}
	{
		float onear = 100.f;
		float ofar = -100.f;
		float hvf = 1080.f;
		float wvf = 1920.f;
		float lvf = 0.f;
		float rvf = wvf;

		zoomx = 2.f / rvf;
		zoomy = 2.f / hvf;

		ortho = { zoomx, 0.f, 0.f, 0.f,
				 0.f, zoomy, 0.f, 0.f,
				 0.f, 0.f, -2.f/(ofar - onear), 0.f,
				 -(rvf + lvf) / (rvf - lvf), -1.f, -(ofar+onear)/(ofar-onear), 1.f};

		editor_ortho = ortho;

		float near { 100.f };
		float far{ -100.f };
		//complete the rest of the perspective matrix
		aspect = (float)LB::WINDOWSSYSTEM->GetWidth() / (float)LB::WINDOWSSYSTEM->GetHeight();

		perspective =
		{
			(2.f) / (rvf), 0.f,0.f,0.f,
			0.f, (2.f) / hvf, 0.f, 0.f,
			//rvf / rvf, hvf / hvf, -((far + near) / (far - near)), -1.f,
			0.2f, 0.2f, -((far + near) / (far - near)), -1.f,
			0.f, 0.f, -((2.f*near * far) / (far - near)), 0.f
		};

		editor_world_NDC = editor_ortho * free_cam_coords;
		world_NDC = ortho * nel;
		ui_NDC = world_NDC;
	}

	/*!***********************************************************************
	\brief
	 free_cam_move moves the free cam in the scene view

	\param new_pos
	 The direction and movement vector that adds to the camera's current
	 position
	*************************************************************************/
	void Camera::free_cam_move(LB::Vec2<float> new_pos)
	{
		cam_pos.x += new_pos.x;
		cam_pos.y += new_pos.y;
		free_cam_coords = glm::inverse(glm::mat4{ o_right, o_up, o_w, cam_pos });
		editor_world_NDC = editor_ortho * free_cam_coords;
	}

	/*!***********************************************************************
	\brief
	 Function to update the free cam projection matrix to either zoom further
	 into the scene view or further away.

	\param zoom
	 The zoom in percentage. 1.f = normal, 0.5f = zoomed in, 1.5f = zoomed out
	*************************************************************************/
	void Camera::free_cam_zoom(float zoom) {
		editor_ortho[0][0] = zoomx * zoom;
		editor_ortho[1][1] = zoomy * zoom;

		editor_world_NDC = editor_ortho * free_cam_coords;
	}

	/*!***********************************************************************
	 \brief
	 Returns the coordinates for the free camera
	*************************************************************************/
	glm::mat4 Camera::get_free_cam() const
	{
		return free_cam_coords;
	}

	/*!***********************************************************************
	 \brief
	 Returns the camera position
	*************************************************************************/
	glm::vec3 Camera::get_cam_pos() const
	{
		return cam_pos;
	}

}