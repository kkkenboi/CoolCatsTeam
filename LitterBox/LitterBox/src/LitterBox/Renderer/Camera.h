/*!************************************************************************
 \file                Camera.h
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                28-09-2023
 \brief

 Header file declaring the Camera class function and it's member functinon

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include <glm.hpp>

namespace Renderer {
	/*!***********************************************************************
	\brief
	
	Camera class to us to define the window to the world. Or at least to 
	determine what's infront of the camera. It's supposed to be for clipping
	but that's more of an M2 thing.
	*************************************************************************/
	//----------------------------------------CAMERA-----------------------------------
	class Camera {
		//--------premade values so it doesn't look like I'm hardcoding-----
		glm::vec4 pos{ 0.f, 0.f, 5.f, 1.f };
		glm::vec4 up{ 0.f,1.f,0.f, 0.f };
		glm::vec4 right{ 1.f,0.f,0.f, 0.f };
		glm::vec4 w{ 0.f,0.f,1.f, 0.f };
		//------------------------------------------------------------------
		//The values defined above are already the inverse values
		glm::mat4 inv_mat{ right, up, w, pos };
		glm::mat4 nel{ glm::inverse(inv_mat) };
		glm::mat4 ortho{};


	public:
		glm::mat4 world_NDC{};
		/*!***********************************************************************
		\brief
		 Contrustor for Camera class object that does ortho graphic projection
		 with the left and bottom being 0 and the right and top being screen width
		 and height respectively. Also creates a world->NDC matrix to be used.
		*************************************************************************/
		Camera();

		/*!***********************************************************************
		\brief
		 Function to perform camera shake by randomising the camera x y position
		 values. There is no way to return the camera position to its default so
		 use wisely.

		 NOTE: This is mainly for testing the matrix multiplications but it was
		 very fun to have so I kept it in, please don't be mad.
		*************************************************************************/
		void move_cam() {
			pos.x = -10.f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (20.f));
			pos.y = -10.f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (20.f));
			nel = glm::inverse(glm::mat4{ right, up, w, pos });
			world_NDC = ortho * nel;
		}
	};
	//----------------------------------------CAMERA-----------------------------------
}