/*!************************************************************************
 \file                Renderer.cpp
 \author(s)           Ryan Tan Jian Hao | Amadeus Chia Jinhan
 \par DP email(s):    ryanjianhao.tan\@digipen.edu | amadeusjinhan.chia\@digipen.edu
 \par Course:         CSD2401A
 \date                29-09-2023
 \brief

 Header file that contains helper function definitions for Rendering as
 well as the member functions of all the relevant classes.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <array>
#include <string>
#include <sstream>
#include <filesystem>
#include "Renderer.h"
#include "LitterBox/Factory/Components.h"

#include "LitterBox/Engine/Time.h"

//-----------------------------------------HELPER FUNCTIONS--------------------------------
/*!***********************************************************************
\brief
 shader_parse opens a file located at shader_file_name, copies and
 stores the text into a shader_source struct.

 NOTE: Shader file has both vertex and fragment shader delimited by
 #VERTEX #FRAGMENT respectively

\param shader_file_name
 The location of the shader file to be compiled.

\return
 shader_source object with text data of the vertex shader and fragment
 shader
*************************************************************************/
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
/*!***********************************************************************
\brief
 compile_shader takes the text data of a specified shader type and compiles
 the shader in the GPU.

\param source
 Shader text data in C style string

\param type
 The type of shader whether fragment or shader

\return
 The handle to the compiled shader in GPU
*************************************************************************/
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

/*!***********************************************************************
\brief
 create_shader creates the shader program in the GPU and returns the
 handle to it.

\param vertex_shader
 vertex Shader text data in C style string

\param fragment_shader
 vertex Shader text data in C style string

\return
 The handle to the shader program in GPU
*************************************************************************/
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

/*!***********************************************************************
\brief
 init_anim stores a pointer to an array of UV coordinates along with
 the total animation playback time and number of UV coordinates in the
 array.

\param animation_name
 The key assigned to the animation object

\param data
 Pointer to the UV coordinate array that acts as the animation

\param anim_time
 The playback time of the animation in terms of seconds

\param number_of_frames
 The number of UV coordinates quad stored in the array
*************************************************************************/
void Renderer::Animation_Manager::load_anim(const std::string& animation_name, const std::array<LB::Vec2<float>,4>* data, const float anim_time, const int number_of_frames) {
	animations.emplace(std::make_pair(animation_name, Animation{ anim_time, number_of_frames, data }));
}
//---------------------------------------ANIMATIONS-------------------------------------

//------------------------------------------RENDERER-OBJECT---------------------------------------------
/*!***********************************************************************
\brief
 constructor of CPRender object
*************************************************************************/
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
/*!***********************************************************************
\brief
 Destructor of CPRender object
*************************************************************************/
LB::CPRender::~CPRender()
{
	Renderer::GRAPHICS->remove_object(renderer_id, this);
}

//########################ANIMATION##############################

/*!***********************************************************************
\brief
 Loads the animation into the queue with a flag that tells the animation
 function to play that animation on repeat.
*************************************************************************/
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

/*!***********************************************************************
\brief
 Loads an animation into the queue and makes it wait till queue pops all
 the other animations infront of it.
*************************************************************************/
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

/*!***********************************************************************
\brief
 Pops every animtion currently in the queue before pushing the given animation
 into the queue.
*************************************************************************/
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

/*!***********************************************************************
\brief
 Pops of all animation currently in the animation queue.
*************************************************************************/
void LB::CPRender::stop_anim()
{
	while (animation.size())
		animation.pop();
}

