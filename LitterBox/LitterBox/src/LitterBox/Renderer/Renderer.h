#pragma once
#include "LitterBox/Core/System.h"
#include <utility>
#include <array>
#include <list>

namespace Renderer {
	//-------------Structs to make things easier for now--------
	struct vec2 {
		float x, y;
	};

	struct vec3 {
		float x, y, z;
	};
	//----------------------------------------------------------

	//Vertex data
	struct Vertex {
		vec2 pos{ 0.5f, 0.5f };
		vec2 tex{ 0.f,0.f };
		vec3 color{ 1.f,0.f,0.f };
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

	class render_Object; //Forward declerator

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

	public:
		Renderer object_renderer;

		RenderSystem();
		~RenderSystem();

		virtual void Update(float dt);
		virtual void Draw();
		virtual std::string GetName() { return "Rendering System"; }
	};

	//A pointer to the system object in the core engine
	//made to be singleton
	extern RenderSystem* GRAPHICS;

	//Render object is an object that will be exposed to the programmers in the level creator
	class render_Object {
	private:
		unsigned int quad_id;
	public:
		vec2			position;
		float			scal;
		float			w;
		float			h;
		vec3			col;
		bool			activated;

		render_Object(vec2 pos = { 0.f, 0.f }, float width = 1.f, float height = 1.f, float scale = 1.f, vec3 color = { 0.f,0.f,0.f }, bool active = true);
		~render_Object();
		
		inline const unsigned int get_index() const { return quad_id; }

		inline bool operator==(const render_Object& rhs) const {
			return quad_id == rhs.quad_id;
		}
	};
}