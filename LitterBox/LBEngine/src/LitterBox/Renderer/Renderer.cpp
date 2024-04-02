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
#include "LitterBox/Components/CameraComponent.h"

#include "../../dependencies/glm/glm/ext/matrix_clip_space.hpp"

#include "LitterBox/Engine/Time.h"
#include "LitterBox/Debugging/Debug.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "TileMap.h"


//---------------------------------DEFINES-------------------------------
constexpr Renderer::index inactive_idx{ 0,0,0,0,0 };
extern LB::CPCamera* game_cam{ nullptr };
extern LB::WindowsSystem* LB::WINDOWSSYSTEM;
//---------------------------------DEFINES-------------------------------

//-----------------------------------------HELPER FUNCTIONS--------------------------------



Renderer::Renderer_Types& Renderer::operator++(Renderer_Types& rt) {
	using utype = std::underlying_type_t<Renderer_Types>;
	//increase rt forward
	rt =
		//check rt is not last
		rt != Renderer_Types::Last ?
		static_cast<Renderer_Types>(static_cast<utype>(rt) + 1) :
		//else wrap around
		static_cast<Renderer_Types>(0);
	return rt;
}

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
		//printf("COULD NOT OPEN FILE %s\n", shader_file_name);
		DebuggerLogErrorFormat("Could not open file %s", shader_file_name);
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
		DebuggerLogErrorFormat("Failed to compile shader: %s", message);
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

//--------------------------------------CPCamera----------------------------------

/*!***********************************************************************
\brief
 Function initialises the game camera object in the scene to a single
 camera.

 NOTE: you can have multiple camera components in multiple game objects
 but only one will affect the actual game camera
*************************************************************************/
void LB::CPCamera::Initialise() {
	if (!game_cam)
		game_cam = this;
	else
		DebuggerLogError("Object already exists bozo");
}

