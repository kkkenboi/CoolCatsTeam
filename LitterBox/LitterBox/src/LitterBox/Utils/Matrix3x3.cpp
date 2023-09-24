#include "Matrix3x3.h"
#include <cmath>

namespace LB
{
	// :(

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Identity()
	{
		Zero();
		m[0][0] = 1;
		m[1][1] = 1;
		m[2][2] = 1;
		return *this;
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Zero()
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int z = 0; z < 3; ++z)
			{
				m[i][z] = 0;
			}
		}
		return *this();
	}

	template<typename T>
	double Matrix3x3<T>::Determinant()
	{
		return Matrix3x3();
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Inverse()
	{
		return Matrix3x3();
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Transpose()
	{
		return Matrix3x3();
	}

	template<typename T>
	void Matrix3x3<T>::SetScale(T x, T y)
	{
		//m[0][0] *= x;
		//return Matrix3x3();
	}

	template<typename T>
	void Matrix3x3<T>::SetTranslate(T x, T y)
	{
		//return Matrix3x3();
	}

	template<typename T>
	void Matrix3x3<T>::SetRotate(double angle)
	{
		//return Matrix3x3();
	}

	template<typename T>
	void Matrix3x3<T>::SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& scale, Matrix3x3<T> const& rot)
	{
		//return Matrix3x3();
	}

	template<typename T>
	Matrix3x3<T> SetScale(T x, T y)
	{
		return Matrix3x3();
	}

	template<typename T>
	Matrix3x3<T> SetRotate(T x, T y)
	{
		return Matrix3x3();
	}

	template<typename T>
	Matrix3x3<T> SetTranslate(double angle)
	{
		return Matrix3x3();
	}

	template<typename T>
	Matrix3x3<T> SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& scale, Matrix3x3<T> const& rot)
	{
		return Matrix3x3();
	}
}