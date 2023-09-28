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
#include "LitterBox/Factory/Components.h"

#include "LitterBox/Engine/Time.h"

//-----------------------------------------HELPER FUNCTIONS--------------------------------
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
//-----------------------------------------HELPER FUNCTIONS--------------------------------

//---------------------------------------ANIMATIONS-------------------------------------

//TODO for array of UV data for serialization probably gonna need to store data on heap

void Renderer::Animation_Manager::load_anim(const std::string& animation_name, const std::array<LB::Vec2<float>,4>* data, const float anim_time, const int number_of_frames) {
	animations.emplace(std::make_pair(animation_name, Animation{ anim_time, number_of_frames, data }));
}
//---------------------------------------ANIMATIONS-------------------------------------

//------------------------------------------RENDERER-OBJECT---------------------------------------------
LB::CPRender::CPRender(
	Vec2<float>	 pos,
	float width,
	float height,
	Vec2<float> scale,
	Vec3<float>	 color,
	std::array<Vec2<float>, 4> uv,
	int texture,
	bool active,
	Renderer::Renderer_Types rend_type) :
	renderer_id{ rend_type }, position{ pos }, scal{ scale }, w{ width }, h{ height },
	col{ color }, activated{ active }, quad_id{ UINT_MAX }, texture{ texture },
	uv{ uv }, frame{ 0 }, time_elapsed{ 0.f }, rotation{ 0.f }, transform{nullptr}
{
	if (!Renderer::GRAPHICS) {
		std::cerr << "GRAPHICS SYSTEM NOT INITIALIZED" << std::endl;
		return;
	}


	quad_id = Renderer::GRAPHICS->create_object(renderer_id, this);
}

LB::CPRender::~CPRender()
{
	Renderer::GRAPHICS->remove_object(renderer_id, this);
}

//########################ANIMATION##############################
void LB::CPRender::play_repeat(const std::string& name)
{
	const Renderer::Animation* anim{ Renderer::GRAPHICS->get_anim(name) };
	if (anim) {
		animation.push(std::make_pair(anim, true));
	}
	else {
		std::cerr << "There is no animation: " << name << " loaded." << std::endl;
	}
}

void LB::CPRender::play_next(const std::string& name)
{
	const Renderer::Animation* anim{ Renderer::GRAPHICS->get_anim(name) };
	if (anim) {
		animation.push(std::make_pair(anim, false));
	}
	else {
		std::cerr << "There is no animation: " << name << " loaded." << std::endl;
	}
}

void LB::CPRender::play_now(const std::string& name)
{
	const Renderer::Animation* anim{ Renderer::GRAPHICS->get_anim(name) };
	if (anim) {
		while (animation.size() != 0) {
			animation.pop();
		}
		animation.push(std::make_pair(anim, false));
	}
	else {
		std::cerr << "There is no animation: " << name << " loaded." << std::endl;
	}
}

void LB::CPRender::stop_anim()
{
	while (animation.size())
		animation.pop();
}

void LB::CPRender::animate()
{
	//increment time elapsed
	time_elapsed += (float)LB::TIME->GetDeltaTime();

	//move to next frame based on time
	if (time_elapsed >= animation.front().first->get_inc()) {
		++frame;
		time_elapsed = 0.f;
	}

	//check if on last frame
	if (frame == (unsigned)animation.front().first->get_frame_count()) {
		//pop the animation if its non repeat or reset the frame if it is
		if (!animation.front().second) {
			animation.pop();
		}
		frame = 0;
	}

	//check if we still have animation
	if (!animation.size())
		return;

	uv = *animation.front().first->get_uv(frame);
}
//########################ANIMATION##############################
//------------------------------------------RENDERER-OBJECT---------------------------------------------

//----------------------------------------------RENDERER---------------------------------------------------
Renderer::Renderer::Renderer(const Renderer_Types& renderer) :
	vao{}, vbo{}, ibo{},
	quad_buff{ nullptr }, index_buff{},
	quad_buff_size{}, active_objs{}
{
	//create vertex
	//quad_buff_size = 3000;
	//TODO have a proper reasoning for limits to buff size
	switch (renderer) {
	case Renderer_Types::RT_OBJECT:
		quad_buff_size = 3000;
		break;
	case Renderer_Types::RT_BACKGROUND:
		quad_buff_size = 10;
		break;
	case Renderer_Types::RT_DEBUG:
		quad_buff_size = 200;
		break;
	case Renderer_Types::RT_UI:
		quad_buff_size = 50;
		break;
	}
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
}

