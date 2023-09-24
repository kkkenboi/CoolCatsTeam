#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <array>
#include <string>
#include <sstream>
#include <filesystem>
#include "Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
	std::array<vec2, 4> uv,
	int text,
	bool active) :
	position{ pos }, scal{ scale }, w{ width }, h{ height },
	col{ color }, activated{ active }, quad_id{ UINT_MAX }, texture{ (int)text },
	uv{ uv }
{
	if (!GRAPHICS) {
		std::cerr << "GRAPHICS SYSTEM NOT INITIALIZED" << std::endl;
		return;
	}

	quad_id = GRAPHICS->object_renderer.create_render_object(this);
}

Renderer::render_Object::~render_Object()
{
	GRAPHICS->object_renderer.remove_render_object(this);
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
	index_buff.resize(quad_buff_size);

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
	//texture index
	glEnableVertexArrayAttrib(vao, 4);
	glVertexArrayVertexBuffer(vao, 4, vbo, sizeof(vec2) * 2U + sizeof(vec3), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 4, 1, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 4, 4);

	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, index_buff.capacity() * sizeof(index),
		nullptr, GL_DYNAMIC_STORAGE_BIT);
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

unsigned int Renderer::Renderer::create_render_object(const render_Object* obj)
{
	unsigned int i{ 0 };
	for (i; i < quad_buff_size; ++i) {
		if (quad_buff[i].data[0].active)
			continue;

		//set the index of the quad vertexes
		quad_buff[i].data[0].index = i;
		quad_buff[i].data[1].index = i;
		quad_buff[i].data[2].index = i;
		quad_buff[i].data[3].index = i;

		quad_buff[i].data[0].active = true;
		break;
	}

	if (i == quad_buff_size) {
		std::cerr << "OBJECT BUFFER FULL" << std::endl;

		//return quad_data full of garbage
		return i;
	}

	//cache width and height values
	float x_pos{ obj->w * 0.5f * obj->scal };
	float y_pos{ obj->h * 0.5f * obj->scal };

	//set position of quad
	quad_buff[i].data[0].pos = { obj->position.x - x_pos, obj->position.y - y_pos };//bottom left
	quad_buff[i].data[1].pos = { obj->position.x + x_pos, obj->position.y - y_pos };//bottom right
	quad_buff[i].data[2].pos = { obj->position.x + x_pos, obj->position.y + y_pos };//top right
	quad_buff[i].data[3].pos = { obj->position.x - x_pos, obj->position.y + y_pos };//top left

	quad_buff[i].data[0].index = obj->texture;
	quad_buff[i].data[1].index = obj->texture;
	quad_buff[i].data[2].index = obj->texture;
	quad_buff[i].data[3].index = obj->texture;

	quad_buff[i].data[0].tex = { 0.f, 0.f };
	quad_buff[i].data[1].tex = { 1.f, 0.f };
	quad_buff[i].data[2].tex = { 1.f, 1.f };
	quad_buff[i].data[3].tex = { 0.f, 1.f };

	active_objs.emplace_back(obj);

	return i;
}

void Renderer::Renderer::remove_render_object(const render_Object* obj)
{
	for (int i{ 0 }; i < 4; ++i) {
		//quad_buff[obj->get_index()].data[i].active = false;
		std::cout << obj->get_index() << '\n';
	}

	active_objs.remove_if([obj](const render_Object* in_list) { return *obj == *in_list; });
}

void Renderer::Renderer::update_buff()
{
	unsigned int i{ 0 };
	for (const render_Object*& e : active_objs) {
		unsigned int obj_index{ e->get_index() };
		//cache width and height values
		float x_pos{ e->w * 0.5f * e->scal };
		float y_pos{ e->h * 0.5f * e->scal };

		unsigned short idx{ unsigned short(e->get_index()) };

		index_buff.at(i) = index{ std::array<unsigned short, 6>{idx, (unsigned short)(idx + 1), (unsigned short)(idx + 2),
			(unsigned short)(idx + 2), (unsigned short)(idx + 3), idx} };

		//set position of quad
		quad_buff[obj_index].data[0].pos = { e->position.x - x_pos, e->position.y - y_pos };//bottom left
		quad_buff[obj_index].data[1].pos = { e->position.x + x_pos, e->position.y - y_pos };//bottom right
		quad_buff[obj_index].data[2].pos = { e->position.x + x_pos, e->position.y + y_pos };//top right
		quad_buff[obj_index].data[3].pos = { e->position.x - x_pos, e->position.y + y_pos };//top left

		for (size_t j{ 0 }; j < 4; ++j) {
			//set colour of quad
			quad_buff[obj_index].data[j].color = e->col;
			quad_buff[obj_index].data[j].tex = e->uv[j]; // 0 = bot left, 1 = bot right, 2 = top right, 3 = top left
		}
		++i;

		quad_buff[obj_index].data[0].texIndex = (float)e->texture;
		quad_buff[obj_index].data[1].texIndex = (float)e->texture;
		quad_buff[obj_index].data[2].texIndex = (float)e->texture;
		quad_buff[obj_index].data[3].texIndex = (float)e->texture;
	}

	glNamedBufferSubData(vbo, 0, sizeof(quad) * quad_buff_size, quad_buff);
	glNamedBufferSubData(ibo, 0, sizeof(index) * active_objs.size(), index_buff.data());
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << (int)err << std::endl;

}
//----------------------------------------------RENDERER---------------------------------------------------

