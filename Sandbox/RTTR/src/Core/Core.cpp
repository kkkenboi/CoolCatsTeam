
#include "rttr/registration.h"
#include "Core.h"

namespace LB
{
	void LBEngine::Initialize()
	{
        for (auto& type : rttr::type::get_types()) {
            if (type.is_derived_from<ISystem>()) {
                m_systems.push_back(std::make_shared<ISystem>(type.create().get_value<ISystem*>()));
            }
        }
	}
}