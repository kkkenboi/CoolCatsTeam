#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <array>
#include <string>
#include <sstream>
#include <filesystem>
#include "Renderer.h"



struct shader_source {
	std::string vtx_shd;
	std::string frg_shd;
};

shader_source shader_parser(const char* shader_file_name) {
	std::ifstream ifs{ shader_file_name };

	enum class shader_type {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	if (!ifs.is_open()) {
		printf("COULD NOT OPEN FILE %s\n", shader_file_name);
		exit(EXIT_FAILURE);
	}

	std::string line{};
	std::stringstream ss[2];
	shader_type type{ shader_type::NONE };
	while (getline(ifs, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = shader_type::VERTEX;//vertex shader
			else if (line.find("fragment") != std::string::npos)
				type = shader_type::FRAGMENT;//fragment shader
		}
		else if (type != shader_type::NONE) {
			ss[(int)type] << line << '\n';
		}
	}

	return shader_source{ ss[0].str(), ss[1].str() };
}

unsigned int compile_shader(const char*& source, unsigned int type) {
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		printf("Failed to compile shader: %s\n", message);
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int create_shader(const char* vertex_shader, const char* fragment_shader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compile_shader(vertex_shader, GL_VERTEX_SHADER);
	unsigned int fs = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

Renderer::render_Object::render_Object(
	vec2 pos, 
	float width, 
	float height, 
	float scale, 
	vec3 color, 
	bool active) :
	position{ pos }, scal{ scale }, w{ width }, h{ height }, 
	col{ color }, activated{ active }, object{ nullptr }, idx_it{},
	quad_id{}
{
	std::pair<quad*, std::vector<index>*> obj = GRAPHICS->object_renderer.create_render_object();

	if (!obj.first || !obj.second) {
		std::cerr << "Render object not found" << std::endl;
		return;
	}

	object = obj.first;
	idx_it = obj.second;

	quad_id = obj.first->data[0].index;

	for (size_t j{ 0 }; j < 4; ++j) {
		//activate quad for rendering
		obj.first->data[j].active = true;
		//set colour of quad
		obj.first->data[j].color = col;
	}
	//TODO Check that index moves over 4 vertexs with each increment
	//i.e. index i = 1 is first quad with 4 vtxs and i = 2 is second quad of 4 vtxs

	//cache width and height values
	float x_pos{ w * 0.5f * scal };
	float y_pos{ h * 0.5f * scal };

	//set position of quad
	obj.first->data[0].pos = { position.x - x_pos, position.y - y_pos };//bottom left
	obj.first->data[1].pos = { position.x + x_pos, position.y - y_pos };//bottom right
	obj.first->data[2].pos = { position.x + x_pos, position.y + y_pos };//top right
	obj.first->data[3].pos = { position.x - x_pos, position.y + y_pos };//top left

	//index array
	//TODO INDEX BUFFER NEEDS INDEX * 4 BUT BUFFER SUB DATA NEEDS INDEX
	unsigned short i{ (unsigned short)(obj.first->data[0].index * 4) };
	idx_it->emplace_back(std::array<unsigned short, 6>{i, (unsigned short)(i + 1), (unsigned short)(i + 2),
		(unsigned short)(i + 2), (unsigned short)(i + 3), i});

	//get iterator to the indexes for removal
	it = idx_it->end() - 1;
	
	//TODO make it modular ONLY TESTING FOR 1 BOX RN
	glNamedBufferSubData(GRAPHICS->object_renderer.get_vbo(), obj.first->data[0].index * sizeof(quad), sizeof(quad), &obj.first[0]);
	glNamedBufferSubData(GRAPHICS->object_renderer.get_ibo(), obj.first->data[0].index * sizeof(index), sizeof(index), &idx_it->back());
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << (int)err << std::endl;

	GRAPHICS->object_renderer.active_objs.emplace_back(this);
}

Renderer::render_Object::~render_Object()
{
}

//----------------------------------------------RENDERER---------------------------------------------------
Renderer::Renderer::Renderer() :
	vao{}, shader_program{}, vbo{}, ibo{},
	quad_buff{ nullptr }, index_buff{},
	quad_buff_size{}, active_objs{}
{
	//create vertex
	quad_buff_size = 3000;
	quad_buff = new quad[quad_buff_size];
	index_buff.reserve(quad_buff_size);
	active_objs.reserve(quad_buff_size);

	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo,
		sizeof(quad) * quad_buff_size,
		quad_buff,
		GL_DYNAMIC_STORAGE_BIT);

	//create vao
	glCreateVertexArrays(1, &vao);
	//attribute 0 will be position data
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayVertexBuffer(vao, 1, vbo, 0, sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 0, 1);
	//texture coordinates
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayVertexBuffer(vao, 2, vbo, sizeof(vec2), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 1, 2);
	//color coordinates
	glEnableVertexArrayAttrib(vao, 2);
	glVertexArrayVertexBuffer(vao, 3, vbo, sizeof(vec2) * 2U, sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 2, 3);

	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, index_buff.capacity() * sizeof(index),
		index_buff.data(), GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao, ibo);

	std::filesystem::path a = std::filesystem::current_path();

	//TODO Load shaders
	shader_source shd_pgm{ shader_parser("./Basic.shader") };
	shader_program = create_shader(shd_pgm.vtx_shd.c_str(), shd_pgm.frg_shd.c_str());
	//TODO Load textures

	//TODO make it modular so multiple objects can be made from this model
}

Renderer::Renderer::~Renderer()
{
	//cleanup on server side
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shader_program);
}