//----------------------------------------------RENDERER-SYSTEM-------------------------------------------
//Global pointer to render system for accessing of
//render objects
Renderer::RenderSystem* Renderer::GRAPHICS = nullptr;

Renderer::render_Object* testobj;
Renderer::Texture* again;

Renderer::RenderSystem::RenderSystem()
{
	//singleton that shiet
	if (!GRAPHICS)
		GRAPHICS = this;
	else
		std::cerr << "Render System already exist" << std::endl;

	glUseProgram(object_renderer.get_shader());
	glBindVertexArray(object_renderer.get_vao());

	testobj = new render_Object;
	testobj->col = { 0.f,0.f,0.f };
	testobj->uv[0] = { 0.f, 0.6f };
	testobj->uv[1] = { .12f, 0.6f };
	testobj->uv[2] = { .12f, .73f };
	testobj->uv[3] = { 0.f, .73f };

	t_Manager.add_texture("./test.png", "test");
	t_Manager.add_texture("./test2.png", "logo");
	t_Manager.add_texture("./test3.png", "pine");
	t_Manager.add_texture("./anim.png", "run");
	testobj->texture = t_Manager.get_texture_index("run");
	/*glBindTextureUnit(0, t_Manager.get_texture("test"));
	glTextureParameteri(t_Manager.get_texture("test"), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(t_Manager.get_texture("test"), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*GLuint tex_loc = glGetUniformLocation(object_renderer.get_shader(), "u_SamplerID");
	glUniform1i(tex_loc, 0);*/
}

Renderer::RenderSystem::~RenderSystem()
{
	if (GRAPHICS)
		GRAPHICS = nullptr;
}

std::array<std::array<Renderer::vec2, 4>, 18> frames{
	Renderer::vec2{0.f, 0.6f}, Renderer::vec2{.12f, .6f}, Renderer::vec2{.12f, .72f}, Renderer::vec2{0.f, .72f},
		Renderer::vec2{.12f, 0.6f}, Renderer::vec2{.23f, .6f}, Renderer::vec2{.23f, .72f}, Renderer::vec2{.12f, .72f},
		Renderer::vec2{0.23f, 0.6f}, Renderer::vec2{.345f, .6f}, Renderer::vec2{.345f, .72f}, Renderer::vec2{0.23f, .72f},
		Renderer::vec2{0.345f, 0.6f}, Renderer::vec2{.45f, .6f}, Renderer::vec2{.45f, .72f}, Renderer::vec2{0.345f, .72f},
		Renderer::vec2{0.45f, 0.6f}, Renderer::vec2{.55f, .6f}, Renderer::vec2{.55f, .72f}, Renderer::vec2{0.45f, .72f},
		Renderer::vec2{0.55f, 0.6f}, Renderer::vec2{.65f, .6f}, Renderer::vec2{.65f, .72f}, Renderer::vec2{0.55f, .72f},
		Renderer::vec2{0.65f, 0.6f}, Renderer::vec2{.755f, .6f}, Renderer::vec2{.755f, .72f}, Renderer::vec2{0.65f, .72f},
		Renderer::vec2{0.755f, 0.6f}, Renderer::vec2{.85f, .6f}, Renderer::vec2{.85f, .72f}, Renderer::vec2{0.755f, .72f},
		Renderer::vec2{0.85f, 0.6f}, Renderer::vec2{.94f, .6f}, Renderer::vec2{.94f, .72f}, Renderer::vec2{0.85f, .72f},

		Renderer::vec2{0.f, .458f}, Renderer::vec2{.09f, .458f}, Renderer::vec2{.09f, .59f}, Renderer::vec2{0.f, .59f},
		Renderer::vec2{.09f, .458f}, Renderer::vec2{.19f, .458f}, Renderer::vec2{.19f, .59f}, Renderer::vec2{.09f, .59f},
		Renderer::vec2{0.19f, .458f}, Renderer::vec2{.29f, .458f}, Renderer::vec2{.29f, .59f}, Renderer::vec2{0.19f, .59f},
		Renderer::vec2{0.29f, .458f}, Renderer::vec2{.39f, .458f}, Renderer::vec2{.39f, .59f}, Renderer::vec2{0.29f, .59f},
		Renderer::vec2{0.39f, .458f}, Renderer::vec2{.51f, .458f}, Renderer::vec2{.51f, .59f}, Renderer::vec2{0.39f, .59f},
		Renderer::vec2{0.51f, .458f}, Renderer::vec2{.6f, .458f}, Renderer::vec2{.6f, .59f}, Renderer::vec2{0.51f, .59f},
		Renderer::vec2{0.6f, .458f}, Renderer::vec2{.7f, .458f}, Renderer::vec2{.7f, .59f}, Renderer::vec2{0.6f, .59f},
		Renderer::vec2{0.7f, .458f}, Renderer::vec2{.8f, .458f}, Renderer::vec2{.8f, .59f}, Renderer::vec2{0.7f, .59f},
		Renderer::vec2{0.8f, .458f}, Renderer::vec2{.94f, .458f}, Renderer::vec2{.94f, .59f}, Renderer::vec2{0.8f, .59f}
};

