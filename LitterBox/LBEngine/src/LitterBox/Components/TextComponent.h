#pragma once
#include "Component.h"
#include <string>
#include "LitterBox/Renderer/ForwardDeclerators.h"

namespace LB {
	class CPText : public IComponent {
	private:
		Renderer::message msg;
	public:
		~CPText() { Destroy(); }

		void Initialise() override;
		void Destroy() override;
		void Update() override;

		void update_msg_text(const std::string& str);
		void update_msg_color(const LB::Vec3<float>& col);
		void update_msg_size(float font_size);
		void update_msg_pos(const LB::Vec2<float>& pos);
		
		const std::string& get_msg_text() const;
		const LB::Vec3<float>& get_msg_color() const;
		const float& get_msg_size() const;
		Renderer::message& get_msg();
	};
}