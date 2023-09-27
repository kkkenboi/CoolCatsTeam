#include "Camera.h"
#include "Platform\Windows\Windows.h"

namespace Renderer {
	Camera::Camera() :
		ortho{ 2.f / LB::WINDOWSSYSTEM->GetWidth(), 0.f ,0.f, 0.f,
						  0.f, 2.f / LB::WINDOWSSYSTEM->GetHeight(), 0.f, 0.f,
						  0.f, 0.f, 0.2f, 0.f,
						  -1.f, -1.f, -(0.2f), 1.f },
		world_NDC{ ortho }
	{
		//near = 4.f
		//far = -6.f
		float hvf = (float)LB::WINDOWSSYSTEM->GetHeight();
		float wvf = (float)LB::WINDOWSSYSTEM->GetWidth();
		float lvf = 0.f;
		float rvf = wvf;
		ortho = { 2.f / rvf - lvf, 0.f, 0.f, 0.f,
				 0.f, 2.f / hvf, 0.f, 0.f,
				 0.f, 0.f, 0.2f, 0.f,
				 -(rvf + lvf) / (rvf - lvf), -1.f, -0.2f, 1.f };
		world_NDC = ortho * nel;
	}
}