/*!***********************************************************************
\brief
 Function that is incharge of play the animation in the front of the queue
 based on time and not frames.
*************************************************************************/
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
/*!***********************************************************************
\brief
 Renderer constructor that sets up the VBO and VAO

\param renderer
 The type of renderer you want this renderer to be.
*************************************************************************/
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
	glVertexArrayVertexBuffer(vao, 2, vbo, sizeof(LB::Vec2<float>), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 1, 2);
	//color coordinates
	glEnableVertexArrayAttrib(vao, 2);
	glVertexArrayVertexBuffer(vao, 3, vbo, sizeof(LB::Vec2<float>) * 2U, sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 2, 3);
	//texture index
	glEnableVertexArrayAttrib(vao, 3);
	glVertexArrayVertexBuffer(vao, 4, vbo, sizeof(LB::Vec2<float>) * 2U + sizeof(LB::Vec3<float>), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 3, 1, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 3, 4);
	//width height data
	glEnableVertexArrayAttrib(vao, 4);
	glVertexArrayVertexBuffer(vao, 5, vbo, sizeof(LB::Vec2<float>) * 2U + sizeof(LB::Vec3<float>) + sizeof(float), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 4, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 4, 5);
	//scaling and rotation data
	glEnableVertexArrayAttrib(vao, 5);
	glVertexArrayVertexBuffer(vao, 6, vbo, sizeof(LB::Vec2<float>) * 2U + sizeof(LB::Vec3<float>) * 2U + sizeof(float), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 5, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 5, 6);

	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, index_buff.capacity() * sizeof(index),
		nullptr, GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao, ibo);
}
/*!***********************************************************************
\brief
 Renderer destructor
*************************************************************************/
Renderer::Renderer::~Renderer()
{
	//cleanup on server side
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}
/*!***********************************************************************
\brief
 create_render_object stores a pointer to render object
 into active_objs list. I don't know what else to say. OOOONGA BOONGA

\param obj
 Pointer to a render object to be added to List
*************************************************************************/
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
/*!***********************************************************************
\brief
 remove_render_object removes a render object pointer from its active
 object list. Shock horror.

\param obj
 Pointer to a render object to be removed to List
*************************************************************************/
void Renderer::Renderer::remove_render_object(const LB::CPRender* obj)
{
	for (int i{ 0 }; i < 4; ++i) {
		quad_buff[obj->get_index()].data[i].active = false;
	}

	active_objs.remove_if([obj](const LB::CPRender* in_list) { return *obj == *in_list; });
}
/*!***********************************************************************
\brief
 update_buff is the function that will update all render object values
 and then pass the value to GPU

\param r_type
 Type of render object
*************************************************************************/
void Renderer::Renderer::update_buff()
{
	for (const LB::CPRender*& e : active_objs) {
		unsigned int obj_index{ e->get_index() };
		//create index in index buffer
		unsigned short idx{ unsigned short(e->get_index() * 4) };

		index_buff.at(obj_index) = index{ std::array<unsigned short, 6>{idx, (unsigned short)(idx + 1), (unsigned short)(idx + 2),
			(unsigned short)(idx + 2), (unsigned short)(idx + 3), idx} };

		if (e->get_queue_size()) {
			const_cast<LB::CPRender*>(e)->animate();
			for (int i{ 0 }; i < 4; ++i) {
				quad_buff[obj_index].data[i].tex.x = e->uv[i].x; // 0 = bot left, 1 = bot right, 2 = top right, 3 = top left
				quad_buff[obj_index].data[i].tex.y = e->uv[i].y; // 0 = bot left, 1 = bot right, 2 = top right, 3 = top left
			}
		}

		const_cast<LB::CPRender*>(e)->get_transform_data();
		//set position based off camera mat
		//edit color and uv coordinates and texture
		for (int i{ 0 }; i < 4; ++i) {
			quad_buff[obj_index].data[i].pos.x = e->position.x;//pos.x;
			quad_buff[obj_index].data[i].pos.y = e->position.y;//pos.y;

			quad_buff[obj_index].data[i].widHeightType.x = e->w;
			quad_buff[obj_index].data[i].widHeightType.y = e->h;
			quad_buff[obj_index].data[i].widHeightType.z = (float)i;
			quad_buff[obj_index].data[i].others.x = e->scal.x;
			quad_buff[obj_index].data[i].others.y = e->scal.y;
			quad_buff[obj_index].data[i].others.z = e->rotation;

			quad_buff[obj_index].data[i].color.x = e->col.x;
			quad_buff[obj_index].data[i].color.y = e->col.y;
			quad_buff[obj_index].data[i].color.z = e->col.z;
			if(quad_buff[obj_index].data[i].texIndex != (float)e->texture)
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

/*!***********************************************************************
\brief
 RenderSystem constructor sets up a shader program and initalizes a background
*************************************************************************/
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

	GLint uni_loc = glGetUniformLocation(shader_program, "cam");
	if (uni_loc == -1)
		std::cerr << "Cannot find uniform location" << std::endl;
	glUniformMatrix4fv(uni_loc, 1, GL_FALSE, &object_renderer.cam.world_NDC[0][0]);

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

/*!***********************************************************************
\brief
 RenderSystem destructor
*************************************************************************/
Renderer::RenderSystem::~RenderSystem()
{
	if (GRAPHICS)
		GRAPHICS = nullptr;

	glDeleteProgram(shader_program);
}

/*!***********************************************************************
\brief
 The update function that gets called every loop also in charge of
 drawing everything. Counterintuitive, I know.
*************************************************************************/
void Renderer::RenderSystem::Update()
{
	bg_renderer.update_buff();
	object_renderer.update_buff();
	glClearColor(.3f, 0.5f, .8f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader_program);
	glBindVertexArray(bg_renderer.get_vao());
	glDrawElements(GL_TRIANGLES, (GLsizei)(bg_renderer.get_ao_size() * 6), GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(object_renderer.get_vao());
	glDrawElements(GL_TRIANGLES, (GLsizei)(object_renderer.get_ao_size() * 6), GL_UNSIGNED_SHORT, NULL);
}
/*!***********************************************************************
\brief
 Draw function does nothing for now
*************************************************************************/
void Renderer::RenderSystem::Draw()
{
	//TODO HAVE RENDERER PERFORM THE SWAP BUFFER INSTEAD OF WINDOW
	// Draw stuff
}
/*!***********************************************************************
\brief
 create_texture acts as an API for any developer to load texture data into
 the GPU.

\param file_path
 The file path of the texture you want to load

\param name
 The std::string key to assign to the Texture object

\return
 true if texture is successfully loaded into GPU and false if not.
*************************************************************************/
bool Renderer::RenderSystem::create_texture(const std::string& file_path, const std::string& name)
{
	return t_Manager.add_texture(file_path, name);
}
/*!***********************************************************************
\brief
 remove_texture is another API to delete texture data from the GPU.

\param name
 The key assigned to the Texture object

\return
 true if texture is successfully removed from GPU and false if not.
*************************************************************************/
bool Renderer::RenderSystem::remove_texture(const std::string& name)
{
	return t_Manager.remove_texture(name);
}
/*!***********************************************************************
\brief
 flush_textures is another API to delete all texture data from the GPU.
*************************************************************************/
void Renderer::RenderSystem::flush_textures()
{
	t_Manager.flush_textures();
}
/*!***********************************************************************
\brief
 create_object is a way to create a render object with a specific render
 type that determines which renderer the render object would belong to.

\param r_type
 The type of render object

\param obj
 Poitner to a render object that was just created
*************************************************************************/
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
	return 0;
}
/*!***********************************************************************
\brief
 remove_object is a way to remove a render object from a renderer the render
 object would belongs to.

\param r_type
 The type of render object

\param obj
 Poitner to a render object that was just created
*************************************************************************/
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
///*!***********************************************************************
//\brief
// Constructor for the texture object
//\param path
// The file path of the texture image
//*************************************************************************/
//Renderer::Texture::Texture(const std::string& path) :
//	id{ 0 }, file_path{ path }, local_buff{ nullptr },
//	w{ 0 }, h{ 0 }, fluff{ 0 }
//{
//	stbi_set_flip_vertically_on_load(1);
//
//	local_buff = stbi_load(path.c_str(), &w, &h, &fluff, 4);
//	if (!local_buff) {
//		std::cerr << "Texture file path: " << path << " NOT FOUND!" << std::endl;
//		return;
//	}
//
//	glCreateTextures(GL_TEXTURE_2D, 1, &id);
//	glTextureStorage2D(id, 1, GL_RGBA8, w, h);
//	glTextureSubImage2D(id, 0, 0, 0, w, h,
//		GL_RGBA, GL_UNSIGNED_BYTE, local_buff);
//
//	stbi_image_free(local_buff);
//
//
//	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//	//std::cout << "Picture specs: " << id << " " << w << " " << h << " " << fluff << std::endl;
//}

/*!***********************************************************************
\brief
 Texture class destructor
*************************************************************************/
Renderer::Texture::~Texture()
{
	//std::cout << "DELETED TEXTURE " << id << std::endl;
	glDeleteTextures(1, &id);
}

/*!***********************************************************************
\brief
 add_texture supposedly adds a texture to GPU memory and binds it to an
 available texture slot.

\param file_path
 The file path of the texture, duh

\param name
 The key in the form of a std::string to get the Texture object and its
 relevant data

\return
 The initial intention was to return true if successfully loaded texture
 and false if not but I'm not gonna lie I don't think it does even that.
*************************************************************************/
bool Renderer::Texture_Manager::add_texture(const std::string& file_path, const std::string& name)
{
	LB::ASSETMANAGER->AddTexture(file_path, name);

	GLint uni_loc = glGetUniformLocation(GRAPHICS->get_shader(), "u_SamplerID");
	int test[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	glUniform1iv(uni_loc, 13, test);

	return true;
}

/*!***********************************************************************
\brief
 remove_texture frees a specified Texture data from the GPU

\param name
 The key in the form of a std::string to find the Texture object and its
 relevant data

\return
 The initial intention was to return true if successfully deleted texture
 and false if not but I'm not gonna lie I don't think it does even that.

 Yes I did copy paste this header how'd you know?
*************************************************************************/
bool Renderer::Texture_Manager::remove_texture(const std::string& name)
{
	//unbind texture at that position
	free[textures.find(name)->second.second] = false;
	delete textures.find(name)->second.first;
	return textures.erase(name);
}

/*!***********************************************************************
\brief
 flush_textures frees all Texture data from the GPU. Just like the posb
 advertisements: So Simple! :D
*************************************************************************/
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