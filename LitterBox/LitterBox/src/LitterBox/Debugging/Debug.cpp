/*!************************************************************************
 \file			Debug.cpp
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			18-09-2023
 \brief

**************************************************************************/

#include "Debug.h"
#include "LitterBox/Renderer/Renderer.h"
#include <iostream>

constexpr int CIRCLE_LINES{ 20 };
constexpr float INCREMENT{ 2.f * (float)PI / (float)CIRCLE_LINES };

namespace LB 
{
	Debugger* DEBUG = nullptr;

	Debugger::Debugger()
	{
		if (!DEBUG)
			DEBUG = this;
		else
			std::cerr << "Debug System already exist" << std::endl;
	}

	struct debug_vertex {
		Vec2<float> pos;
		Vec4<float> col;
	};

	//TODO modulate the vertex size
	//vertex size should = min 3000 x 4 (number of quads in render object)
	void Debugger::Initialize() {
		SetSystemName("Debug System");
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

		shader_source shd_pgm{ shader_parser("../Assets/Shaders/Basic.shader") };
		shader = create_shader(shd_pgm.vtx_shd.c_str(), shd_pgm.frg_shd.c_str());

		std::cout << "STACK SIZE BEFORE: " << drawobj.size();
		DrawLine({ 30.f, 30.f }, { 60.f, 60.f }, { 0.f, 1.f, 0.f, 1.f });
		std::cout << " STACK AFTER: " << drawobj.size() << std::endl;

		glLineWidth(5.f);
	}

	//loop through stack
	//if line object then do matrix projection on both start and end points
	//send the points to the gpu
	//edit the index in the index vector
	//send the index to gpu
	void Debugger::line_update(Debug_Object& obj, const size_t& index) {
		//-----------------Matrix projection of start point--------------
		glm::vec4 start_point{ obj.center.x, obj.center.y, 0.f, 1.f };
		start_point = cam.world_NDC * start_point;
		obj.center.x = start_point.x;
		obj.center.y = start_point.y;
		//-----------------Matrix projection of start point--------------

		//-----------------Matrix projection of end point--------------
		start_point = { obj.end.x, obj.end.y, 0.f, 1.f };
		start_point = cam.world_NDC * start_point;
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
		idx.at(index) = index;
		idx.at(index + 1) = index + 1;
		//-----------------Send data to GPU--------------
	}

	//TODO can remove the checking of debug type and just draw lines
	void Debugger::Update() {
		size_t index{ 0 };
		while (drawobj.size()) {
			//loop through object to 
			line_update(drawobj.top(), index);
			index += 2;
			drawobj.pop();
		}

		//pass index data inside
		glNamedBufferSubData(ibo, 0, index * sizeof(unsigned short), idx.data());
		glVertexAttrib1f(4, -1.f);
		glBindVertexArray(vao);
		glDrawElements(GL_LINES, index, GL_UNSIGNED_SHORT, nullptr);

		//DrawLine({ 450.f, 450.f }, { 600.f, 600.f }, {1.f,0.f,0.f,1.f});
		DrawCircle({450.f, 450.f}, 30.f, {0.f,1.f,0.f,1.f});
	}

	void Debugger::SetColor(Vec4<float> color)
	{
		m_drawColor = color;
	}

	void Debugger::DrawLine(Vec2<float> start, Vec2<float> end, Vec4<float> color)
	{
		drawobj.push(Debug_Object{ 0.f,0.f,0.f,
			Debug_Types::LINE,
			start,
			end,
			color });
	}

	void Debugger::DrawLine(Vec2<float> start, Vec2<float> end)
	{
		DrawLine(start, end, m_drawColor);
	}

	void Debugger::DrawBox(Vec2<float> center, float length, Vec4<float> color)
	{
		UNREFERENCED_PARAMETER(center);
		UNREFERENCED_PARAMETER(length);
		UNREFERENCED_PARAMETER(color);
	}

	void Debugger::DrawBox(Vec2<float> center, float width, float height, Vec4<float> color)
	{
		debug_vertex data[4]{}; //bot left, bot right, top right, top left
		//-------------Matrix Projection of four points-----------------
		glm::vec4 point[4]{
			{center.x - width * 0.5f, center.y - height * 0.5f, 0.f, 1.f},//bottom left
			{center.x + width * 0.5f, center.y - height * 0.5f, 0.f, 1.f}, //bottom right
			{center.x + width * 0.5f, center.y + height * 0.5f, 0.f, 1.f}, //top right
			{center.x - width * 0.5f, center.y + height * 0.5f, 0.f, 1.f} //top left
		};

		for (int i{ 0 }; i < 4; ++i) {
			data[i].pos = { point[i].x, point[i].y };
			data[i].col = color;
		}
		DrawLine(data[0].pos, data[1].pos, color);
		DrawLine(data[1].pos, data[2].pos, color);
		DrawLine(data[2].pos, data[3].pos, color);
		DrawLine(data[3].pos, data[0].pos, color);
	}

	void Debugger::DrawBox(Vec2<float> center, float length)
	{
		DrawBox(center, length, m_drawColor);
	}

	void Debugger::DrawCircle(Vec2<float> center, float radius, Vec4<float> color)
	{
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

	void Debugger::Log(std::string const& message, const char* file, int line)
	{
		fprintf(stdout, "[DEBUGGER LOG] [%s:%d] %s\n", file, line, message.c_str());
	}

	void Debugger::LogWarning(std::string const& message, const char* file, int line)
	{
		fprintf(stdout, "[DEBUGGER WARNING!] [%s:%d] %s\n", file, line, message.c_str());
	}

	void Debugger::LogError(std::string const& message, const char* file, int line)
	{
		fprintf(stderr, "[DEBUGGER ERROR!!] [%s:%d] %s\n", file, line, message.c_str());

	}

	void Debugger::Assert(bool expectedCondition, std::string const& message, const char* file, int line)
	{
		if (!expectedCondition)
		{
			fprintf(stderr, "[DEBUGGER ASSERT!!] [%s:%d] %s\n", file, line, message.c_str());
		}
	}

}