/*!***********************************************************************
\brief
 Function to get the position coordinates of the transform component of
 the game object that the game camera is attached to.

\return
 The x and y coordinates of the game object
*************************************************************************/
LB::Vec2<float> LB::CPCamera::getCam()
{
	return gameObj->GetComponent<CPTransform>()->GetPosition();
}
//--------------------------------------CPCamera----------------------------------


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
void Renderer::Animation_Manager::load_anim(const std::string& animation_name, const std::array<LB::Vec2<float>, 4>* data, const float anim_time, const int number_of_frames) {
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
	uv{ uv }, frame{ 0 }, time_elapsed{ 0.f }, rotation{ 0.f }, transform{ nullptr },
	indices{}, z_val{ 1.f }
{
	if (!Renderer::GRAPHICS) {
		DebuggerLogError("GRAPHICS SYSTEM NOT INITIALIZED");
		return;
	}


	quad_id = Renderer::GRAPHICS->create_object(renderer_id, this);

	unsigned short idx{ (unsigned short)(quad_id * 4) };

	indices = Renderer::index{ std::array<unsigned short, 6>{idx, (unsigned short)(idx + 1), (unsigned short)(idx + 2),
			(unsigned short)(idx + 2), (unsigned short)(idx + 3), idx} };

	parent = nullptr;
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
		DebuggerLogErrorFormat("There is no animation: %s loaded.", name.c_str());
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
		DebuggerLogErrorFormat("There is no animation: %s loaded.", name.c_str());
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
		DebuggerLogErrorFormat("There is no animation: %s loaded.", name.c_str());
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

	frame = 0;
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

/*!***********************************************************************
\brief
 set_active is an API that will toggle whether a render component will be
 rendered on the screen or not.
*************************************************************************/
void LB::CPRender::set_active()
{
	activated = !activated;
	if (Renderer::GRAPHICS)
		Renderer::GRAPHICS->change_object_state(renderer_id, this);
	else
		DebuggerLogError("GRAPHICS SYSTEM NOT INITIALIZED");
}

/*!***********************************************************************
\brief
 Destroy acts as the destructor for the CPRender object and is the API for
 deallocating the resources given to the component from the RenderSystem
 side.
*************************************************************************/
void LB::CPRender::Destroy()
{
	if (!destroyed) {
		Renderer::GRAPHICS->remove_object(renderer_id, this);
		destroyed = true;
	}
}
/*!**********************************************************************
\brief
Destructor of the CPRender component.
*************************************************************************/
LB::CPRender::~CPRender() {
	//this->Destroy();
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
	quad_buff_size{}, active_objs{}, active{ true }
{
	//create vertex
	//quad_buff_size = 3000;
	//TODO have a proper reasoning for limits to buff size
	switch (renderer) {
	case Renderer_Types::RT_OBJECT:
		quad_buff_size = 3000;
		break;
	case Renderer_Types::RT_BACKGROUND:
		quad_buff_size = 1000;
		break;
	case Renderer_Types::RT_DEBUG:
		quad_buff_size = 200;
		break;
	case Renderer_Types::RT_UI:
		quad_buff_size = 100;
		break;
	case Renderer_Types::RT_MENU:
		quad_buff_size = 50;
		break;
	}
	quad_buff = DBG_NEW quad[quad_buff_size];
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
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 0, 1);
	//texture coordinates
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayVertexBuffer(vao, 2, vbo, sizeof(LB::Vec3<float>), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 1, 2);
	//color coordinates
	glEnableVertexArrayAttrib(vao, 2);
	glVertexArrayVertexBuffer(vao, 3, vbo, sizeof(LB::Vec2<float>) + sizeof(LB::Vec3<float>), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 2, 3);
	//texture index
	glEnableVertexArrayAttrib(vao, 3);
	glVertexArrayVertexBuffer(vao, 4, vbo, sizeof(LB::Vec2<float>) + sizeof(LB::Vec3<float>) * 2U, sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 3, 1, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 3, 4);
	//width height data
	glEnableVertexArrayAttrib(vao, 4);
	glVertexArrayVertexBuffer(vao, 5, vbo, sizeof(LB::Vec2<float>) + sizeof(LB::Vec3<float>) * 2U + sizeof(float), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 4, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 4, 5);
	//scaling and rotation data
	glEnableVertexArrayAttrib(vao, 5);
	glVertexArrayVertexBuffer(vao, 6, vbo, sizeof(LB::Vec2<float>) + sizeof(LB::Vec3<float>) * 3U + sizeof(float), sizeof(Vertex));
	glVertexArrayAttribFormat(vao, 5, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao, 5, 6);

	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, index_buff.capacity() * sizeof(index),
		nullptr, GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao, ibo);
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
		DebuggerLogError("OBJECT BUFFER FULL");

		//return quad_data full of garbage
		return i;
	}

	//set position of quad
	quad_buff[i].data[0].pos = { -0.5f, -0.5f, 1.f };//bottom left
	quad_buff[i].data[1].pos = { 0.5f, -0.5f, 1.f };//bottom right
	quad_buff[i].data[2].pos = { 0.5f, 0.5f, 1.f };//top right
	quad_buff[i].data[3].pos = { -0.5f, 0.5f, 1.f };//top left

	quad_buff[i].data[0].tex = { 0.f, 0.f };
	quad_buff[i].data[1].tex = { 1.f, 0.f };
	quad_buff[i].data[2].tex = { 1.f, 1.f };
	quad_buff[i].data[3].tex = { 0.f, 1.f };

	unsigned short idx{ (unsigned short)(i * 4) };

	index_buff.at(i) = index{ std::array<unsigned short, 6>{idx, (unsigned short)(idx + 1), (unsigned short)(idx + 2),
			(unsigned short)(idx + 2), (unsigned short)(idx + 3), idx} };

	active_objs.emplace_back(obj);

	furthest_index = i > furthest_index ? i : furthest_index;

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
		memset(quad_buff + obj->get_index(), 0, sizeof(quad));
		//quad_buff[obj->get_index()].data[i].active = false;
	}

	//set the indices to 0
	index_buff.at(obj->get_index()) = index{ 0,0,0,0,0,0 };
	active_objs.remove_if([obj](const LB::CPRender* in_list) { return obj == in_list; });

	if (active_objs.size()) {
		unsigned int max_index{ active_objs.front()->get_index() };
		for (auto const& e : active_objs) {
			max_index = e->get_index() > max_index ? e->get_index() : max_index;
		}
		furthest_index = max_index;
	}
	else {
		furthest_index = 0;
	}
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
		if (!e->activated)
			continue;
		if (e->texture == 0)
			continue;
		unsigned int obj_index{ e->get_index() };
		const_cast<LB::CPRender*>(e)->get_transform_data();

		//set position based off camera mat
		//edit color and uv coordinates and texture
		for (int i{ 0 }; i < 4; ++i) {
			quad_buff[obj_index].data[i].pos.x = e->position.x;//pos.x;
			quad_buff[obj_index].data[i].pos.y = e->position.y;//pos.y;
			quad_buff[obj_index].data[i].pos.z = e->z_val;//pos.z;

			quad_buff[obj_index].data[i].widHeightType.x = e->w;
			quad_buff[obj_index].data[i].widHeightType.y = e->h;
			quad_buff[obj_index].data[i].widHeightType.z = (float)i;
			quad_buff[obj_index].data[i].others.x = e->scal.x;
			quad_buff[obj_index].data[i].others.y = e->scal.y;
			quad_buff[obj_index].data[i].others.z = e->rotation;

			quad_buff[obj_index].data[i].color.x = e->col.x;
			quad_buff[obj_index].data[i].color.y = e->col.y;
			quad_buff[obj_index].data[i].color.z = e->col.z;
			if (quad_buff[obj_index].data[i].texIndex != (float)e->texture)
				quad_buff[obj_index].data[i].texIndex = (float)e->texture;
			//update uv
			//for (int j{ 0 }; j < 4; ++j) {
			quad_buff[obj_index].data[i].tex.x = e->uv[i].x; // 0 = bot left, 1 = bot right, 2 = top right, 3 = top left
			quad_buff[obj_index].data[i].tex.y = e->uv[i].y; // 0 = bot left, 1 = bot right, 2 = top right, 3 = top left
			//}
		}
	}

	glNamedBufferSubData(vbo, 0, sizeof(quad) * quad_buff_size, quad_buff);
	glNamedBufferSubData(ibo, 0, sizeof(index) * index_buff.size(), index_buff.data());
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		std::cerr << (int)err << std::endl;

}
/*!***********************************************************************
\brief
 update_anim is the function that will update all render object animations
 if the object has any.
*************************************************************************/
void Renderer::Renderer::update_anim()
{
	for (const LB::CPRender*& e : active_objs) {
		if (!e->activated)
			continue;

		unsigned int obj_index{ e->get_index() };

		if (e->get_queue_size()) {
			const_cast<LB::CPRender*>(e)->animate();
			for (int i{ 0 }; i < 4; ++i) {
				quad_buff[obj_index].data[i].tex.x = e->uv[i].x; // 0 = bot left, 1 = bot right, 2 = top right, 3 = top left
				quad_buff[obj_index].data[i].tex.y = e->uv[i].y; // 0 = bot left, 1 = bot right, 2 = top right, 3 = top left
			}
		}
	}
}
/*!***********************************************************************
\brief
 change_render_state will change the indices in the index buffer so that
 a particular will or will not be shown.

\param object
 The CPRender object which you want to either show or hide.
*************************************************************************/
void Renderer::Renderer::change_render_state(const LB::CPRender& object)
{
	index_buff.at(object.get_index()) = object.activated ? object.get_indices() : inactive_idx;
}

/*!***********************************************************************
\brief
 Destroy_Renderer is the function that should be called when RenderSystem
 is going out of scope.

 NOTE: Anything you want done in the destructor should be done here instead.
*************************************************************************/
void Renderer::Renderer::Destroy_Renderer()
{
	//cleanup on server side
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	if (quad_buff) {
		delete[] quad_buff;
		quad_buff = nullptr;
	}
	GRAPHICS = nullptr;
}
//----------------------------------------------RENDERER---------------------------------------------------


