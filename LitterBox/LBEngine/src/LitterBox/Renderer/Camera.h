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
#include "LitterBox/Utils/Math.h"

namespace Renderer {
	//------------------pre defines for the ortho graphic cam-----------------
	constexpr static glm::vec4 o_up{ 0.f,1.f,0.f, 0.f };
	constexpr static glm::vec4 o_right{ 1.f,0.f,0.f, 0.f };
	constexpr static glm::vec4 o_w{ 0.f,0.f,1.f, 0.f };
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
		glm::vec4 p_up{ 0.f,1.f,0.f,0.f };
		glm::vec4 p_right{ 1.f,0.f,0.f, 0.f };
		glm::vec4 p_w{ 0.f,0.f,1.f, 0.f };
		//------------------------------------------------------------------

		glm::mat4 inv_mat{ o_right, o_up, o_w, pos };
		//for game cam inverse
		glm::mat4 nel{ glm::inverse(inv_mat) };
		//for free cam inverse
		glm::mat4 free_cam_coords{nel};

		//-------------Required to move a free cam-------------
		double longitude{}, azimuth{};
		float zoomx{}, zoomy{}, aspect{};

		glm::vec3 view_vector{ 0.f, 0.f, 1.f };
		glm::vec4 cam_pos{ 0.f, 0.f, 5.f, 1.f };

		glm::vec3 basis_vec_i{};
		glm::vec3 basis_vec_j{};
		glm::vec3 basis_vec_k{};
		//-------------Required to move a free cam-------------

	public:

		double mouse_x{}, mouse_y{}, x_disp{1.f}, y_disp{};
		
		//-------------The projection matrices------------
		glm::mat4 ortho{}, editor_ortho{};
		glm::mat4 perspective{};
		//-------------The projection matrices------------

		glm::mat4 world_NDC{};
		glm::mat4 editor_world_NDC{};
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
		void move_cam(float intensity) {
			pos.x = -intensity * 0.25f + static_cast<float>(rand()) * intensity / static_cast<float>(RAND_MAX);
			pos.y = -intensity * 0.25f + static_cast<float>(rand()) * intensity / static_cast<float>(RAND_MAX);
			nel = glm::inverse(glm::mat4{ o_right, o_up, o_w, pos });
			world_NDC = ortho * nel;
		}

		/*!***********************************************************************
		\brief
		 Function to reset camera to starting position.
		*************************************************************************/
		void reset_cam()
		{
			pos.x = 0.f;
			pos.y = 0.f;
		}

		/*!***********************************************************************
		\brief
		 Function to update the orthographic camera/game camera position. Which
		 means the camera can be moved around the x and y axis.

		\param
		 Is a vector that contains the new x and y positions in world space
		*************************************************************************/
		void update_ortho_cam(LB::Vec2<float> new_pos) {
			pos.x = new_pos.x;
			pos.y = new_pos.y;
			nel = glm::inverse(glm::mat4{ o_right, o_up, o_w, pos });
			world_NDC = ortho * nel;
		}
		/*!***********************************************************************
		\brief
		 Function to update the free cam projection matrix to either zoom further
		 into the scene view or further away.

		\param zoom
		 The zoom in percentage. 1.f = normal, 0.5f = zoomed in, 1.5f = zoomed out
		*************************************************************************/
		void free_cam_zoom(float zoom = 1.f);
		/*!***********************************************************************
		\brief
		 free_cam_move moves the free cam in the scene view

		\param new_pos
		 The direction and movement vector that adds to the camera's current
		 position
		*************************************************************************/
		void free_cam_move(LB::Vec2<float> new_pos);

		/*!***********************************************************************
		 \brief
		 Returns the x and y projection values of the game camera.

		 NOTE: The values given are the horizontal and vertical bound of the
		 projection matrix. (Which is 1 / distance)
		*************************************************************************/
		inline LB::Vec2<float> zoomed() { return LB::Vec2<float>{editor_ortho[0][0], editor_ortho[1][1]}; }

		/*!***********************************************************************
		 \brief
		 Returns the coordinates for the free camera
		*************************************************************************/
		glm::mat4 get_free_cam() const;

		/*!***********************************************************************
		 \brief
		 Returns the camera position
		*************************************************************************/
		glm::vec3 get_cam_pos() const;

		/*!***********************************************************************
		 \brief
		 Returns reference to camera's x position
		*************************************************************************/
		float& get_cam_x() { return pos.x; }
		/*!***********************************************************************
		 \brief
		 Returns reference to camera's y position
		*************************************************************************/
		float& get_cam_y() { return pos.y; }

		void update_cam()
		{
			nel = glm::inverse(glm::mat4{ o_right, o_up, o_w, pos });
			world_NDC = ortho * nel;
		}
	};
	//----------------------------------------CAMERA-----------------------------------
}