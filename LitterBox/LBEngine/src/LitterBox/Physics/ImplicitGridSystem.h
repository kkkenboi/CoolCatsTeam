/*!************************************************************************
 \file				ImplicitGridSystem.h
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
#pragma once
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Components/ColliderComponent.h"
#include "Platform/Windows/Windows.h"
#include "LitterBox/Physics/Collisions.h"

namespace LB {
	class ColliderImplicitGridSystem
	{
	public:
		/*!***********************************************************************
		\brief
		Initialises the ColliderImplicitGridSystem
		*************************************************************************/
		void Initialize();
		/*!***********************************************************************
		\brief
		Calculates the CellWidthAndHeight from the current screen size
		*************************************************************************/
		void CalculateCellWidthHeight();
		/*!***********************************************************************
		\brief
		Returns the number of rows in the Implicit Grid System
		*************************************************************************/
		int GetRows();
		/*!***********************************************************************
		\brief
		Returns the number of columns in the Implicit Grid System
		*************************************************************************/
		int GetColumns();
		/*!***********************************************************************
		\brief
		Gets a reference to the number of rows in the Implicit Grid
		System
		*************************************************************************/
		int& GetRowsRef();
		/*!***********************************************************************
		\brief
		Gets a reference to the number of columns in the Implicit Grid
		System
		*************************************************************************/
		int& GetColumnsRef();
		/*!***********************************************************************
		\brief
		Gets the cell width of one cell in the system
		*************************************************************************/
		float GetCellWidth();
		/*!***********************************************************************
		\brief
		Gets the cell height of one cell in the system
		*************************************************************************/
		float GetCellHeight();
		/*!***********************************************************************
		\brief
		Updates the GridFrames for a particular CPCollider*
		*************************************************************************/
		void UpdateGridFrame(CPCollider* collider);
		/*!***********************************************************************
		\brief
		Checks the GridFrames of two CPCollider*, and returns true if they can find
		one common grid between them or false if they can't
		*************************************************************************/
		bool CheckGridFrames(CPCollider* colliderA, CPCollider* colliderB);
		/*!***********************************************************************
		\brief
		Draws the GridLines for the ColliderImplicitGridSystem
		*************************************************************************/
		void DrawGridLines();
		/*!***********************************************************************
		\brief
		Sets the m_needs_update variable to be true within the
		system
		*************************************************************************/
		void NeedsUpdate();

	private:
		// Row major
		int m_rows;
		int m_columns;

		// Check when updating grid line creation
		bool m_needs_update;

		std::vector<AABB> m_grids;

		LB::Vec2<float> m_screensize;

		float m_cell_width;
		float m_cell_height;
	};
}