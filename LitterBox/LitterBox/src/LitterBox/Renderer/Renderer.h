#pragma once
#include "LitterBox/Core/System.h"
#include "Platform\Windows\Windows.h"
#include "Camera.h"
#include "ForwardDeclerators.h"
#include "LitterBox/Factory/Components.h"
#include <utility>
#include <array>
#include <list>
#include <map>
#include <queue>
#include <glm.hpp>
#include "LitterBox/Serialization/AssetManager.h"

//-----------------------------------------HELPER FUNCTIONS--------------------------------
struct shader_source {
	std::string vtx_shd;
	std::string frg_shd;
};
shader_source shader_parser(const char* shader_file_name);
unsigned int compile_shader(const char*& source, unsigned int type);
unsigned int create_shader(const char* vertex_shader, const char* fragment_shader);
//-----------------------------------------HELPER FUNCTIONS--------------------------------

namespace Renderer {
	//-------------Structs to make things easier for now--------
	struct vec2 {
		float x, y;
	};

	struct vec3 {
		float x, y, z;
	};

	struct vec4 {
		vec3 xyz;
		float w;


	};
	//----------------------------------------------------------

	//-----------------------Vertex data------------------------
	struct Vertex {
		vec2 pos{ 0.5f, 0.5f };
		vec2 tex{ 0.f,0.f };
		vec3 color{ 1.f,0.f,0.f };
		float texIndex{ -1.f };
		unsigned int index{ 0 };
		bool active{ false };
	};

	//Array of 4 vertices for drawing quads on screen
	struct quad {
		std::array<Vertex, 4> data;
	};

	//NOTE: because index data type is unsigned short
	//the maximum number of vectors allowed is 65535
	//therefore maximum number of quads allowed is 16383
	struct index {
		std::array<unsigned short, 6> indexes;
	};
	//-----------------------Vertex data------------------------

	//---------------------TEXTURES-------------------------------------------
	class Texture {
	private:
		unsigned int id;
		std::string file_path;
		unsigned char* local_buff;
		int w, h, fluff;

	public:
		Texture(const std::string& path);
		~Texture();

		const unsigned int get_tex() const { return id; }
	};

	//TODO CHANGE THE LIMIT FOR TEXTURES TO BE BASED ON HARDWARE LIMITS
	//OF TEXTURE UNIT
	class Texture_Manager {
	private:
		bool free[32];
		std::map<std::string, std::pair<const Texture*, int>> textures;
	public:
		//initialize every variable in free to false: IMPORTANT
		Texture_Manager() : free{ false } {}
		bool add_texture(const std::string& file_path, const std::string& name);
		bool remove_texture(const std::string& name);

		const int get_texture_index(const std::string& name) const { return textures.find(name)->second.second; }

		void flush_textures();
	};
	//------------------------------------------------------------------------
	
	//------------------------------------------------RENDERING SPECIFIC------------------------------------------------
	//Renderer class will be incharge the vao, shader program and buffers.
	//Renderer class will not be exposed to the programmers and is meant-
	//-to have a level of disconnect even from render_object class

	//TODO change render class so that it handles adding of new objects and renderer
	//and that render objects only need to call the function.
	class Renderer {
	private:
		unsigned int vao;
		unsigned int vbo;
		unsigned int ibo;
		quad* quad_buff;
		std::vector<index> index_buff;
		unsigned int quad_buff_size;

		std::list<const LB::CPRender*> active_objs;
		//TODO figure out parsing in shader file paths
		//std::string shader_file_path

	public:
		Camera cam;

		Renderer(const Renderer_Types& renderer);
		~Renderer();

		inline unsigned int get_vao() { return vao; }
		inline unsigned int get_ibo() { return ibo; }
		inline size_t get_ao_size() { return active_objs.size(); }
		inline unsigned int get_vbo() { return vbo; }

		unsigned int create_render_object(const LB::CPRender* obj);
		void remove_render_object(const LB::CPRender* obj);
		void update_buff(Renderer_Types r_type);
	};

	//The actual system that will get initialized into the engine
	class RenderSystem : public LB::ISystem {
	private:
		//TODO figure out some way in the serialization process
		//how to pass in object limit for renderer
		unsigned int shader_program;

		Texture_Manager t_Manager;
		Animation_Manager a_Manager;

		Renderer bg_renderer;
		Renderer object_renderer;
		Renderer debug_renderer;
	public:

		RenderSystem();
		~RenderSystem();

		virtual void Update();
		virtual void Draw();
		void Initialize() override { SetSystemName("Renderer System"); }

		bool create_texture(const std::string& file_path, const std::string& name);
		bool remove_texture(const std::string& name);
		const int get_texture(const std::string& name) const { return t_Manager.get_texture_index(name); }
		void flush_textures();

		void init_anim(const std::string& animation_name, const std::array<LB::Vec2<float>, 4>* data, const float anim_time, const int number_of_frames) {
			a_Manager.load_anim(animation_name, data, anim_time, number_of_frames);
		}
		auto get_anim(const std::string& name) const { return a_Manager.find_animation(name); }

		unsigned int create_object(Renderer_Types r_type, const LB::CPRender* obj);
		void remove_object(Renderer_Types r_type, const LB::CPRender* obj);

		inline unsigned int get_shader() { return shader_program; }
	};

	//A pointer to the system object in the core engine
	//made to be singleton
	extern RenderSystem* GRAPHICS;

	//Render object is an object that will be exposed to the programmers in the level creator
	//class CPRender : public LB::IComponent {
	//private:
	//	const Renderer_Types							renderer_id;
	//	unsigned int									quad_id;
	//	unsigned int									frame;
	//	float											time_elapsed;
	//	std::queue<std::pair<const Animation*, bool>>	animation;

	//public:
	//	vec2						position;
	//	float						scal;
	//	float						w;
	//	float						h;
	//	vec3						col;
	//	std::array<vec2, 4>			uv; //bot left, bot right, top right, top left
	//	int							texture;
	//	bool						activated;

	//	CPRender(
	//		vec2 pos = { 0.f, 0.f }, 
	//		float width = 1.f, 
	//		float height = 1.f, 
	//		float scale = 1.f, 
	//		vec3 color = { 0.f,0.f,0.f }, 
	//		std::array<vec2,4> uv = {}, 
	//		int texture = -1, 
	//		bool active = true,
	//		Renderer_Types rend_type = Renderer_Types::RT_OBJECT);
	//	~CPRender();
	//	
	//	inline const unsigned int get_index() const { return quad_id; }
	//	inline const size_t get_queue_size() const { return animation.size(); }
	//	inline const Renderer_Types get_r_type() const { return renderer_id; }

	//	inline bool operator==(const CPRender& rhs) const {
	//		return quad_id == rhs.quad_id;
	//	}

	//	void play_repeat(const std::string& name);
	//	void play_next(const std::string& name);
	//	void play_now(const std::string& name);

	//	inline void get_transform_data() {  };

	//	void animate();
	//};

	//------------------------------------------------RENDERING SPECIFIC------------------------------------------------
}