#pragma once

namespace Renderer {

	//----------------------------------------ANIMATION--------------------------------
	//Have function that will take in a render_Object and change the uv to the uv in containers
	class Animation {
	private:
		const float playback;
		const float increment;
		const int frame_count;
		const std::array<LB::Vec2<float>, 4>* arr;
	public:
		bool repeat{ false };
		//TODO make sure that ptr eventually points to memory in the heap
		Animation(const float pb, const int fc, const std::array<LB::Vec2<float>, 4>* ptr) : playback{ pb }, increment{ pb / (float)fc }, frame_count{ fc }, arr{ ptr } {}

		inline const float get_length() const { return playback; }
		inline const int get_frame_count() const { return frame_count; }
		inline const std::array<LB::Vec2<float>, 4>* get_uv(int offset = 0) const { return arr + offset; }
		inline const float get_inc() const { return increment; }
	};

	class Animation_Manager {
		std::map<std::string, const Animation> animations;
	public:
		void load_anim(const std::string& animation_name, const std::array<LB::Vec2<float>, 4>* data, const float anim_time, const int number_of_frames);
		const Animation* find_animation(const std::string& name) const { return animations.find(name) != animations.end() ? &(animations.find(name)->second) : nullptr; }
	};
	//----------------------------------------ANIMATION--------------------------------

	enum class Renderer_Types {
		RT_OBJECT,
		RT_BACKGROUND,
		RT_DEBUG,
		RT_UI
	};
}