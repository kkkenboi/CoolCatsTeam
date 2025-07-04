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
#include <queue>
#include <map>
#include <glm.hpp>
#include "LitterBox/Serialization/AssetManager.h"
#include <ft2build.h>
#include FT_FREETYPE_H

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
		LB::Vec3<float> pos{ 0.5f, 0.5f, 1.f };
		LB::Vec2<float> tex{ 0.f,0.f };
		LB::Vec3<float> color{ 1.f,0.f,0.f };
		float texIndex{ -1.f };
		LB::Vec3<float> widHeightType{ 0.f, 0.f, 0.f };
		LB::Vec3<float> others {0.f, 0.f, 0.f }; //width, height, scale x, scale y, rotation
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
		unsigned int furthest_index{ 0 };
		bool active;

		std::list<const LB::CPRender*> active_objs;
		//TODO figure out parsing in shader file paths
		//std::string shader_file_path

	public:
		/*!***********************************************************************
		\brief
		 Renderer constructor

		\param renderer
		 The type of renderer you want this renderer to be.
		*************************************************************************/
		Renderer(const Renderer_Types& renderer);

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
		inline size_t get_furthest_index() { return static_cast<size_t>(furthest_index) + 1; }
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
		*************************************************************************/
		void update_buff();
		/*!***********************************************************************
		\brief
		 update_anim is the function that will update all render object animations
		 if the object has any.
		*************************************************************************/
		void update_anim();
		/*!***********************************************************************
		\brief
		 change_render_state will change the indices in the index buffer so that
		 a particular will or will not be shown.

		\param object
		 The CPRender object which you want to either show or hide.
		*************************************************************************/
		void change_render_state(const LB::CPRender& object);

		/*!***********************************************************************
		\brief
		 Destroy_Renderer deallocates all memory on the freestore that the
		 Renderer uses. The function also releases the vertex array, index buffer
		 and vertex buffer back to the GPU.

		 NOTE: done here instead of destructor because destructor is not getting
		 invoked.
		*************************************************************************/
		void Destroy_Renderer();

		/*!***********************************************************************
		\brief
		 Getter function to return the state of the Rendering layer.
		*************************************************************************/
		inline const bool& getActive() const { return active; }

		/*!***********************************************************************
		\brief
		 Setter function to toggle the active state of the Renderering Layer
		*************************************************************************/
		void toggleActive() { active = !active; }

		/*!***********************************************************************
		\brief
		 Setter function to set the active state of the Renderering Layer
		*************************************************************************/
		void setActive(bool activity) { active = activity; }

		/*!***********************************************************************
		\brief
		 getObjectList will return a const reference to the list of active objects
		 in the renderer

		\return
		 List of active CPRender objects in the renderer layer
		*************************************************************************/
		 const std::list<const LB::CPRender*>& getObjectList() const { return active_objs; }
	};

	/*!***********************************************************************
	\brief
	 Character is a structure containing information of each character glyph
	 in a font include how far to the right the next character will be.
	*************************************************************************/
	struct Character {
		unsigned int TextureID{};
		LB::Vec2<unsigned int> Size{};
		LB::Vec2<FT_Int> Bearing{};
		unsigned int Advance{};
		unsigned int font{};
	};

	/*!***********************************************************************
	\brief
	 TextRenderer class handles the loading of fonts, character glyph informations
	 and text printing on the screen.

	 NOTE: the text only prints on the game view and not the scene view
	*************************************************************************/
	class TextRenderer {
	private:
		//map of character glyph information with the key being the character
		//std::map<char, Character> Characters;
		//map of character glyphs for different fonts
		std::map<std::string, std::map<char, Character>> font_glyphs;
		
		std::map<std::string, unsigned int> font_height_adv;

		//openGL handles
		unsigned int tShader, tVao, tVbo;

		//text max bound before wrapping
		float maxBound;

		//Necessary freetype variables to load font
		//FT_Library ft;
		//FT_Face font;

		//list to store all text components
		std::list<LB::CPText*> active_msgs;
		
		//list of fonts to load
		//std::vector<std::string> font_files;
	public:

		/*!***********************************************************************
		\brief
		 Constructor will load the freetype library and font then store all the
		 character glpyh information the in the map member followed by freeing
		 of the freetype library.
		*************************************************************************/
		TextRenderer();

		/*!***********************************************************************
		\brief
		 RenderText loads a message, loops through each character in the message and
		 renders the appropriate character in openGL using the character glyph
		 information.

		\param msg
		 The message with the string and text you want to render in openGL
		*************************************************************************/
		void RenderText(message& msg);

		/*!***********************************************************************
		\brief
		 Destroy_TextRend will free all the data on the OpenGL side back to the
		 GPU.
		*************************************************************************/
		void Destroy_TextRend();

		/*!***********************************************************************
		\brief
		 Getter method to get the handle to the shader program for the text renderer.
		*************************************************************************/
		inline const unsigned int get_text_shader() const { return tShader; }

		/*!***********************************************************************
		\brief
		 Adds pointer to a text component into the active object list.

		\param obj
		 Pointer to the text object component
		*************************************************************************/
		inline void add_text_component(LB::CPText* obj) { if (obj) active_msgs.emplace_back(obj); }

		/*!***********************************************************************
		\brief
		 Removes pointer to a text component from the active object list.

		\param obj
		 Pointer to the text object component
		*************************************************************************/
		inline void remove_text_component(LB::CPText* obj) { if(obj) active_msgs.remove(obj); }

		/*!***********************************************************************
		\brief
		 Used by text renderer to update the text information
		*************************************************************************/
		void update_text();

		/*!***********************************************************************
		\brief
		 Getter method to get the map of loaded fonts in the renderer
		*************************************************************************/
		std::map<std::string, std::map<char, Character>>&
			get_font_map() { return font_glyphs; }

		std::map<std::string, unsigned int>&
			get_heights_map() { return font_height_adv; }

		unsigned int& get_vertex_array() { return tVao; }
		unsigned int& get_vertex_buffer() { return tVbo; }
		unsigned int& get_shader() { return tShader; }
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

		//-------for the game view-------------
		unsigned int framebuffer;
		unsigned int textureColorbuffer;
		//-------for the game view-------------

		//-------for depth testing-------
		unsigned int rbo;
		//-------for depth testing-------

		//-------for scene view--------
		unsigned int svfb;
		unsigned int svtcb;
		//-------for scene view--------

		Texture_Manager t_Manager;
		Animation_Manager a_Manager;

		Renderer bg_renderer;
		Renderer object_renderer;
		Renderer ui_renderer;
		Renderer menu_renderer;
		TextRenderer text_renderer;

		Camera cam;
		LB::Vec2<GLint> m_winPos;
		LB::Vec2<GLsizei> m_winSize;

		std::queue<message> msgs;

		bool editor_mode{ false };
		//Camera shake stuff
		bool isShake{ false };
		float shakeDuration{};
		float shakeIntensity{ 100.f };

	public:
		/*!***********************************************************************
		\brief
		 RenderSystem constructor
		*************************************************************************/
		RenderSystem();

		/*!***********************************************************************
		\brief
		 The update function that gets called every loop also in charge of 
		 drawing everything. Counterintuitive, I know.
		*************************************************************************/
		void Update() override;

		/*!***********************************************************************
		\brief
		 Updates the game viewport position and size based on the current ImGUI
		 window.
		*************************************************************************/
		void UpdateGameWindowPos(GLint winPosX, GLint winPosY, GLsizei winSizeX, GLsizei winSizeY);

		/*!***********************************************************************
		\brief
		 The FixedUpdate function is a time based update function that will only
		 be called after a set amount of time is passed or will be called multiple
		 times if application runs to slowly.

		 NOTE: For rendering context FixedUpdate is used for consistent aniamtion
		 regardless of framerate.
		*************************************************************************/
		void FixedUpdate() override;
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
		void Initialize() override;

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
		 swap_object_type is a function that allows the player to change a render
		 object's layer.

		 NOTE: Not recommended for making objects appear behind or infront of one
		 another as there is too much overhead. Use the z-axis for that.

		\param curr_type
		 The layer that the object is currently on.

		\param new_type
		 The new layer you want the object to be on.

		 \param obj
		 Poitner to the render object that you want to alter
		*************************************************************************/
		void swap_object_type(Renderer_Types new_type, LB::CPRender* obj);

		/*!***********************************************************************
		\brief
		 get_shader returns the handle to the shader program loaded into GPU

		\return
		 Handle to the shader program of the render system in GPU.
		*************************************************************************/
		inline unsigned int get_shader() { return shader_program; }
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
		inline void change_object_state(Renderer_Types r_type, const LB::CPRender* obj);

		/*!***********************************************************************
		\brief
		 render_msg adds a pointer to a new text component into text renderer.

		\param obj
		 The text component to be rendered
		*************************************************************************/
		void render_msg(LB::CPText* obj);

		/*!***********************************************************************
		\brief
		 remove_msg removes a pointer to a text component from the text renderer.

		\param obj
		 The text component to be removed
		*************************************************************************/
		inline void remove_msg(LB::CPText* obj) { text_renderer.remove_text_component(obj); }

		/*!***********************************************************************
		\brief
		 update_cam will update the editor free cam for the scene view.

		\param xpos
		 The amount to move the camera by on the x-axis. +ve for right -ve for left
		\param ypos
		 The amount to move the camera by on the y-axis. +ve for up -ve for down
		*************************************************************************/
		void update_cam(float xpos, float ypos);

		/*!***********************************************************************
		\brief
		 fcam_zoom will change the size of the projection matrix that the free cam
		 uses. This will give the effect of zooming in and out of a location.

		\param amount
		 The change in zoom in percentage. 1.f means no zoom and 0.5f means zoom in
		 by half
		*************************************************************************/
		void fcam_zoom(float amount); //1.f means no zoom

		/*!***********************************************************************
		\brief
		 get_cam_mat is a getter function that will return the matrix of the free
		 cam of the scene view

		\return
		 returns the 4x4 matrix of the freecam editor
		*************************************************************************/
		inline auto get_cam_mat() { return cam.editor_world_NDC; }

		/*!***********************************************************************
		\brief
		 get_game_cam_mat is a getter function that will return the matrix of the free
		 cam of the scene view

		\return
		 returns the 4x4 matrix of the freecam editor
		*************************************************************************/
		inline auto get_game_cam_mat() { return cam.world_NDC; }

		/*!***********************************************************************
		\brief
		 get_game_cam_mat is a getter function that will return the a matrix for
		 screen space.

		\return
		 returns the 4x4 matrix of the screen
		*************************************************************************/
		inline auto get_ui_cam_mat() { return cam.ui_NDC; }

		/*!***********************************************************************
		\brief
		 get_cam_proj_mat is a getter function that will return the matrix of the
		 orthographic projection used for the game.

		\return
		 returns the 4x4 matrix of the game ortho projection
		*************************************************************************/
		inline auto get_cam_proj_mat() { return cam.ortho; }

		/*!***********************************************************************
		\brief
		 get_cam is a getter function that will return the camera object with all
		 the relevant matrices needed for a camera

		\return
		 returns the camera object
		*************************************************************************/
		Camera* get_cam() { return &cam; }

		/*!***********************************************************************
		\brief 
		 Destroy will act as the destructor for render system and deallocate
		 The shader and call the individual renderers destroy functions.

		 NOTE: Anything you want to use in the destructor should be used here
		 instead.
		*************************************************************************/
		void Destroy() override;

		/*!***********************************************************************
		 \brief
		 Frees all resources allocated by Render system
		*************************************************************************/
		~RenderSystem() override;

		/*!***********************************************************************
		 \brief
		 Toggles the window for rendering, in editor mode, will render to the 
		 Gameview instead.
		*************************************************************************/
		void turnOnEditorMode();

		/*!***********************************************************************
		 \brief
		 get_game_view returns the ID of the texture that the game view is
		 written to.

		\return
		 texture ID to display the game view
		*************************************************************************/
		inline unsigned int get_game_view() { return editor_mode ? textureColorbuffer : -1; }
		/*!***********************************************************************
		 \brief
		 get_scene_view returns the ID of the texture that the scene view is
		 written to.

		\return
		 Texture ID to display the scene view
		*************************************************************************/
		inline unsigned int get_scene_view() { return editor_mode ? svtcb : -1; }
		/*!***********************************************************************
		 \brief
		 get_sv_framebuff returns the ID of the framebuffer that the game view is
		 written to.

		\return
		 FrameBuffer ID of the scene view frame buffer
		*************************************************************************/
		inline unsigned int get_sv_framebuff() { return editor_mode ? svfb : -1; }
	
		/*!***********************************************************************
		\brief
		 get_layer_objs() gets the list of active objects in a specified layer

		\return
		 returns the list of active objects in a specified layer
		*************************************************************************/
		const std::list<const LB::CPRender*>& get_layer_objs(Renderer_Types layer) const;
	
		/*!***********************************************************************
		\brief
		 get_layer_active() gets the active state of a specified layer

		\return
		 returns the list of active objects in a specified layer
		*************************************************************************/
		const bool& get_layer_active(Renderer_Types layer) const;

		/*!***********************************************************************
		\brief
		 shaker_camera is a function that moves the camera slightly to a random
		 position.

		 To make the camera shake just call this each frame.

		\param
		 intensity - float value that determins the range of random position.

		\return
		 returns the list of active objects in a specified layer
		*************************************************************************/
		void shake_camera(float intsenity = 100.f,float duration = 1.f)
		{
			shakeIntensity = intsenity;
			shakeDuration = duration;
			isShake = true;
		}

		/*!***********************************************************************
		\brief
		 reset_cam will reset the cam to the origin position.

		 Honestly, can be done with shaker_cam(0.f).
		 NOTE: If the camera has moved to a different position, then reset_cam will
		 not move it back.

		\return
		 returns the list of active objects in a specified layer
		*************************************************************************/
		void reset_cam()
		{
			cam.move_cam(0.f);
		}
	};

	//A pointer to the system object in the core engine
	//made to be singleton
	extern RenderSystem* GRAPHICS;
	//------------------------------------------------RENDERING SPECIFIC------------------------------------------------
}