//---------------------------------------------TEXTRENDERER------------------------------------------------
/*!***********************************************************************
\brief
 Constructor will load the freetype library and font then store all the
 character glpyh information the in the map member followed by freeing
 of the freetype library.
*************************************************************************/
Renderer::TextRenderer::TextRenderer() : //Characters{}, 
	tShader{}, tVao{}, tVbo{}, active_msgs{}, maxBound{} //, ft{}, font{} 
{
	//--------------------------------DEPRECATED---------------------------------------
	////Get fonts
	//auto fonts{ LB::FILESYSTEM->GetFilesOfType(".otf") };
	//auto fonts2{ LB::FILESYSTEM->GetFilesOfType(".ttf") };


	////-------------------LOAD FONT------------------------
	////init freetype lib
	//if (FT_Init_FreeType(&ft)) {
	//	DebuggerLogError("ERROR On the freetype: could not init the lib");
	//}

	////load all fonts both otf and ttf
	//for (auto const& e : fonts) {
	//	//load font
	//	if (FT_New_Face(ft, e.string().c_str(), 0, &font)) {
	//		DebuggerLogError("ERROR on the freetype: could not load font");
	//	}
	//	//set default font face
	//	FT_Set_Pixel_Sizes(font, 0, 50); //the width is 0 so it is based off the height value

	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//	for (unsigned char c{}; c < 128; ++c) {
	//		//load glyph
	//		if (FT_Load_Char(font, c, FT_LOAD_RENDER)) {
	//			DebuggerLogErrorFormat("ERROR on the freetype: could not load glyph %c", c);
	//			continue;
	//		}
	//		//generate texture
	//		unsigned int character_glyph;
	//		glGenTextures(1, &character_glyph);
	//		glBindTexture(GL_TEXTURE_2D, character_glyph);
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
	//			font->glyph->bitmap.width, font->glyph->bitmap.rows,
	//			0, GL_RED, GL_UNSIGNED_BYTE, font->glyph->bitmap.buffer);
	//		//set texture options
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//		//store that shit
	//		Character sc = {
	//			character_glyph,
	//			LB::Vec2<unsigned int>{font->glyph->bitmap.width, font->glyph->bitmap.rows},
	//			LB::Vec2<FT_Int>{font->glyph->bitmap_left, font->glyph->bitmap_top},
	//			static_cast<unsigned int>(font->glyph->advance.x)
	//		};
	//		Characters.emplace(std::pair<char, Character>(c, sc));
	//	}
	//	//-------------------LOAD FONT------------------------
	//	font_glyphs.emplace(std::make_pair(e.stem().string(), Characters));
	//	Characters.clear();
	//	//free up all the used resources from FT
	//	FT_Done_Face(font);
	//}

	//for (auto const& e : fonts2) {
	//	//load font
	//	if (FT_New_Face(ft, e.string().c_str(), 0, &font)) {
	//		DebuggerLogError("ERROR on the freetype: could not load font");
	//	}
	//	//set default font face
	//	FT_Set_Pixel_Sizes(font, 0, 50); //the width is 0 so it is based off the height value

	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//	for (unsigned char c{}; c < 128; ++c) {
	//		//load glyph
	//		if (FT_Load_Char(font, c, FT_LOAD_RENDER)) {
	//			DebuggerLogErrorFormat("ERROR on the freetype: could not load glyph %c", c);
	//			continue;
	//		}
	//		//generate texture
	//		unsigned int character_glyph;
	//		glGenTextures(1, &character_glyph);
	//		glBindTexture(GL_TEXTURE_2D, character_glyph);
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
	//			font->glyph->bitmap.width, font->glyph->bitmap.rows,
	//			0, GL_RED, GL_UNSIGNED_BYTE, font->glyph->bitmap.buffer);
	//		//set texture options
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//		//store that shit
	//		Character sc = {
	//			character_glyph,
	//			LB::Vec2<unsigned int>{font->glyph->bitmap.width, font->glyph->bitmap.rows},
	//			LB::Vec2<FT_Int>{font->glyph->bitmap_left, font->glyph->bitmap_top},
	//			static_cast<unsigned int>(font->glyph->advance.x)
	//		};
	//		Characters.emplace(std::pair<char, Character>(c, sc));
	//	}
	//	//-------------------LOAD FONT------------------------

	//	//-------------------LOAD FONT------------------------
	//	font_glyphs.emplace(std::make_pair(e.stem().string(), Characters));
	//	Characters.clear();
	//	//free up all the used resources from FT
	//	FT_Done_Face(font);
	//}
	////free ft lib
	//FT_Done_FreeType(ft);

	////create the vertex array and buffer
	//glGenVertexArrays(1, &tVao);
	//glGenBuffers(1, &tVbo);
	//glBindVertexArray(tVao);
	//glBindBuffer(GL_ARRAY_BUFFER, tVbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	////setup the shader program
	//shader_source shd_pgm{ shader_parser("Assets/Shaders/text.shader") };
	//tShader = create_shader(shd_pgm.vtx_shd.c_str(), shd_pgm.frg_shd.c_str());

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//--------------------------------DEPRECATED---------------------------------------
	LB::ASSETMANAGER->LoadFonts(reinterpret_cast<void*>(this));
}
/*!***********************************************************************
\brief
 RenderText loads a message, loops through each character in the message and
 renders the appropriate character in openGL using the character glyph
 information.

\param msg
 The message with the string and text you want to render in openGL
*************************************************************************/
void Renderer::TextRenderer::RenderText(message& msg) {
	glUseProgram(tShader);
	glUniform3f(glGetUniformLocation(tShader, "textColor"), msg.color.x, msg.color.y, msg.color.z);
	//glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(tVao);

	GLuint uni_loc = glGetUniformLocation(tShader, "projection");
	if (uni_loc == -1)
		DebuggerLogError("Unable to find uniform location");
	if (msg.use_world_space)
		glUniformMatrix4fv(uni_loc, 1, GL_FALSE, &GRAPHICS->get_game_cam_mat()[0][0]);
	else
		glUniformMatrix4fv(uni_loc, 1, GL_FALSE, &GRAPHICS->get_ui_cam_mat()[0][0]);

	float x = msg.x;
	float y = msg.y;

	size_t stroffset{ 0 };
	while (stroffset != std::string::npos)
	{
		size_t end_of_word{ msg.text.find_first_of(' ', stroffset) == std::string::npos ? msg.text.size() : msg.text.find_first_of(' ', stroffset) };
		std::string word{ msg.text.substr(stroffset, end_of_word - stroffset + 1) };

		//bool newline{ false };

		int adv{ 0 };
		float prev_x = x;
		for (auto const& cha : word)
		{
			Character ch = font_glyphs[msg.font_file_name_wo_ext][cha];
			x += (ch.Advance >> 6) * msg.finalScale;
			adv = ch.Advance;
		}

		if (x > msg.x + msg.xbound * msg.transScale)
		{
			y -= (font_height_adv[msg.font_file_name_wo_ext] >> 6) * msg.transScale;
			x = msg.x;
		}
		else
		{
			x = prev_x;
		}

		for (auto const& cha : word) {
			Character ch = font_glyphs[msg.font_file_name_wo_ext][cha];//Characters[cha];

			float xpos = x + ch.Bearing.x * msg.finalScale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * msg.finalScale;

			float w = ch.Size.x * msg.finalScale;
			float h = ch.Size.y * msg.finalScale;



			float vertices[6][4] = {
				{xpos, ypos + h, 0.f, 0.f},
				{xpos, ypos, 0.f, 1.f},
				{xpos + w, ypos, 1.f, 1.f},

				{xpos, ypos + h, 0.f, 0.f},
				{xpos + w, ypos, 1.f, 1.f},
				{xpos + w, ypos + h, 1.f, 0.f}
			};
			//bind texture
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			//update vbo
			glBindBuffer(GL_ARRAY_BUFFER, tVbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			//move to the left by the glyph advance value
			x += (ch.Advance >> 6) * msg.finalScale;
		}

		stroffset = msg.text.find_first_of(' ', stroffset + 1);
		stroffset += stroffset == std::string::npos ? 0 : 1;
	}

	//iterate through all chars
	//for (auto const& cha : msg.text) {
	//	Character ch = font_glyphs[msg.font_file_name_wo_ext][cha];//Characters[cha];

	//	float xpos = x + ch.Bearing.x * msg.scale;
	//	if (xpos > msg.x + msg.xbound)
	//	{
	//		y -= (ch.Advance >> 6) * msg.scale + 10.f;
	//		x = msg.x;
	//		xpos = x + ch.Bearing.x * msg.scale;
	//	}

	//	float ypos = y - (ch.Size.y - ch.Bearing.y) * msg.scale;

	//	float w = ch.Size.x * msg.scale;
	//	float h = ch.Size.y * msg.scale;

	//	

	//	float vertices[6][4] = {
	//		{xpos, ypos + h, 0.f, 0.f},
	//		{xpos, ypos, 0.f, 1.f},
	//		{xpos + w, ypos, 1.f, 1.f},

	//		{xpos, ypos + h, 0.f, 0.f},
	//		{xpos + w, ypos, 1.f, 1.f},
	//		{xpos + w, ypos + h, 1.f, 0.f}
	//	};
	//	//bind texture
	//	glBindTexture(GL_TEXTURE_2D, ch.TextureID);
	//	//update vbo
	//	glBindBuffer(GL_ARRAY_BUFFER, tVbo);
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glDrawArrays(GL_TRIANGLES, 0, 6);

	//	//move to the left by the glyph advance value
	//	x += (ch.Advance >> 6) * msg.scale;
	//}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
/*!***********************************************************************
\brief
 Destroy_TextRend will free all the data on the OpenGL side back to the
 GPU.
*************************************************************************/
void Renderer::TextRenderer::Destroy_TextRend()
{
	glDeleteProgram(tShader);
	glDeleteVertexArrays(1, &tVao);
	glDeleteBuffers(1, &tVbo);
}

/*!***********************************************************************
\brief
 Used by text renderer to update the text information
*************************************************************************/
void Renderer::TextRenderer::update_text()
{
	for (auto& text : active_msgs) {
		text->Update();

		RenderText(text->get_msg());
	}
}
//---------------------------------------------TEXTRENDERER------------------------------------------------


//----------------------------------------------RENDERER-SYSTEM-------------------------------------------
//Global pointer to render system for accessing of
//render objects
Renderer::RenderSystem* Renderer::GRAPHICS = nullptr;


/*!***********************************************************************
\brief
 RenderSystem constructor sets up a shader program and initalizes a background
*************************************************************************/
Renderer::RenderSystem::RenderSystem() : shader_program{ 0 },
object_renderer{ Renderer_Types::RT_OBJECT },
bg_renderer{ Renderer_Types::RT_BACKGROUND },
ui_renderer{ Renderer_Types::RT_UI },
menu_renderer{ Renderer_Types::RT_MENU },
text_renderer{},
framebuffer{},
svfb{},
svtcb{},
textureColorbuffer{},
rbo{}
{
	SetSystemName("Renderer System");
	//singleton that shiet
	if (!GRAPHICS)
		GRAPHICS = this;
	else
		DebuggerLogError("Render System already exists");
}

//----------For ImGUI rendering---------------
bool imgui_ready{ false }; // to make sure ImGui doesn't render empty
//----------For ImGUI rendering---------------


//----------text button v1------------

/*!***********************************************************************
\brief
 textbutt is an early implementation to render an In Game textbox GUI
 button.
*************************************************************************/
struct textbutt {
	LB::CPRender background{ {0.f ,0.f}, 50.f, 50.f, {1.f,1.f}, {1.f,1.f,1.f}, {}, -1, true, Renderer::Renderer_Types::RT_UI };
	LB::CPText text;


	/*!***********************************************************************
	\brief
	 update_text will render the text in middle left aligned with the render
	 box of the object.
	*************************************************************************/
	void update_text() {
		LB::Vec2<float> textposition{};
		//number based on initial font size when loading freetype font
		textposition.y = background.position.y - 25.f;
		textposition.x = background.position.x - background.w * 0.5f;

		//number based on initial font size when loading freetype font
		float nscale = background.h < 50.f ? background.h / 50.f : 1.f;

		text.update_msg_pos(textposition);
		text.update_msg_size(nscale);
	}

	/*!***********************************************************************
	\brief
	 textbutt is the contstructor that make sure all the member variables are
	 properly initialized so that it can be seen in the game view
	*************************************************************************/
	textbutt() {
		text.Initialise();
		text.update_msg_font("KernlGrotesk");
		update_text();
	}
};
//----------text button v1------------


LB::CPRender* test2;
std::vector<LB::CPRender*> backgrounds;
LB::CPRender* buttonbg_1;
LB::CPRender* buttonbg_2;

LB::CPText* text;
LB::CPText* text2;
textbutt* button;

/*!***********************************************************************
\brief
 REsponsible for updating the opengl viewport. Meant to subscribe to an
 event that is invoked everytime the window changes size. (Goes from fullscreen,
 to windowed mode)

\param
 float to store the height offset of the new viewport if any
*************************************************************************/
void change_vp()
{
	float height{ 9.f / 16.f };
	height *= (float)LB::WINDOWSSYSTEM->GetWidth();
	float diff{ (float)LB::WINDOWSSYSTEM->GetHeight() - height };
	glViewport(0, (int)(diff * 0.5f), LB::WINDOWSSYSTEM->GetWidth(), (int)height);

	LB::WINDOWSSYSTEM->updateScreenSize(static_cast<float>(LB::WINDOWSSYSTEM->GetWidth()), height, diff * 0.5f);
}

glm::mat4 cameraMat{ glm::perspective(glm::radians(90.f), 1920.f / 1080.f, 0.1f, 10.f) };

/*!***********************************************************************
\brief
 Initialize function from base class ISystem.
 Does cool whacky stuff. Or not.
*************************************************************************/
void Renderer::RenderSystem::Initialize()
{
	//set the initial values for x and y for the camera
	glfwGetCursorPos(LB::WINDOWSSYSTEM->GetWindow(), &cam.mouse_x, &cam.mouse_y);

	//shader_source shd_pgm{ shader_parser("Assets/Shaders/Basic.shader") };
	//shader_program = create_shader(shd_pgm.vtx_shd.c_str(), shd_pgm.frg_shd.c_str());
	LB::ASSETMANAGER->LoadShader("Assets/Shaders/Basic.shader", shader_program);

	glUseProgram(shader_program);
	glBindVertexArray(object_renderer.get_vao());

	//-------------------------cam test---------------------------
	GLint uni_loc = glGetUniformLocation(shader_program, "cam");
	if (uni_loc == -1)
		DebuggerLogError("Unable to find uniform location");
	glUniformMatrix4fv(uni_loc, 1, GL_FALSE, &cam.world_NDC[0][0]);
	//-------------------------cam test---------------------------

	GLint uni_loc2 = glGetUniformLocation(GRAPHICS->get_shader(), "u_SamplerID");
	int test[32]{};
	for (int i{ 0 }; i < 32; ++i) {
		test[i] = i;
	}
	glUniform1iv(uni_loc2, 32, test);

	//-################FOR BACKGROUND##########################

	//TODO: SERIALIZE THIS AMADEUS
	/*LB::TileMap tm(4, 4, 7, 6, "TilemapsTransparent_SpriteSheet",
		{ 1, 3, 3, 2,
		 9, 0, 0, 10,
		 9, 5, 4, 8,
		 21, 8, 17, 18 });*/

		 //cache some values
		 /*float midx = 0.f;
		 float midy = 0.f;
		 float w = 4000.f;
		 float h = 4000.f;

		 test2 = LB::Memory::Instance()->Allocate<LB::CPRender>(LB::Vec2<float>(midx, midy), w, h, LB::Vec2<float>(1.f, 1.f), LB::Vec3<float>(0.f, 0.f, 0.f), std::array<LB::Vec2<float>, 4>{}, -1, true, Renderer_Types::RT_BACKGROUND);
		 test2->z_val = 2.f;

		 test2->texture = LB::ASSETMANAGER->GetTextureUnit("bg");
		 test2->uv[0].x = 0.25f;
		 test2->uv[0].y = 0.25f;
		 test2->uv[1].x = .75f;
		 test2->uv[1].y = 0.25f;
		 test2->uv[2].x = .75f;
		 test2->uv[2].y = .75f;
		 test2->uv[3].x = 0.25f;
		 test2->uv[3].y = .75f;*/

		 //LB::LoadMap(tm);
		 //-################FOR BACKGROUND##########################
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//turnOnEditorMode();
	//delete text;

	if (!editor_mode) {
		change_vp();
		LB::WINDOWSSYSTEM->screenSizeChange.Subscribe(change_vp, 0);
	}
}

/*!***********************************************************************
 \brief
 Toggles the window for rendering, in editor mode, will render to the
 Gameview instead.
*************************************************************************/
void Renderer::RenderSystem::turnOnEditorMode() {
	//----For rendering scene onto texture for ImGUI-------------
	//TODO make the monitor dimensions based on the window instead of primary monitor
	if (!editor_mode) {
		GLFWvidmode dimensions;
		GLFWmonitor* mon = glfwGetPrimaryMonitor();
		dimensions = *glfwGetVideoMode(mon);

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		//create the texture that the frame buffer writes too
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, LB::WINDOWSSYSTEM->GetWidth(), LB::WINDOWSSYSTEM->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		//ATTEMPTING SOMETHING COOL
		//using the same render buffer, for depth testing, for both frame buffers
		/*glGenRenderbuffers(1, &rbo);
		glNamedRenderbufferStorage(rbo, GL_DEPTH_COMPONENT32, LB::WINDOWSSYSTEM->GetWidth(), LB::WINDOWSSYSTEM->GetHeight());
		glNamedFramebufferRenderbuffer(framebuffer, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);*/

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, LB::WINDOWSSYSTEM->GetWidth(), LB::WINDOWSSYSTEM->GetHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		//create the texture that the frame buffer writes too
		glGenFramebuffers(1, &svfb);
		glBindFramebuffer(GL_FRAMEBUFFER, svfb);

		glGenTextures(1, &svtcb);
		glBindTexture(GL_TEXTURE_2D, svtcb);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, LB::WINDOWSSYSTEM->GetWidth(), LB::WINDOWSSYSTEM->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, svtcb, 0);

		glNamedFramebufferRenderbuffer(svfb, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		imgui_ready = true;
		editor_mode = true;
	}
	//----For rendering scene onto texture for ImGUI-------------
}

/*!***********************************************************************
\brief
 The update function that gets called every loop also in charge of
 drawing everything. Counterintuitive, I know.

 NOTE: UI and Text are only rendered in game view because it renders directly
 to camera so the movement will be a little weirder.
*************************************************************************/
void Renderer::RenderSystem::Update()
{
	//set the shader program before hand
	glUseProgram(shader_program);
	/*if (game_cam)
	{
		cam.update_ortho_cam(game_cam->getCam());
		GLint uni_loc = glGetUniformLocation(shader_program, "cam");
		if (uni_loc == -1)
			DebuggerLogError("Unable to find uniform location");
		glUniformMatrix4fv(uni_loc, 1, GL_FALSE, &cam.world_NDC[0][0]);
	}*/

	GLint uni_loc = glGetUniformLocation(shader_program, "cam");
	if (uni_loc == -1)
		DebuggerLogError("Unable to find uniform location");
	//glUniformMatrix4fv(uni_loc, 1, GL_FALSE, &cameraMat[0][0]);
	glUniformMatrix4fv(uni_loc, 1, GL_FALSE, &cam.world_NDC[0][0]);

	if (editor_mode)
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // we're not using the stencil buffer now nor the depth either just in case you were wondering

	//Render the game scene window
	bg_renderer.update_buff();
	object_renderer.update_buff();
	ui_renderer.update_buff();
	menu_renderer.update_buff();

	//NOTE: for the depth buffer, because OpenGL is left handed
	//the higher the z-axis value, the further away it is from the camera
	//need to check if the specific layer is activated and render if it is
	if (bg_renderer.getActive())
	{
		glClear(GL_DEPTH_BUFFER_BIT); //we clear the depth buffer bit after drawing each layer to ensure that everything in the next layer gets drawn
		glBindVertexArray(bg_renderer.get_vao());
		glDrawElements(GL_TRIANGLES, (GLsizei)(bg_renderer.get_furthest_index() * 6), GL_UNSIGNED_SHORT, NULL);
	}
	if (object_renderer.getActive())
	{
		glClear(GL_DEPTH_BUFFER_BIT); //we clear the depth buffer bit after drawing each layer to ensure that everything in the next layer gets drawn
		glBindVertexArray(object_renderer.get_vao());
		glDrawElements(GL_TRIANGLES, (GLsizei)(object_renderer.get_furthest_index() * 6), GL_UNSIGNED_SHORT, NULL);
	}
	//need to check if the specific layer is activated and render if it is
	if (ui_renderer.getActive())
	{
		glClear(GL_DEPTH_BUFFER_BIT); //we clear the depth buffer bit after drawing each layer to ensure that everything in the next layer gets drawn
		glUniform1i(glGetUniformLocation(shader_program, "UI"), true);
		glUniformMatrix4fv(glGetUniformLocation(shader_program, "uicam"), 1, GL_FALSE, &cam.ui_NDC[0][0]);
		glBindVertexArray(ui_renderer.get_vao());
		glDrawElements(GL_TRIANGLES, (GLsizei)(ui_renderer.get_furthest_index() * 6), GL_UNSIGNED_SHORT, NULL);
		glUniform1i(glGetUniformLocation(shader_program, "UI"), false);
	}
	//print all messages here
	text_renderer.update_text();
	//print all messages here
	//menu layer
	if (menu_renderer.getActive())
	{
		glUseProgram(shader_program);
		glUniformMatrix4fv(uni_loc, 1, GL_FALSE, &cam.ui_NDC[0][0]);
		glClear(GL_DEPTH_BUFFER_BIT); //we clear the depth buffer bit after drawing each layer to ensure that everything in the next layer gets drawn
		glBindVertexArray(menu_renderer.get_vao());
		glDrawElements(GL_TRIANGLES, (GLsizei)(menu_renderer.get_furthest_index() * 6), GL_UNSIGNED_SHORT, NULL);
	}

	if (editor_mode) {
		glUseProgram(shader_program);
		glUniformMatrix4fv(uni_loc, 1, GL_FALSE, &cam.editor_world_NDC[0][0]);

		glBindFramebuffer(GL_FRAMEBUFFER, svfb);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // we're not using the stencil buffer now nor the depth either just in case you were wondering

		glClear(GL_DEPTH_BUFFER_BIT); //we clear the depth buffer bit after drawing each layer to ensure that everything in the next layer gets drawn
		if (bg_renderer.getActive())
		{
			glBindVertexArray(bg_renderer.get_vao());
			glDrawElements(GL_TRIANGLES, (GLsizei)(bg_renderer.get_furthest_index() * 6), GL_UNSIGNED_SHORT, NULL);
		}
		//need to check if the specific layer is activated and render if it is
		if (object_renderer.getActive())
		{
			glClear(GL_DEPTH_BUFFER_BIT); //we clear the depth buffer bit after drawing each layer to ensure that everything in the next layer gets drawn
			glBindVertexArray(object_renderer.get_vao());
			glDrawElements(GL_TRIANGLES, (GLsizei)(object_renderer.get_furthest_index() * 6), GL_UNSIGNED_SHORT, NULL);
		}
		//menu layer
		if (menu_renderer.getActive())
		{
			glClear(GL_DEPTH_BUFFER_BIT); //we clear the depth buffer bit after drawing each layer to ensure that everything in the next layer gets drawn
			glBindVertexArray(menu_renderer.get_vao());
			glDrawElements(GL_TRIANGLES, (GLsizei)(menu_renderer.get_furthest_index() * 6), GL_UNSIGNED_SHORT, NULL);
		}
		//UI and TEXT don't get rendered in scene view
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	if (isShake)
	{
		shakeDuration -= static_cast<float>(LB::TIME->GetDeltaTime());
		cam.move_cam(shakeIntensity);
	}
	//WARNING!! THIS WILL BE THE SOURCE OF A BUG WHEN WE DO THE BIGGER MAPS!!
	if (shakeDuration <= 0) { isShake = false; reset_cam(); }
}

/*!***********************************************************************
\brief
 render_msg adds a pointer to a new text component into text renderer.

\param obj
 The text component to be rendered
*************************************************************************/
void Renderer::RenderSystem::render_msg(LB::CPText* obj) {
	text_renderer.add_text_component(obj);
}

/*!***********************************************************************
\brief
 Updates the game viewport position and size based on the current ImGUI
 window.
*************************************************************************/
void Renderer::RenderSystem::UpdateGameWindowPos(GLint winPosX, GLint winPosY, GLsizei winSizeX, GLsizei winSizeY)
{
	m_winPos.x = winPosX;
	m_winPos.y = winPosY;
	m_winSize.x = winSizeX;
	m_winSize.y = winSizeY;
}

/*!***********************************************************************
\brief
 The FixedUpdate function is a time based update function that will only
 be called after a set amount of time is passed or will be called multiple
 times if application runs to slowly.

 NOTE: For rendering context FixedUpdate is used for consistent aniamtion
 regardless of framerate.
*************************************************************************/
void Renderer::RenderSystem::FixedUpdate()
{
	bg_renderer.update_anim();
	object_renderer.update_anim();
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

//---------------------------------------DEPRECATED------------------------------------
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
//bool Renderer::RenderSystem::create_texture(const std::string& file_path, const std::string& name)
//{
//	return t_Manager.add_texture(file_path, name);
//}
//---------------------------------------DEPRECATED------------------------------------


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
		//TODO for UI and DEBUG
	case Renderer_Types::RT_UI:
		return ui_renderer.create_render_object(obj);
	case Renderer_Types::RT_MENU:
		return menu_renderer.create_render_object(obj);
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
	case Renderer_Types::RT_UI:
		ui_renderer.remove_render_object(obj);
		break;
	case Renderer_Types::RT_MENU:
		menu_renderer.remove_render_object(obj);
		break;
	}
}
/*!***********************************************************************
\brief
 remove object from its current rendering layer and place it into the new
 specified rendering layer

\param new_type
 The new rendering layer we want to place the CPRender object into

\param obj
 Poitner to a render object that was just created
*************************************************************************/
void Renderer::RenderSystem::swap_object_type(Renderer_Types new_type, LB::CPRender* obj)
{
	unsigned int newid{ 0 };
	//simple check to reduce overheads
	if (obj->get_r_type() == new_type)
		return;

	//remove the object from it's current layer
	switch (obj->get_r_type()) {
	case Renderer_Types::RT_OBJECT:
		object_renderer.remove_render_object(obj);
		break;
	case Renderer_Types::RT_BACKGROUND:
		bg_renderer.remove_render_object(obj);
		break;
	case Renderer_Types::RT_UI:
		ui_renderer.remove_render_object(obj);
		break;
	case Renderer_Types::RT_MENU:
		menu_renderer.remove_render_object(obj);
		break;
	}
	//add the object to the new layer
	switch (new_type) {
	case Renderer_Types::RT_OBJECT:
		newid = object_renderer.create_render_object(obj);
		break;
	case Renderer_Types::RT_BACKGROUND:
		newid = bg_renderer.create_render_object(obj);
		break;
	case Renderer_Types::RT_UI:
		newid = ui_renderer.create_render_object(obj);
		break;
	case Renderer_Types::RT_MENU:
		newid = menu_renderer.create_render_object(obj);
		break;
	}

	//change labeling of render object
	obj->set_r_type(new_type);

	//finish
	obj->set_index(newid);

	obj->update_indices();
}

/*!***********************************************************************
\brief
 change_object_state is a way for the CPRender object to tell it's renderer
 that they either do not want to or want to be shown. This is achieved by
 changing the indices in the index buffer so the CPRender object quad is not
 rendered.

\param r_type
 The type of render object

\param obj
 Poitner to a render object that was just created
*************************************************************************/
inline void Renderer::RenderSystem::change_object_state(Renderer_Types r_type, const LB::CPRender* obj)
{
	switch (r_type) {
	case Renderer_Types::RT_OBJECT:
		object_renderer.change_render_state(*obj);
		break;
	case Renderer_Types::RT_BACKGROUND:
		bg_renderer.change_render_state(*obj);
		break;
	case Renderer_Types::RT_UI:
		ui_renderer.change_render_state(*obj);
		break;
	case Renderer_Types::RT_MENU:
		menu_renderer.change_render_state(*obj);
		break;
	}
}

/*!***********************************************************************
\brief
 update_cam will update the editor free cam for the scene view.

\param xpos
 The amount to move the camera by on the x-axis. +ve for right -ve for left
\param ypos
 The amount to move the camera by on the y-axis. +ve for up -ve for down
*************************************************************************/
void Renderer::RenderSystem::update_cam(float xpos, float ypos)
{
	cam.free_cam_move(LB::Vec2<float>{xpos, ypos});
}

/*!***********************************************************************
\brief
 fcam_zoom will change the size of the projection matrix that the free cam
 uses. This will give the effect of zooming in and out of a location.

\param amount
 The change in zoom in percentage. 1.f means no zoom and 0.5f means zoom in
 by half
*************************************************************************/
void Renderer::RenderSystem::fcam_zoom(float zoom)
{
	cam.free_cam_zoom(zoom);
}

/*!***********************************************************************
\brief
 Destroy will act as the destructor for render system and deallocate
 The shader and call the individual renderers destroy functions.

 NOTE: Anything you want to use in the destructor should be used here
 instead.
*************************************************************************/
void Renderer::RenderSystem::Destroy()
{

}

/*!***********************************************************************
\brief
 Returns the list of objects inside the specified rendering layer

 NOTE: Will return object renderer object list by default

\param layer
 The rendering layer we want to access
*************************************************************************/
const std::list<const LB::CPRender*>& Renderer::RenderSystem::get_layer_objs(Renderer_Types layer) const
{
	switch (layer) {
	case Renderer_Types::RT_OBJECT:
		return object_renderer.getObjectList();
	case Renderer_Types::RT_BACKGROUND:
		return bg_renderer.getObjectList();
	case Renderer_Types::RT_UI:
		return ui_renderer.getObjectList();
	case Renderer_Types::RT_MENU:
		return menu_renderer.getObjectList();
	}
	return object_renderer.getObjectList();
}
/*!***********************************************************************
\brief
 Performs a check on whether the rendering layer is active or not

\param layer
 The rendering layer we want to check

\return bool
 True - if rendering layer is active, False - if not
 NOTE: return object layer by default
*************************************************************************/
const bool& Renderer::RenderSystem::get_layer_active(Renderer_Types layer) const
{
	switch (layer) {
	case Renderer_Types::RT_OBJECT:
		return object_renderer.getActive();
	case Renderer_Types::RT_BACKGROUND:
		return bg_renderer.getActive();
	case Renderer_Types::RT_UI:
		return ui_renderer.getActive();
	case Renderer_Types::RT_MENU:
		return menu_renderer.getActive();
	}

	return object_renderer.getActive();
}

/*!***********************************************************************
 \brief
 Frees all resources allocated by Render system
*************************************************************************/
Renderer::RenderSystem::~RenderSystem()
{
	if (test2)
		LB::Memory::Instance()->Deallocate(test2);

	if (button)
		delete button;

	if (text)
		delete text;

	if (text2)
		delete text2;

	if (backgrounds.size())
	{
		for (LB::CPRender* renderComp : backgrounds)
		{
			LB::Memory::Instance()->Deallocate(renderComp);
		}
		backgrounds.clear();
	}

	object_renderer.Destroy_Renderer();
	bg_renderer.Destroy_Renderer();
	ui_renderer.Destroy_Renderer();
	menu_renderer.Destroy_Renderer();
	text_renderer.Destroy_TextRend();

	glDeleteFramebuffers(1, &framebuffer);
	glDeleteFramebuffers(1, &svfb);
	glDeleteTextures(1, &textureColorbuffer);
	glDeleteTextures(1, &svtcb);

	glDeleteProgram(shader_program);
}

//----------------------------------------------RENDERER-SYSTEM-------------------------------------------


//----------------------------------------------TEXTURES--------------------------------------------
/*!***********************************************************************
\brief
 Texture class destructor
*************************************************************************/
Renderer::Texture::~Texture()
{
	//std::cout << "DELETED TEXTURE " << id << std::endl;
	glDeleteTextures(1, &id);
}

//----------------------------------------------DEPRECATED-------------------------------------------
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
//bool Renderer::Texture_Manager::add_texture(const std::string& file_path, const std::string& name)
//{
//	file_path;
//	name;
//	//LB::ASSETMANAGER->AddTexture(file_path, name);
//
//	GLint uni_loc = glGetUniformLocation(GRAPHICS->get_shader(), "u_SamplerID");
//	int test[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
//	glUniform1iv(uni_loc, 13, test);
//
//	return true;
//}
//----------------------------------------------DEPRECATED-------------------------------------------


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


//----------------------------------------------TEXT--------------------------------------------
/*!***********************************************************************
\brief
 Initialises the text object component by adding it to the active list of
 text objects to render.
*************************************************************************/
void LB::CPText::Initialise()
{
	Renderer::GRAPHICS->render_msg(this);
}

/*!***********************************************************************
\brief
 Removes the text object comopnent from active list in text object renderer
*************************************************************************/
void LB::CPText::Destroy()
{
	Renderer::GRAPHICS->remove_msg(this);
}

/*!***********************************************************************
\brief
 Update will move the text object component based on the game objects
 transform.

 NOTE: currently using self-defined positions for UI right now
*************************************************************************/
void LB::CPText::Update()
{
	LB::Vec2<float> pos = GetComponent<CPTransform>()->GetPosition();
	update_msg_pos(pos); // Because message pos is not a vector 2

	msg.transScale = GetComponent<CPTransform>()->GetScale().x;

	msg.finalScale = msg.scale * msg.transScale;

}

/*!***********************************************************************
 \brief
 Serializes all the information needed for Text rendering
*************************************************************************/
bool LB::CPText::Serialize(Value& data, Document::AllocatorType& alloc)
{
	DebuggerLog("Serializing Text");
	data.SetObject();
	data.AddMember("Active", m_active, alloc);
	Value messageValue;
	if (msg.Serialize(messageValue, alloc))
	{
		data.AddMember("Message", messageValue, alloc);
		return true;
	}
	else return false;
}

/*!***********************************************************************
 \brief
 Deserializes all the information needed for Text rendering
*************************************************************************/
bool LB::CPText::Deserialize(const Value& data)
{
	bool HasActive = data.HasMember("Active");
	bool HasMessage = data.HasMember("Message");
	if (HasActive)
	{
		m_active = data["Active"].GetBool();
	}
	if (HasMessage)
	{
		const Value& messageValue = data["Message"];
		msg.Deserialize(messageValue);
	}
	return true;
}

/*!***********************************************************************
\brief
 Updates the text to be printed on the screen

\param str
 The text to be rendered
*************************************************************************/
void LB::CPText::update_msg_text(const std::string& str)
{
	msg.text = str;
}

/*!***********************************************************************
\brief
 Updates the font color of the text

 NOTE: format is RGB and values go from 0 - 1.f

\param col
 The values of the new color in vector format
*************************************************************************/
void LB::CPText::update_msg_color(const LB::Vec3<float>& col)
{
	msg.color = col;
}

/*!***********************************************************************
\brief
 Updates the font that will be used to print the text object message

 NOTE: The font will be loaded as the file name of the font without the
 extension. E.g. "Ariel.ttf" file, the font name will be "Ariel"

\param file_name_wo_ext
 Font file name without the extension
*************************************************************************/
void LB::CPText::update_msg_font(const std::string& file_name_wo_ext)
{
	msg.font_file_name_wo_ext = file_name_wo_ext;
}

/*!***********************************************************************
\brief
 Updates the bounding box of the text component

 NOTE: Text should follow camera size. Therefore, left is 0 and right is
 1920

\param new_bound
 The new size of the bounding box for the text
*************************************************************************/
void LB::CPText::update_msg_xbound(float new_bound)
{
	msg.xbound = new_bound;
}

/*!***********************************************************************
\brief
 Getter method to get the string text of the message/CPText object
*************************************************************************/
inline const std::string& LB::CPText::get_msg_text() const
{
	// TODO: insert return statement here
	return msg.text;
}

/*!***********************************************************************
\brief
 Getter method to get the font color of the CPText object in Vec3 format
 RGB from 0 - 1.f
*************************************************************************/
inline const LB::Vec3<float>& LB::CPText::get_msg_color() const
{
	// TODO: insert return statement here
	return msg.color;
}

/*!***********************************************************************
\brief
 Getter method to get the scaling value of the CPText object
*************************************************************************/
inline const float& LB::CPText::get_msg_size() const
{
	// TODO: insert return statement here
	return msg.scale;
}

/*!***********************************************************************
\brief
 Getter method to get the string text of the font used by the CPText object
*************************************************************************/
inline const std::string& LB::CPText::get_msg_font() const
{
	// TODO: insert return statement here
	return msg.font_file_name_wo_ext;
}

/*!***********************************************************************
\brief
 Setter method to update the type of coordinate system we want to use
*************************************************************************/
void LB::CPText::use_world_coords(bool use_world)
{
	msg.use_world_space = use_world;
}

/*!***********************************************************************
\brief
 Getter method to figure out what type of coordinate system we want to use

\return
 returns the boolean
*************************************************************************/
bool LB::CPText::get_coord_sys()
{
	return msg.use_world_space;
}

/*!***********************************************************************
\brief
 Getter method to get the entire message object in CPText object that
 contains all necessary information for the text to be printed
*************************************************************************/
//inline Renderer::message& LB::CPText::get_msg()
//{
//	// TODO: insert return statement here
//	return msg;
//}

//----------------------------------------------TEXTURES--------------------------------------------