void Renderer::RenderSystem::Update()
{
	static int frame_p_sec{ 0 };
	static int currFrame{ 0 };
	if (frame_p_sec >= 6) {
		testobj->uv = frames[currFrame];
		frame_p_sec = 0;

		currFrame = currFrame >= 17 ? 0 : ++currFrame;
	}
	else {
		++frame_p_sec;
	}
	object_renderer.update_buff();
	glClearColor(.3f, 0.5f, .8f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, (GLsizei)(object_renderer.get_ao_size() * 6), GL_UNSIGNED_SHORT, NULL);
}

void Renderer::RenderSystem::Draw()
{
	//TODO HAVE RENDERER PERFORM THE SWAP BUFFER INSTEAD OF WINDOW
	// Draw stuff
}
bool Renderer::RenderSystem::create_texture(const std::string& file_path, const std::string& name)
{
	return t_Manager.add_texture(file_path, name);
}
bool Renderer::RenderSystem::remove_texture(const std::string& name)
{
	return t_Manager.remove_texture(name);
}
void Renderer::RenderSystem::flush_textures()
{
	t_Manager.flush_textures();
}
//----------------------------------------------RENDERER-SYSTEM-------------------------------------------



Renderer::Texture::Texture(const std::string& path) :
	id{ 0 }, file_path{ path }, local_buff{ nullptr },
	w{ 0 }, h{ 0 }, fluff{ 0 }
{
	stbi_set_flip_vertically_on_load(1);

	local_buff = stbi_load(path.c_str(), &w, &h, &fluff, 4);
	if (!local_buff) {
		std::cerr << "Texture file path: " << path << " NOT FOUND!" << std::endl;
		return;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, GL_RGBA8, w, h);
	glTextureSubImage2D(id, 0, 0, 0, w, h,
		GL_RGBA, GL_UNSIGNED_BYTE, local_buff);

	stbi_image_free(local_buff);


	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	std::cout << "Picture specs: " << id << " " << w << " " << h << " " << fluff << std::endl;
}

Renderer::Texture::~Texture()
{
	std::cout << "DELETED TEXTURE " << id << std::endl;
	glDeleteTextures(1, &id);
}

bool Renderer::Texture_Manager::add_texture(const std::string& file_path, const std::string& name)
{
	if (textures.size() >= 32) {
		std::cerr << "Maximum textures reached" << std::endl;
		return false;
	}

	//Loop through to get the first free unit slot available
	int i{ 0 };
	for (; i < 32; ++i) {
		if (!free[i])
			break;
	}

	textures.emplace(std::make_pair(name, std::make_pair(new Texture{ file_path }, i)));
	//getting the pair then the texture id
	glBindTextureUnit(0 + i, textures.find(name)->second.first->get_tex());
	free[i] = true;

	GLint uni_loc = glGetUniformLocation(GRAPHICS->object_renderer.get_shader(), "u_SamplerID");
	int test[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	glUniform1iv(uni_loc, 8, test);


	std::cout << "Texture index: " << i << std::endl;

	return true;
}

bool Renderer::Texture_Manager::remove_texture(const std::string& name)
{
	//unbind texture at that position
	free[textures.find(name)->second.second] = false;
	delete textures.find(name)->second.first;
	return textures.erase(name);
}

void Renderer::Texture_Manager::flush_textures()
{
	for (auto& e : textures) {
		delete e.second.first;
	}
	textures.clear();
	for (int i{ 0 }; i < 32; ++i)
		free[i] = false;
}