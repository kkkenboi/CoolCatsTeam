/*!************************************************************************
 \file				DebugDraw.cpp
 \author(s)			Ryan Tan Jian Hao
 \par DP email(s):	ryanjianhao.tan\@digipen.edu
 \par Course:		CSD2401A
 \date				18/09/23
 \brief

 This file contains functions to draw debug information onto the screen,
 boxes and lines (mainly used for physics).

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Renderer/Renderer.h"
#include "Debug.h"

extern Renderer::RenderSystem* Renderer::GRAPHICS;
extern bool imgui_ready;

namespace LB
{
	//-----------------Pre-defines------------------------------
	constexpr int CIRCLE_LINES{ 20 };
	constexpr float INCREMENT{ 2.f * (float)PI / (float)CIRCLE_LINES };

	float wid_div;		// { 1.f / (LB::WINDOWSSYSTEM->GetWidth() * 0.5f) };
	float height_div;	// { 1.f / (LB::WINDOWSSYSTEM->GetHeight() * 0.5f) };
	//-----------------Pre-defines------------------------------

	/*!***********************************************************************
	 \brief
	 Vertex required for a line end point
	*************************************************************************/
	struct debug_vertex 
	{
		Vec2<float> pos;
		Vec4<float> col;
	};

	//TODO modulate the vertex size
	//vertex size should = min 3000 x 4 (number of quads in render object)
	/*!***********************************************************************
	 \brief
	 Debugger class Initializer that loads all the VAO, VBO and IBO necessary
	 to start drawing things on the screen.
	*************************************************************************/
	void Debugger::InitializeDrawer()
	{
		//--------------------Debugging input registering---------------------
		INPUT->SubscribeToKey(ToggleDebugOn, m_debugToggleKey, KeyEvent::TRIGGERED, KeyTriggerType::NONPAUSABLE);
		INPUT->SubscribeToKey(LB::StepPhysics, m_stepPhysicsKey, KeyEvent::TRIGGERED, KeyTriggerType::NONPAUSABLE);

		//--------------------Drawing Setup---------------------
		wid_div = { 1.f / (LB::WINDOWSSYSTEM->GetWidth() * 0.5f) };
		height_div = { 1.f / (LB::WINDOWSSYSTEM->GetHeight() * 0.5f) };

		//assume we have one index per vertex
		idx.resize(12000);

		glCreateBuffers(1, &vbo);
		glNamedBufferStorage(vbo,
			sizeof(debug_vertex) * 12000,
			nullptr,
			GL_DYNAMIC_STORAGE_BIT);

		//create vao
		glCreateVertexArrays(1, &vao);
		//attribute 0 will be position data
		glEnableVertexArrayAttrib(vao, 0);
		glVertexArrayVertexBuffer(vao, 1, vbo, 0, sizeof(debug_vertex));
		glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vao, 0, 1);
		//color coordinates
		glEnableVertexArrayAttrib(vao, 2);
		glVertexArrayVertexBuffer(vao, 3, vbo, sizeof(Vec2<float>), sizeof(debug_vertex));
		glVertexArrayAttribFormat(vao, 2, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vao, 2, 3);

		glCreateBuffers(1, &ibo);
		glNamedBufferStorage(ibo, idx.capacity() * sizeof(unsigned short),
			nullptr, GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayElementBuffer(vao, ibo);

		shader_source shd_pgm{ shader_parser("Assets/Shaders/debug.shader") };
		shader = create_shader(shd_pgm.vtx_shd.c_str(), shd_pgm.frg_shd.c_str());

		glLineWidth(5.f);
	}

	/*!***********************************************************************
	 \brief
	 The system's update function which sends compiles the data and sends
	 the information to the GPU to get drawn
	*************************************************************************/
	void Debugger::UpdateDrawer()
	{
		//-----------------Debug Rendering------------------
		size_t index{ 0 };
		while (drawobj.size()) {
			//loop through object to 
			LineUpdate(drawobj.top(), index);
			index += 2;
			drawobj.pop();
		}

		//pass index data inside
		glNamedBufferSubData(ibo, 0, index * sizeof(unsigned short), idx.data());

		//draw lines to the imgui renderered box
		//Bind the frame buffer to the texture image
		if (imgui_ready)
			glBindFramebuffer(GL_FRAMEBUFFER, Renderer::GRAPHICS->get_sv_framebuff());

		glUseProgram(shader);
		glBindVertexArray(vao);

		glDrawElements(GL_LINES, (GLsizei)index, GL_UNSIGNED_SHORT, nullptr);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//we don't change back because we let graphics system handle the rest
	}

	/*!***********************************************************************
	 \brief
	 DrawBox function loads 4 lines to be drawn to represent a box with a
	 rotation defined in radians turning counter clockwise.
	*************************************************************************/
	void Debugger::DrawBox(Vec2<float> center, float width, float height, Vec4<float> color, float rot)
	{
		if (!m_debugModeOn) return;
		//rotation matrix
		glm::mat4 rotation{
			cosf(rot), sinf(rot), 0.f, 0.f,
				-sinf(rot), cosf(rot), 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f
		};
		//matrix to convert the world coord to NDC
		glm::mat4 translate{1.f};
		translate[3][0] = center.x;
		translate[3][1] = center.y;

		glm::mat4 scale{ 1.f };
		scale[0][0] = width;
		scale[1][1] = height;

		debug_vertex data[4]{}; //bot left, bot right, top right, top left
		//-------------Matrix Projection of four points-----------------
		glm::vec4 point[4]{
			{-0.5f, -0.5f, 0.f, 1.f},//bottom left
			{0.5f, -0.5f, 0.f, 1.f}, //bottom right
			{0.5f, 0.5f, 0.f, 1.f}, //top right
			{-0.5f, 0.5f, 0.f, 1.f} //top left
		};

		for (int i{ 0 }; i < 4; ++i) {
			glm::vec4 pos{ translate* rotation* scale* point[i] };
			data[i].pos = { pos.x, pos.y };
			data[i].col = color;
		}
		DrawLine(data[0].pos, data[1].pos, color);
		DrawLine(data[1].pos, data[2].pos, color);
		DrawLine(data[2].pos, data[3].pos, color);
		DrawLine(data[3].pos, data[0].pos, color);
	}

	/*!***********************************************************************
	\brief
	 DrawCircle draws lines in a way that represents a circle at center with
	 a radius of radius.
	*************************************************************************/
	void Debugger::DrawCircle(Vec2<float> center, float radius, Vec4<float> color)
	{
		if (!m_debugModeOn) return;
		//one angle for before
		float bangle{ 0.f };
		//one angle for after
		float aangle{ bangle + INCREMENT };
		//set each circle to have 20 lines
		for (size_t i{ 0 }; i < CIRCLE_LINES; ++i) {
			DrawLine({ center.x + radius * cosf(bangle), center.y + radius * sinf(bangle) },
				{ center.x + radius * cosf(aangle), center.y + radius * sinf(aangle) },
				color);
			bangle = i + 1 * (float)INCREMENT;
			aangle = bangle + INCREMENT;
		}
	}

	/*!***********************************************************************
	 \brief
	 Function loads a line for the update to draw. The line is defined by a
	 start and an end point.
	*************************************************************************/
	void Debugger::DrawLine(Vec2<float> start, Vec2<float> end, Vec4<float> color)
	{
		if (!m_debugModeOn) return;
		//std::cout << drawobj.size() << '\n';
		//std::cout << '\n';
		//drawobj.size();
		drawobj.push(DebugObject{ 0.f,0.f,0.f,
			DebugTypes::LINE,
			start,
			end,
			color }); 

	}

	/*!***********************************************************************
	\brief
	 Function pushes a line for update to draw. This line is defined by a
	 start point, a direction and a magnitude.

	 NOTE: direction is not guarenteed to be normalized. If you want it to be
	 normalized then you need to do it yourself.
	*************************************************************************/
	void Debugger::DrawLine(Vec2<float> start, Vec2<float> direction, float magnitude, Vec4<float> color)
	{
		direction.x = direction.x * magnitude;
		direction.y = direction.y * magnitude;

		direction.x = direction.x + start.x;
		direction.y = direction.y + start.y;

		DrawLine(start, direction, color);
	}

	//loop through stack
	//if line object then do matrix projection on both start and end points
	//send the points to the gpu
	//edit the index in the index vector
	//send the index to gpu
	/*!***********************************************************************
	\brief
	 Sends the data of a debug object to the GPU to be drawn.
	*************************************************************************/
	void Debugger::LineUpdate(DebugObject& obj, const size_t& index) 
	{
		
		//-----------------Matrix projection of start point--------------

		glm::vec4 start_point{ obj.center.x, obj.center.y, 0.f, 1.f };
		start_point = Renderer::GRAPHICS->get_cam_mat() * start_point;
		obj.center.x = start_point.x;
		obj.center.y = start_point.y;
		//-----------------Matrix projection of start point--------------

		//-----------------Matrix projection of end point--------------
		start_point = { obj.end.x, obj.end.y, 0.f, 1.f };
		start_point = Renderer::GRAPHICS->get_cam_mat() * start_point;
		obj.end.x = start_point.x;
		obj.end.y = start_point.y;
		//-----------------Matrix projection of end point--------------

		//-----------------Send data to GPU--------------
		debug_vertex server_data[2]{};
		server_data[0].pos = obj.center;
		server_data[0].col = obj.color;
		server_data[1].pos = obj.end;
		server_data[1].col = obj.color;

		glNamedBufferSubData(vbo, index * sizeof(debug_vertex), sizeof(debug_vertex) * 2U, server_data);
		idx.at(index) = (unsigned short)index;
		idx.at(index + 1) = (unsigned short)(index + 1);
		//-----------------Send data to GPU--------------
	}
}