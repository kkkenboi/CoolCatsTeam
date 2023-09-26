#include "Matrix3x3.h"
#include <cmath>

namespace LB
{
	//	a	b	c
	//	d	e	f
	//	g	h	i
	
	//constructing matrix
	template<typename T>
	Matrix3x3<T>::Matrix3x3(T a, T b, T c, T d, T e, T f, T g, T h, T i)
	{
		this->a = a;  this->b = b;  this->c = c;
		this->d = d;  this->e = e;  this->f = f;
		this->g = g;  this->h = h;  this->i = i;

		m[0][0] = a;  m[0][1] = b;  m[0][2] = c;
		m[1][0] = d;  m[1][1] = e;  m[1][2] = f;
		m[2][0] = g;  m[2][1] = h;  m[2][2] = i;
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Identity()
	{
		Zero();
		a = 1;
		e = 1;
		i = 1;
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
		return ((a * ((e * i) - (f * h))) - 
				(b * ((d * i) - (g * f))) + 
				(c * ((d * h) - (e * g))));
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Inverse()
	{
		T det = this->Determinant();
		Matrix3x3<T> inverseMatrix;

		inverseMatrix.a = (this->e * this->i - this->h * this->f) / det;
		inverseMatrix.b = (this->g * this->f - this->d * this->i) / det;
		inverseMatrix.c = (this->d * this->h - this->g * this->e) / det;

		inverseMatrix.d = (this->h * this->b - this->b * this->i) / det;
		inverseMatrix.e = (this->a * this->i - this->g * this->c) / det;
		inverseMatrix.f = (this->g * this->b - this->a * this->h) / det;

		inverseMatrix.g = (this->b * this->f - this->e * this->c) / det;
		inverseMatrix.h = (this->d * this->c - this->a * this->f) / det;
		inverseMatrix.i = (this->a * this->e - this->d * this->b) / det;

		*this = inverseMatrix;
		return *this;
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Transpose()
	{
		Matrix3x3<T> transposeResult;
		for (int i = 0; i < 3; ++i)
		{
			for (int z = 0; z < 3; ++z)
			{
				transposeResult.m[i][z] = m[z][i];
			}
		}
		return transposeResult;
	}

	template<typename T>
	void Matrix3x3<T>::SetScale(T x, T y)
	{
		Matrix3x3<T> scaleMatrix;

		scaleMatrix.m[0][0] = x;
		scaleMatrix.m[1][1] = y;
		scaleMatrix.m[2][2] = 1;

		*this = scaleMatrix;
	}

	template<typename T>
	void Matrix3x3<T>::SetTranslate(T x, T y)
	{
		Matrix3x3<T> transMatrix;

		transMatrix.m[0][0] = 1;
		transMatrix.m[1][1] = 1;
		transMatrix.m[2][2] = 1;

		transMatrix[0][2] = x;
		transMatrix[1][2] = y;

		*this = transMatrix;
	}

	template<typename T>
	void Matrix3x3<T>::SetRotate(double angle)
	{
		Matrix3x3<T> rotMatrix;

		rotMatrix.m[0][0] = cos(angle);
		rotMatrix.m[0][1] = sin(angle);
		rotMatrix.m[1][0] = -sin(angle);
		rotMatrix.m[1][1] = cos(angle);
		rotMatrix.m[2][2] = 1;

		*this = rotMatrix;
	}

	template<typename T>
	void Matrix3x3<T>::SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& scale, Matrix3x3<T> const& rot)
	{
		*this = trans * scale * rot;
	}

	template<typename T>
	Matrix3x3<T> SetScale(T x, T y)
	{
		Matrix3x3<T> scaleMatrix;

		scaleMatrix.m[0][0] = x;
		scaleMatrix.m[1][1] = y;
		scaleMatrix.m[2][2] = 1;

		return scaleMatrix;
	}

	template<typename T>
	Matrix3x3<T> SetRotate(double angle)
	{
		Matrix3x3<T> rotMatrix;

		rotMatrix.m[0][0] = cos(angle);
		rotMatrix.m[0][1] = sin(angle);
		rotMatrix.m[1][0] = -sin(angle);
		rotMatrix.m[1][1] = cos(angle);

		return rotMatrix;
	}

	template<typename T>
	Matrix3x3<T> SetTranslate(T x, T y)
	{
		Matrix3x3<T> transMatrix;
		transMatrix.m[0][0] = 1;
		transMatrix.m[1][1] = 1;
		transMatrix.m[2][2] = 1;

		transMatrix[0][2] = x;
		transMatrix[1][2] = y;

		return transMatrix;
	}

	template<typename T>
	Matrix3x3<T> SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& scale, Matrix3x3<T> const& rot)
	{
		Matrix3x3<T> transMatrix;
		return transMatrix = trans * scale * rot;
	}
}