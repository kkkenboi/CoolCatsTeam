/*!************************************************************************
 \file				ImplicitGridSystem.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2451A
 \date				09-02-2024
 \brief
  This file contains the ImplicitGridSystem class. This class handles
  the creation of the ImplicitGrid for broad phase collision checks
  between colliders

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "ImplicitGridSystem.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Initialises the ColliderImplicitGridSystem
	*************************************************************************/
	void ColliderImplicitGridSystem::Initialize()
	{
		m_rows = 2;
		m_columns = 2;

		m_screensize.x = (float)WINDOWSSYSTEM->GetWidth();
		m_screensize.y = (float)WINDOWSSYSTEM->GetHeight();

		m_cell_width = m_screensize.y;
		m_cell_height = m_screensize.x;

		m_needs_update = true;
	}

	/*!***********************************************************************
	\brief
	Calculates the CellWidthAndHeight from the current screen size
	*************************************************************************/
	void ColliderImplicitGridSystem::CalculateCellWidthHeight() 
	{
		// If someone updated the rows and columns by pressing a button in
		// IMGUI
		if (m_needs_update)
		{
			if (m_rows) {
				m_cell_width = (m_screensize.x / (float)m_columns);
			}
			else {
				m_cell_width = m_screensize.x;
			}
			if (m_columns) {
				m_cell_height = (m_screensize.y / (float)m_rows);
			}
			else {
				m_cell_height = m_screensize.y;
			}
			//std::cout << "Cell Width: " << m_cell_width << '\n';
			//std::cout << "Cell Height: " << m_cell_height << '\n';

			// Create the vector of AABB's here
			m_grids.clear();

			for (int i = 0; i < m_rows; ++i) 
			{
				for (int j = 0; j < m_columns; ++j)
				{
					AABB temp_aabb;
					temp_aabb.m_c = LB::Vec2<float>{ (m_cell_width / 2) * j, (m_cell_height / 2) * i};
					temp_aabb.m_max = LB::Vec2<float>{m_cell_width * (j + 1), m_cell_height * (i + 1)};
					temp_aabb.m_min = LB::Vec2<float>{ m_cell_width * (j), m_cell_height * (i) };
					//std::cout << "Grid " << j + (i * m_columns) << " Min: " << temp_aabb.m_min.x << ", " << temp_aabb.m_min.y << '\n';
					//std::cout << "Grid " << j + (i * m_columns) << " Max: " << temp_aabb.m_max.x << ", " << temp_aabb.m_max.y << '\n';

					m_grids.push_back(temp_aabb);
				}
			}
			m_needs_update = false;
		}
	}
	
	/*!***********************************************************************
	\brief
	Updates the GridFrames for a particular CPCollider*
	*************************************************************************/
	void ColliderImplicitGridSystem::UpdateGridFrame(CPCollider* collider)
	{
		// We need to form an AABB
		collider->m_grid_frames.clear();

		Vec2<float> temp_normal;
		float temp_depth;
		for (size_t i = 0; i < m_grids.size(); ++i)
		{
			if (CollisionIntersection_BoxBox(collider->m_aabb, Vec2<float>{ 0.f,0.f },
				m_grids[i], Vec2<float>{0.f, 0.f},
				0.1f, temp_normal, temp_depth) == true)
			{
				collider->m_grid_frames.push_back(static_cast<int>(i));
			}
		}
		
	}

	/*!***********************************************************************
	\brief
	Checks the GridFrames of two CPCollider*, and returns true if they can find
	one common grid between them or false if they can't
	*************************************************************************/
	bool ColliderImplicitGridSystem::CheckGridFrames(CPCollider* colliderA, CPCollider* colliderB)
	{
		for (size_t i = 0; i < colliderA->GetGridFrames().size(); ++i) 
		{
			for (size_t j = 0; j < colliderB->GetGridFrames().size(); ++j)
			{
				if (colliderA->GetGridFrames()[i] == colliderB->GetGridFrames()[j])
				{
					return true;
				}
			}
		}
		return false;
	}

	/*!***********************************************************************
	\brief
	Draws the GridLines for the ColliderImplicitGridSystem
	*************************************************************************/
	void ColliderImplicitGridSystem::DrawGridLines()
	{
		// Draw the line that separates the grids
		// Using cell size

		// Draw rows
		for (int i = 1; i < m_rows; ++i)
		{
			DEBUG->DrawLine(Vec2<float>{0.f, m_cell_height * i}, Vec2<float>{m_screensize.x, m_cell_height * i}
			, Vec4<float> {0.f, 1.f, 0.f, 1.0f});
		}

		// Draw columns
		for (int i = 1; i < m_columns; ++i)
		{
			DEBUG->DrawLine(Vec2<float>{m_cell_width * i, 0.f}, Vec2<float>{m_cell_width * i, m_screensize.y}
			, Vec4<float> {0.f, 1.f, 0.f, 1.0f});
		}

		//DebuggerLogFormat("screensize_x: %f\n", m_screensize.x);
		//DebuggerLogFormat("screensize_y: %f\n", m_screensize.y);


		//DebuggerLogFormat("m_cell_width: %f\n", m_cell_width);
		//DebuggerLogFormat("m_cell_height: %f\n", m_cell_height);
	}
	
	/*!***********************************************************************
	\brief
	Returns the number of rows in the Implicit Grid System
	*************************************************************************/
	int ColliderImplicitGridSystem::GetRows()
	{
		return m_rows;
	}

	/*!***********************************************************************
	\brief
	Returns the number of columns in the Implicit Grid System
	*************************************************************************/
	int ColliderImplicitGridSystem::GetColumns()
	{
		return m_columns;
	}

	/*!***********************************************************************
	\brief
	Gets a reference to the number of rows in the Implicit Grid
	System
	*************************************************************************/
	int& ColliderImplicitGridSystem::GetRowsRef()
	{
		return m_rows;
	}

	/*!***********************************************************************
	\brief
	Gets a reference to the number of columns in the Implicit Grid
	System
	*************************************************************************/
	int& ColliderImplicitGridSystem::GetColumnsRef()
	{
		return m_columns;
	}

	/*!***********************************************************************
	\brief
	Gets the cell width of one cell in the system
	*************************************************************************/
	float ColliderImplicitGridSystem::GetCellWidth()
	{
		return m_cell_width;
	}

	/*!***********************************************************************
	\brief
	Gets the cell height of one cell in the system
	*************************************************************************/
	float ColliderImplicitGridSystem::GetCellHeight()
	{
		return m_cell_height;
	}

	/*!***********************************************************************
	\brief
	Sets the m_needs_update variable to be true within the
	system
	*************************************************************************/
	void ColliderImplicitGridSystem::NeedsUpdate()
	{
		m_needs_update = true;
	}
	
}