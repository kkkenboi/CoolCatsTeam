/*!************************************************************************
 \file				SortedVector.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				04-03-2024
 \brief

 This file contains a class for a sorted vector. It wraps the std::vector
 and sorts the data in ascending order.

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <vector>
#include <algorithm>

namespace LB
{
	/*!***********************************************************************
	\brief
	 This class is a sorted vector that wraps the std::vector and sorts the
	 data in ascending order.
	*************************************************************************/
	template <typename T>
	class SortedVector
	{
	public:
		/*!**********************************************************************
		\brief
		 Inserts the value into the vector and sorts the vector.
		*************************************************************************/
		void Insert(T const& value)
		{
			auto it = std::lower_bound(m_data.begin(), m_data.end(), value);
			m_data.insert(it, value);
		}

		T* Find(T const& value)
		{
			auto it = std::find(m_data.begin(), m_data.end(), value);
			return it != m_data.end() ? &*it : nullptr;
		}

		/*!**********************************************************************
		\brief
		 Removes the value from the vector.
		*************************************************************************/
		void Remove(T const& value)
		{
			auto it = std::find(m_data.begin(), m_data.end(), value);
			if (it != m_data.end())
			{
				m_data.erase(it);
			}
		}

		/*!**********************************************************************
		\brief
		 Returns the value at the index.
		*************************************************************************/
		T const& operator[](size_t index) const
		{
			return m_data[index];
		}

		T& operator[](size_t index)
		{
			return m_data[index];
		}

		/*!**********************************************************************
		\brief
		 Returns the size of the vector.
		*************************************************************************/
		size_t Size() const
		{
			return m_data.size();
		}

		/*!**********************************************************************
		\brief
		 Clears the vector.
		*************************************************************************/
		void Clear()
		{
			m_data.clear();
		}

		/*!**********************************************************************
		\brief
		 Returns the actual vector containing the data.
		*************************************************************************/
		std::vector<T>& GetData()
		{
			return m_data;
		}

	private:
		std::vector<T> m_data;
	};
}
