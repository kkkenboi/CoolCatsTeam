#pragma once
#include <glm.hpp>

namespace Renderer {

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

		Camera();

		void move_cam() {
			pos.x = -10.f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (20.f));
			pos.y = -10.f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (20.f));
			nel = glm::inverse(glm::mat4{ right, up, w, pos });
			world_NDC = ortho * nel;
		}
	};
	//----------------------------------------CAMERA-----------------------------------
}