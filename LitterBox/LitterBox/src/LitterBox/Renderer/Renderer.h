#pragma once
#include "LitterBox/Core/System.h"
#include "Platform\Windows\Windows.h"
#include <utility>
#include <array>
#include <list>
#include <map>
#include <queue>
//TODO CHANGE TO RELATIVE PATH
#include <glm.hpp>

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

	class render_Object; //Forward declerator

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

	//----------------------------------------CAMERA-----------------------------------
	class Camera {
		//--------premade values so it doesn't look like I'm hardcoding-----
		glm::vec4 pos	{ 0.f, 0.f, 5.f, 1.f };
		glm::vec4 up	{ 0.f,1.f,0.f, 0.f };
		glm::vec4 right	{ 1.f,0.f,0.f, 0.f };
		glm::vec4 w		{ 0.f,0.f,1.f, 0.f };
		//------------------------------------------------------------------
		//The values defined above are already the inverse values
		glm::mat4 inv_mat{ right, up, w, pos };
		glm::mat4 nel{ glm::inverse(inv_mat) };
		glm::mat4 ortho{2.f / LB::WINDOWSSYSTEM->GetWidth(), 0.f ,0.f, 0.f,
					  0.f, 2.f / LB::WINDOWSSYSTEM->GetHeight(), 0.f, 0.f,
		              0.f, 0.f, 0.2f, 0.f, 
					  -1.f, -1.f, -(0.2f), 1.f};

		
	public:
		glm::mat4 world_NDC {ortho};

		Camera() { 
			std::cout << "WIDTH: " << LB::WINDOWSSYSTEM->GetWidth() << " HEIGHT: " << LB::WINDOWSSYSTEM->GetHeight() << std::endl;
			float hvf = LB::WINDOWSSYSTEM->GetHeight();
			float wvf = LB::WINDOWSSYSTEM->GetWidth();
			float left = 0.f;
			float right = wvf;
			ortho = { 2.f / right - left, 0.f, 0.f, 0.f,
					 0.f, 2.f / hvf, 0.f, 0.f,
					 0.f, 0.f, 0.2f, 0.f,
					 -(right + left)/(right - left), -1.f, -0.2f, 1.f};
			world_NDC = ortho * nel;
		}

		void move_cam() {
			pos.x = -10.f +static_cast<float>(rand()) / static_cast<float>(RAND_MAX/ (20.f));
			pos.y = -10.f +static_cast<float>(rand()) / static_cast<float>(RAND_MAX/ (20.f));
			nel = glm::inverse(glm::mat4{ right, up, w, pos });
			world_NDC = ortho * nel;
		}
	};
	//----------------------------------------CAMERA-----------------------------------

	//----------------------------------------ANIMATION--------------------------------
	//Have function that will take in a render_Object and change the uv to the uv in containers
	class Animation {
	private:
		const float playback;
		const float increment;
		const int frame_count;
		const vec2* arr;
	public:
		bool repeat{ false };
		//TODO make sure that ptr eventually points to memory in the heap
		Animation(const float pb, const int fc, const vec2* ptr) : playback{ pb }, increment{(float)fc / pb}, frame_count{fc}, arr{ptr} {}
	
		inline const float get_length() const { return playback; }
		inline const int get_frame_count() const { return frame_count; }
		inline const vec2* get_uv(int offset = 0) const { return arr + offset; }
		inline const float get_inc() const { return increment; }
	};

	class Animation_Manager {
		std::map<std::string, const Animation> animations;
	public:
		void load_anim(const std::string& animation_name, const vec2* data, const float anim_time, const int number_of_frames);
		const Animation* find_animation(const std::string& name) const { return animations.find(name) != animations.end() ? &(animations.find(name)->second) : nullptr; }
	};
	//----------------------------------------ANIMATION--------------------------------
	
	//------------------------------------------------RENDERING SPECIFIC------------------------------------------------
	//Renderer class will be incharge the vao, shader program and buffers.
	//Renderer class will not be exposed to the programmers and is meant-
	//-to have a level of disconnect even from render_object class

	//TODO change render class so that it handles adding of new objects and renderer
	//and that render objects only need to call the function.
	class Renderer {
	private:
		unsigned int vao;
		unsigned int shader_program;
		unsigned int vbo;
		unsigned int ibo;
		quad* quad_buff;
		std::vector<index> index_buff;
		unsigned int quad_buff_size;

		std::list<const render_Object*> active_objs;
		//TODO figure out parsing in shader file paths
		//std::string shader_file_path

	public:
		Camera cam;

		Renderer();
		~Renderer();

		inline unsigned int get_vao() { return vao; }
		inline unsigned int get_shader() { return shader_program; }
		inline unsigned int get_ibo() { return ibo; }
		inline size_t get_ao_size() { return active_objs.size(); }
		inline unsigned int get_vbo() { return vbo; }

		unsigned int create_render_object(const render_Object* obj);
		void remove_render_object(const render_Object* obj);
		void update_buff();
	};

	//The actual system that will get initialized into the engine
	class RenderSystem : public LB::ISystem {
	private:
		//TODO figure out some way in the serialization process
		//how to pass in object limit for rende
		Texture_Manager t_Manager;
		Animation_Manager a_Manager;
	public:
		Renderer object_renderer;

		RenderSystem();
		~RenderSystem();

		virtual void Update();
		virtual void Draw();
		void Initialize() override { SetSystemName("Renderer System"); }

		bool create_texture(const std::string& file_path, const std::string& name);
		bool remove_texture(const std::string& name);
		const int get_texture(const std::string& name) const { return t_Manager.get_texture_index(name); }
		void flush_textures();

		auto get_anim(const std::string& name) const { return a_Manager.find_animation(name); }
	};

	//A pointer to the system object in the core engine
	//made to be singleton
	extern RenderSystem* GRAPHICS;

	//Render object is an object that will be exposed to the programmers in the level creator
	class render_Object {
	private:
		unsigned int									quad_id;
		float											time_elapsed;
		unsigned int									frame;
		std::queue<std::pair<const Animation*, bool>>	animation;
	public:
		vec2						position;
		float						scal;
		float						w;
		float						h;
		vec3						col;
		std::array<vec2, 4>			uv; //bot left, bot right, top right, top left
		int							texture;
		bool						activated;

		render_Object(
			vec2 pos = { 0.f, 0.f }, 
			float width = 1.f, 
			float height = 1.f, 
			float scale = 1.f, 
			vec3 color = { 0.f,0.f,0.f }, 
			std::array<vec2,4> uv = {}, 
			int texture = -1, 
			bool active = true);
		~render_Object();
		
		inline const unsigned int get_index() const { return quad_id; }
		inline const size_t get_queue_size() const { return animation.size(); }

		inline bool operator==(const render_Object& rhs) const {
			return quad_id == rhs.quad_id;
		}

		void play_repeat(const std::string& name);
		void play_next(const std::string& name);
		void play_now(const std::string& name);

		void animate();
	};
	//------------------------------------------------RENDERING SPECIFIC------------------------------------------------

}