Renderer::Renderer::~Renderer()
{
	//cleanup on server side
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}

unsigned int Renderer::Renderer::create_render_object(const LB::CPRender* obj)
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

	//set position of quad
	quad_buff[i].data[0].pos = { -0.5f, -0.5f };//bottom left
	quad_buff[i].data[1].pos = { 0.5f, -0.5f  };//bottom right
	quad_buff[i].data[2].pos = { 0.5f, 0.5f   };//top right
	quad_buff[i].data[3].pos = { -0.5f, 0.5f  };//top left

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

void Renderer::Renderer::remove_render_object(const LB::CPRender* obj)
{
	for (int i{ 0 }; i < 4; ++i) {
		quad_buff[obj->get_index()].data[i].active = false;
	}

	active_objs.remove_if([obj](const LB::CPRender* in_list) { return *obj == *in_list; });
}

void Renderer::Renderer::update_buff(Renderer_Types r_type)
{
	GLint uni_loc = glGetUniformLocation(GRAPHICS->get_shader(), "z_val");
	if (uni_loc == -1) {
		std::cerr << "Uniform location does not exist" << std::endl;
	}
	switch (r_type) {
	case Renderer_Types::RT_OBJECT:
		glUniform1f(uni_loc, 0.f);
		break;
	case Renderer_Types::RT_BACKGROUND:
		glUniform1f(uni_loc, 0.0f);
		break;
	}

	static glm::vec4 mdl_pts[4]{
		{-0.5f, -0.5f, 0.f, 1.f}, //bottom left
		{0.5f, -0.5f, 0.f, 1.f}, //bottom right
		{0.5f, 0.5f, 0.f, 1.f}, //top right
		{-0.5f, 0.5f, 0.f, 1.f} //top left
	};

	static glm::mat4 scale{
		1.f,0.f,0.f,0.f,
		0.f,1.f,0.f,0.f,
		0.f,0.f,1.f,0.f,
		0.f,0.f,0.f,1.f
	};
	static glm::mat4 rot{
		1.f,0.f,0.f,0.f,
		0.f,1.f,0.f,0.f,
		0.f,0.f,1.f,0.f,
		0.f,0.f,0.f,1.f
	};
	static glm::mat4 trans{
		1.f,0.f,0.f,0.f,
		0.f,1.f,0.f,0.f,
		0.f,0.f,1.f,0.f,
		0.f,0.f,0.f,1.f
	};

	for (const LB::CPRender*& e : active_objs) {
		unsigned int obj_index{ e->get_index() };
		//create index in index buffer
		unsigned short idx{ unsigned short(e->get_index() * 4) };

		index_buff.at(obj_index) = index{ std::array<unsigned short, 6>{idx, (unsigned short)(idx + 1), (unsigned short)(idx + 2),
			(unsigned short)(idx + 2), (unsigned short)(idx + 3), idx} };

		if (e->get_queue_size()) {
			const_cast<LB::CPRender*>(e)->animate();
		}

		const_cast<LB::CPRender*>(e)->get_transform_data();

		//Set appropriate model to world matrices
		scale[0][0] = e->scal.x * e->w;
		scale[1][1] = e->scal.y * e->h;

		rot[0][0] = cosf(e->rotation);
		rot[0][1] = sinf(e->rotation);
		rot[1][0] = -sinf(e->rotation);
		rot[1][1] = cosf(e->rotation);

		trans[3][0] = e->position.x;
		trans[3][1] = e->position.y;

		//set position based off camera mat
		//edit color and uv coordinates and texture
		for (int i{ 0 }; i < 4; ++i) {
			glm::vec4 pos{ trans * rot * scale * mdl_pts[i] };

			pos = cam.world_NDC * pos;
			quad_buff[obj_index].data[i].pos.x = pos.x;
			quad_buff[obj_index].data[i].pos.y = pos.y;

			quad_buff[obj_index].data[i].color.x = e->col.x;
			quad_buff[obj_index].data[i].color.y = e->col.y;
			quad_buff[obj_index].data[i].color.z = e->col.z;
			quad_buff[obj_index].data[i].tex.x = e->uv[i].x; // 0 = bot left, 1 = bot right, 2 = top right, 3 = top left
			quad_buff[obj_index].data[i].tex.y = e->uv[i].y; // 0 = bot left, 1 = bot right, 2 = top right, 3 = top left
			quad_buff[obj_index].data[i].texIndex = (float)e->texture;
		}
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

LB::CPRender* test2;

Renderer::RenderSystem::RenderSystem() :
	object_renderer{Renderer_Types::RT_OBJECT},
	bg_renderer{Renderer_Types::RT_BACKGROUND},
	debug_renderer{Renderer_Types::RT_DEBUG}
{
	//singleton that shiet
	if (!GRAPHICS)
		GRAPHICS = this;
	else
		std::cerr << "Render System already exist" << std::endl;

	shader_source shd_pgm{ shader_parser("../Assets/Shaders/Basic.shader") };
	shader_program = create_shader(shd_pgm.vtx_shd.c_str(), shd_pgm.frg_shd.c_str());

	glUseProgram(shader_program);
	glBindVertexArray(object_renderer.get_vao());

	//-################FOR BACKGROUND##########################
	//cache some values
	float midx = (float)LB::WINDOWSSYSTEM->GetWidth() * 0.5f;
	float midy = (float)LB::WINDOWSSYSTEM->GetHeight() * 0.5f;
	float w = (float)LB::WINDOWSSYSTEM->GetWidth();
	float h = (float)LB::WINDOWSSYSTEM->GetHeight();
	
	test2 = new LB::CPRender{ {midx,midy}, w, h, {1.f,1.f}, {0.f,0.f,0.f}, {}, -1, true, Renderer_Types::RT_BACKGROUND };

	t_Manager.add_texture("../Assets/Textures/test3.png", "pine");
	t_Manager.add_texture("../Assets/Textures/Environment_Background.png", "bg");

	test2->texture = LB::ASSETMANAGER->GetTextureIndex("bg");
	test2->uv[0].x = 0.f;
	test2->uv[0].y = 0.f;
	test2->uv[1].x = 1.f;
	test2->uv[1].y = 0.f;
	test2->uv[2].x = 1.f;
	test2->uv[2].y = 1.f;
	test2->uv[3].x = 0.f;
	test2->uv[3].y = 1.f;
	//-################FOR BACKGROUND##########################
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::RenderSystem::~RenderSystem()
{
	if (GRAPHICS)
		GRAPHICS = nullptr;

	glDeleteProgram(shader_program);
}

void Renderer::RenderSystem::Update()
{
	bg_renderer.update_buff(Renderer_Types::RT_BACKGROUND);
	object_renderer.update_buff(Renderer_Types::RT_OBJECT);
	glClearColor(.3f, 0.5f, .8f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(bg_renderer.get_vao());
	glDrawElements(GL_TRIANGLES, (GLsizei)(bg_renderer.get_ao_size() * 6), GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(object_renderer.get_vao());
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
unsigned int Renderer::RenderSystem::create_object(Renderer_Types r_type, const LB::CPRender* obj)
{
	switch (r_type) {
	case Renderer_Types::RT_OBJECT:
		return object_renderer.create_render_object(obj);
	case Renderer_Types::RT_BACKGROUND:
		return bg_renderer.create_render_object(obj);
	case Renderer_Types::RT_DEBUG:
		return debug_renderer.create_render_object(obj);
	//TODO for UI and DEBUG
	}
}
void Renderer::RenderSystem::remove_object(Renderer_Types r_type, const LB::CPRender* obj)
{
	switch (r_type) {
	case Renderer_Types::RT_OBJECT:
		object_renderer.remove_render_object(obj);
		break;
	case Renderer_Types::RT_BACKGROUND:
		bg_renderer.remove_render_object(obj);
		break;
	case Renderer_Types::RT_DEBUG:
		debug_renderer.remove_render_object(obj);
		break;
	}
}
//----------------------------------------------RENDERER-SYSTEM-------------------------------------------


//----------------------------------------------TEXTURES--------------------------------------------
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

	//std::cout << "Picture specs: " << id << " " << w << " " << h << " " << fluff << std::endl;
}

Renderer::Texture::~Texture()
{
	//std::cout << "DELETED TEXTURE " << id << std::endl;
	glDeleteTextures(1, &id);
}

bool Renderer::Texture_Manager::add_texture(const std::string& file_path, const std::string& name)
{
	LB::ASSETMANAGER->AddTexture(file_path, name);

	GLint uni_loc = glGetUniformLocation(GRAPHICS->get_shader(), "u_SamplerID");
	int test[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	glUniform1iv(uni_loc, 13, test);

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
//----------------------------------------------TEXTURES--------------------------------------------