std::pair<Renderer::quad*, std::vector<Renderer::index>*> Renderer::Renderer::create_render_object()
{
	for (int i{ 0 }; i < quad_buff_size; ++i) {
		if (quad_buff[i].data[0].active)
			continue;

		//set the index of the quad vertexes
		quad_buff[i].data[0].index = i;
		quad_buff[i].data[1].index = i;
		quad_buff[i].data[2].index = i;
		quad_buff[i].data[3].index = i;

		std::pair<quad*, std::vector<index>*> ret{ quad_buff + i, &index_buff };

		return ret;
	}
	std::cerr << "OBJECT BUFFER FULL" << std::endl;
	return std::pair<quad*, std::vector<index>*>(nullptr, nullptr);
}

void Renderer::Renderer::update_buff()
{
	for(const render_Object*& e : active_objs) {
		//cache width and height values
		float x_pos{ e->w * 0.5f * e->scal };
		float y_pos{ e->h * 0.5f * e->scal };

		//set position of quad
		quad_buff[e->quad_id].data[0].pos = { e->position.x - x_pos, e->position.y - y_pos };//bottom left
		quad_buff[e->quad_id].data[1].pos = { e->position.x + x_pos, e->position.y - y_pos };//bottom right
		quad_buff[e->quad_id].data[2].pos = { e->position.x + x_pos, e->position.y + y_pos };//top right
		quad_buff[e->quad_id].data[3].pos = { e->position.x - x_pos, e->position.y + y_pos };//top left

		for (size_t j{ 0 }; j < 4; ++j) {
			//set colour of quad
			quad_buff[e->quad_id].data[j].color = e->col;
		}
	}

	glNamedBufferSubData(vbo, 0, sizeof(quad) * quad_buff_size, quad_buff);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << (int)err << std::endl;
	
}
//----------------------------------------------RENDERER---------------------------------------------------

//----------------------------------------------RENDERER-SYSTEM-------------------------------------------
//Global pointer to render system for accessing of
//render objects
Renderer::RenderSystem* Renderer::GRAPHICS = nullptr;

Renderer::RenderSystem::RenderSystem()
{
	//singleton that shiet
	if (!GRAPHICS)
		GRAPHICS = this;
	else
		std::cerr << "Render System already exist" << std::endl;

	render_Object test{ {0.f,0.f}, 1.f, 1.f, 1.f, {1.f,0.f,0.f}, true};
	render_Object test2{ {0.5f,0.5f}, .4f, .4f, 1.f, {0.f,1.f,0.f}, true};
	render_Object test3{ {-0.5f,-0.5f}, .6f, .6f, .5f, {0.f,1.f,0.f}, true};

	glUseProgram(object_renderer.get_shader());
	glBindVertexArray(object_renderer.get_vao());
}

Renderer::RenderSystem::~RenderSystem()
{
	if (GRAPHICS)
		GRAPHICS = nullptr;
}

void Renderer::RenderSystem::Update(float dt)
{
	object_renderer.update_buff();
	glClearColor(.3f, 0.5f, .8f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, object_renderer.get_idx_size() * 6, GL_UNSIGNED_SHORT, NULL);
}

void Renderer::RenderSystem::Draw()
{
	//TODO HAVE RENDERER PERFORM THE SWAP BUFFER INSTEAD OF WINDOW
	// Draw stuff
}
//----------------------------------------------RENDERER-SYSTEM-------------------------------------------