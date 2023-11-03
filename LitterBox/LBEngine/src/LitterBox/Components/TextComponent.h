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

		inline void update_msg_text(const std::string& str);
		inline void update_msg_color(const LB::Vec3<float>& col);
		inline void update_msg_size(float font_size);
		inline void update_msg_pos(const LB::Vec2<float>& pos);
		inline void update_msg_font(const std::string& file_name_wo_ext);
		
		inline const std::string& get_msg_text() const;
		inline const LB::Vec3<float>& get_msg_color() const;
		inline const float& get_msg_size() const;
		inline const std::string& get_msg_font() const;
		inline Renderer::message& get_msg();
	};
}