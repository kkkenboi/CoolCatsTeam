
#include "Core.h"

namespace LB
{
    void LBEngine::Create()
    {
        // Initialize all systems
        for (auto& type : rttr::type::get_types()) {
            if (type.is_derived_from<ISystem>()) {
                std::shared_ptr<ISystem> newSystem = std::make_shared<ISystem>(type.create().get_value<ISystem*>());
                m_systems.push_back(newSystem);

                if (type.is_derived_from<IUpdateSystem>()) {
                    m_updateSystems.push_back(newSystem);
                }
                if (type.is_derived_from<IFixedUpdateSystem>()) {
					m_fixedUpdateSystems.push_back(newSystem);
				}
            }
        }

        // Order the systems

	}
}
