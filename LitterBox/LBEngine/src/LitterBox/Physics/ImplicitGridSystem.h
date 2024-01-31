#pragma once
#include "LitterBox/Utils/Math.h"
#include "LitterBox/Components/ColliderComponent.h"
#include "Platform/Windows/Windows.h"
#include "LitterBox/Physics/Collisions.h"

namespace LB {
	class ColliderImplicitGridSystem
	{
	public:
		void Initialize();

		void CalculateCellWidthHeight();

		int GetRows();

		int GetColumns();

		int& GetRowsRef();

		int& GetColumnsRef();

		float GetCellWidth();

		float GetCellHeight();

		void UpdateGridFrame(CPCollider* collider);

		bool CheckGridFrames(CPCollider* colliderA, CPCollider* colliderB);

		void DrawGridLines();

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