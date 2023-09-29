/*!************************************************************************
 \file                Renderer.h
 \author(s)           Ryan Tan Jian Hao | Amadeus Chia Jinhan
 \par DP email(s):    ryanjianhao.tan\@digipen.edu | amadeusjinhan.chia\@digipen.edu
 \par Course:         CSD2401A
 \date                28-09-2023
 \brief

 Header file that contains helper function declerations for Rendering as
 well as the classes required for rendering:
	- Vertex
	- quad
	- Texture
	- Texture_Manager
	- Renderer
	- RenderSystem

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "LitterBox/Core/System.h"
#include "Platform\Windows\Windows.h"
#include "Camera.h"
#include "ForwardDeclerators.h"
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Factory/Components.h"
#include <utility>
#include <array>
#include <list>
#include <map>
#include <glm.hpp>
#include "LitterBox/Serialization/AssetManager.h"

//-----------------------------------------HELPER FUNCTIONS--------------------------------
/*!***********************************************************************
\brief
 Struct to store the text of the vertex and fragment shader. For the GPU to
 compile.
*************************************************************************/
struct shader_source {
	std::string vtx_shd;
	std::string frg_shd;
};
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
shader_source shader_parser(const char* shader_file_name);
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
unsigned int compile_shader(const char*& source, unsigned int type);
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
unsigned int create_shader(const char* vertex_shader, const char* fragment_shader);
//-----------------------------------------HELPER FUNCTIONS--------------------------------

namespace Renderer {
	//-----------------------Vertex data------------------------
	/*!***********************************************************************
	\brief
	 Vertex and all the necessary data required to start drawing something on
	 the screen.
	*************************************************************************/
	struct Vertex {
		LB::Vec2<float> pos{ 0.5f, 0.5f };
		LB::Vec2<float> tex{ 0.f,0.f };
		LB::Vec3<float> color{ 1.f,0.f,0.f };
		float texIndex{ -1.f };
		unsigned int index{ 0 };
		bool active{ false };
	};

	/*!***********************************************************************
	\brief
	 Struct that holds array of 4 vertices because everything in the game is
	 basically gonna be a quad.
	*************************************************************************/
	struct quad {
		std::array<Vertex, 4> data;
	};

	//NOTE: because index data type is unsigned short
	//the maximum number of vectors allowed is 65535
	//therefore maximum number of quads allowed is 16383

	/*!***********************************************************************
	\brief
	 Struct of the indices required to draw one quad.
	*************************************************************************/
	struct index {
		std::array<unsigned short, 6> indexes;
	};
	//-----------------------Vertex data------------------------

	//---------------------TEXTURES-------------------------------------------

	/*!***********************************************************************
	\brief
	 Texture class that holds all the necessary information to draw an image
	 on the screen.
	*************************************************************************/
	class Texture {
	private:
		//handler to the texture image data in the GPU
		unsigned int id;
		//image file path
		std::string file_path;
		//local buffer to store the image data when loading
		unsigned char* local_buff;
		//variables to store additional info of the images
		int w, h, fluff;

	public:
		/*!***********************************************************************
		\brief
		 Constructor for the texture object
		\param path
		 The file path of the texture image
		*************************************************************************/
		Texture(const std::string& path);
		/*!***********************************************************************
		\brief
		 Texture class destructor
		*************************************************************************/
		~Texture();

		/*!***********************************************************************
		\brief
		 Function to get the handle to the image data in GPU
		*************************************************************************/
		const unsigned int get_tex() const { return id; }
	};

	//TODO refactor the limits of the textures when possible
	/*!***********************************************************************
	\brief
	 class to manage all the textures loaded into GPU.

	 NOTE: Basically a skin-suit function as most of its insides have been
	 scooped out and put into AssetManager.
	*************************************************************************/
	class Texture_Manager {
	private:
		bool free[32];
		std::map<std::string, std::pair<const Texture*, int>> textures;
	public:
		//initialize every variable in free to false: IMPORTANT
		/*!***********************************************************************
		\brief
		 Texture_Manager constructor. 

		 FUN-FACT: even though I put the comment above this header, someone still
		 messed it up and forgot to initialize everything to false. :(
		*************************************************************************/
		Texture_Manager() : free{ false } {}
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
		bool add_texture(const std::string& file_path, const std::string& name);
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
		bool remove_texture(const std::string& name);
		/*!***********************************************************************
		\brief
		 get_texture_index gets the texture slot that a specified texture is bound
		 to.

		 NOTE: I intended to comment that this function was pretty obvious but
		 I realized it wasn't and that's because I'm a bad programmer. :o

		\param name
		 The key in the form of a std::string to find the Texture object and its
		 relevant data

		\return
		 The texture slot that the specified texture is bound to.
		*************************************************************************/
		const int get_texture_index(const std::string& name) const { return textures.find(name)->second.second; }

		/*!***********************************************************************
		\brief
		 flush_textures frees all Texture data from the GPU. Just like the posb
		 advertisements: So Simple! :D
		*************************************************************************/
		void flush_textures();
	};
	//------------------------------------------------------------------------
	
	//------------------------------------------------RENDERING SPECIFIC------------------------------------------------
	//Renderer class will be incharge the vao and buffers.
	//Renderer class will not be exposed to the programmers and is meant-
	//-to have a level of disconnect even from render_object class

