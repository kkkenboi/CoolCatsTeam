#pragma once
#include "LitterBox/Core/System.h"
#include <utility>
#include <array>

namespace Renderer {
	struct vec2 {
		float x, y;
	};

	struct vec3 {
		float x, y, z;
	};

	struct Vertex {
		vec2 pos{ 0.5f, 0.5f };
		vec2 tex{ 0.f,0.f };
		vec3 color{ 1.f,0.f,0.f };
		unsigned short index{ 0 };
		bool active{ false };
	};

	struct quad {
		std::array<Vertex, 4> data;
	};

	/*quad box1
	{
		vec2{-.8f,-.9f}, vec2{0.f, 1.f}, vec3{1.f, 0.f, 0.f}, true,
		vec2{-.2f,-.9f}, vec2{1.f, 1.f}, vec3{0.f, 1.f, 0.f}, true,
		vec2{-.8f,-1.f}, vec2{0.f, 0.f}, vec3{0.f, 0.f, 1.f}, true,
		vec2{-.2f,-1.f}, vec2{1.f, 0.f}, vec3{1.f, 0.f, 1.f}, true
	};

	quad box2{
		vec2{.2f,.9f}, vec2{1.f, 1.f}, vec3{0.f, 1.f, 0.f}, false,
		vec2{.8f,.9f}, vec2{0.f, 1.f}, vec3{1.f, 0.f, 0.f}, false,
		vec2{.8f,1.f}, vec2{0.f, 0.f}, vec3{0.f, 0.f, 1.f}, false,
		vec2{.2f,1.f}, vec2{1.f, 0.f}, vec3{1.f, 0.f, 1.f}, false
	};*/

	//NOTE: because index data type is unsigned short
	//the maximum number of vectors allowed is 65535
	//therefore maximum number of quads allowed is 16383
	struct index {
		std::array<unsigned short, 6> indexes;
	};

	class render_Object;

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

		//TODO figure out parsing in shader file paths
		//std::string shader_file_path

	public:
		std::vector<const render_Object*> active_objs;


		Renderer();
		~Renderer();

		inline unsigned int get_vao() { return vao; }
		inline unsigned int get_shader() { return shader_program; }
		inline unsigned int get_ibo() { return ibo; }
		inline unsigned int get_idx_size() { return index_buff.size(); }
		inline unsigned int get_vbo() { return vbo; }

		std::pair<quad*, std::vector<index>*> create_render_object();
		void update_buff();
	};

	class RenderSystem : public LitterBox::ISystem {
	private:
		//TODO figure out some way in the serialization process
		//how to pass in object limit for rende

	public:
		Renderer object_renderer;

		RenderSystem();
		~RenderSystem();

		virtual void Update(float dt);
		virtual void Draw();
		virtual std::string GetName() { return "Rendering System"; }
	};

	extern RenderSystem* GRAPHICS;

	//Render object is an object that will be exposed to the programmers in the level creator
	class render_Object {
	public:
		vec2			position;
		float			scal;
		float			w;
		float			h;
		vec3			col;
		int				quad_id;
		bool			activated;

		render_Object(vec2 pos = { 0.f, 0.f }, float width = 1.f, float height = 1.f, float scale = 1.f, vec3 color = { 0.f,0.f,0.f }, bool active = true);
		~render_Object();

	private:
		quad* object;
		std::vector<index>* idx_it;
		std::vector<index>::iterator it;
	};
}