	/*!***********************************************************************
	\brief
	 Renderer is a class that contains a vertex buffer object handle, a vertex
	 array handle and an index array handle. And a set of contiguous memory
	 to be loaded into the GPU. Also incharge of keeping track of how many
	 render objects are alive.
	*************************************************************************/
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

		/*!***********************************************************************
		\brief
		 Renderer constructor

		\param renderer
		 The type of renderer you want this renderer to be.
		*************************************************************************/
		Renderer(const Renderer_Types& renderer);
		/*!***********************************************************************
		\brief
		 Destructor for the Renderer object
		*************************************************************************/
		~Renderer();

		/*!***********************************************************************
		\brief
		 Gets handle to vertex array object in GPU
		*************************************************************************/
		inline unsigned int get_vao() { return vao; }
		/*!***********************************************************************
		\brief
		 Gets handle to index array object in GPU
		*************************************************************************/
		inline unsigned int get_ibo() { return ibo; }
		/*!***********************************************************************
		\brief
		 Gets number of active render objects
		*************************************************************************/
		inline size_t get_ao_size() { return active_objs.size(); }
		/*!***********************************************************************
		\brief
		 Gets handle to vertex buffer object in GPU
		*************************************************************************/
		inline unsigned int get_vbo() { return vbo; }

		/*!***********************************************************************
		\brief
		 create_render_object stores a pointer to render object
		 into active_objs list. I don't know what else to say. OOOONGA BOONGA

		\param obj
		 Pointer to a render object to be added to List
		*************************************************************************/
		unsigned int create_render_object(const LB::CPRender* obj);
		/*!***********************************************************************
		\brief
		 remove_render_object removes a render object pointer from its active
		 object list. Shock horror.

		\param obj
		 Pointer to a render object to be removed to List
		*************************************************************************/
		void remove_render_object(const LB::CPRender* obj);
		/*!***********************************************************************
		\brief
		 update_buff is the function that will update all render object values
		 and then pass the value to GPU

		\param r_type
		 Type of render object
		*************************************************************************/
		void update_buff(Renderer_Types r_type);
	};

	//The actual system that will get initialized into the engine
	/*!***********************************************************************
	\brief
	 RenderSystem is the system in the Engine incharge of all things rendering.
	*************************************************************************/
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

		/*!***********************************************************************
		\brief
		 RenderSystem constructor
		*************************************************************************/
		RenderSystem();
		/*!***********************************************************************
		\brief
		 RenderSystem destructor
		*************************************************************************/
		~RenderSystem();

		/*!***********************************************************************
		\brief
		 The update function that gets called every loop also in charge of 
		 drawing everything. Counterintuitive, I know.
		*************************************************************************/
		void Update() override;
		/*!***********************************************************************
		\brief
		 Draw function does nothing for now
		*************************************************************************/
		void Draw();
		/*!***********************************************************************
		\brief
		 Initialize function from base class ISystem. 
		 Does cool whacky stuff. Or not.
		*************************************************************************/
		void Initialize() override { SetSystemName("Renderer System"); }

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
		bool create_texture(const std::string& file_path, const std::string& name);
		/*!***********************************************************************
		\brief
		 remove_texture is another API to delete texture data from the GPU.

		\param name
		 The key assigned to the Texture object

		\return
		 true if texture is successfully removed from GPU and false if not.
		*************************************************************************/
		bool remove_texture(const std::string& name);
		/*!***********************************************************************
		\brief
		 get_texture returns the texture slot that the specified texture data is 
		 bound to.

		\param name
		 The key assigned to the Texture object

		\return
		 the texture slot that the specified texture data is bound to.
		*************************************************************************/
		const int get_texture(const std::string& name) const { return t_Manager.get_texture_index(name); }
		/*!***********************************************************************
		\brief
		 flush_textures is another API to delete all texture data from the GPU.
		*************************************************************************/
		void flush_textures();

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
		void init_anim(const std::string& animation_name, const std::array<LB::Vec2<float>, 4>* data, const float anim_time, const int number_of_frames) {
			a_Manager.load_anim(animation_name, data, anim_time, number_of_frames);
		}
		/*!***********************************************************************
		\brief
		 get_anim returns the animation data of a specified animation.

		\param name
		 Key that is mapped to the animation data
		*************************************************************************/
		auto get_anim(const std::string& name) const { return a_Manager.find_animation(name); }

		/*!***********************************************************************
		\brief
		 create_object is a way to create a render object with a specific render
		 type that determines which renderer the render object would belong to.

		\param r_type
		 The type of render object

		\param obj
		 Poitner to a render object that was just created
		*************************************************************************/
		unsigned int create_object(Renderer_Types r_type, const LB::CPRender* obj);
		/*!***********************************************************************
		\brief
		 remove_object is a way to remove a render object from a renderer the render 
		 object would belongs to.

		\param r_type
		 The type of render object

		\param obj
		 Poitner to a render object that was just created
		*************************************************************************/
		void remove_object(Renderer_Types r_type, const LB::CPRender* obj);

		/*!***********************************************************************
		\brief
		 get_shader returns the handle to the shader program loaded into GPU

		\return
		 Handle to the shader program of the render system in GPU.
		*************************************************************************/
		inline unsigned int get_shader() { return shader_program; }
	};

	//A pointer to the system object in the core engine
	//made to be singleton
	extern RenderSystem* GRAPHICS;
	//------------------------------------------------RENDERING SPECIFIC------------------------------------------------
}