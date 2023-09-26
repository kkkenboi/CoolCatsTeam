/*!************************************************************************
 \file			Matrix3x3.h
 \author		Vanessa Chua Siew Jin
 \par DP email: vanessasiewjin.chua\@digipen.edu
 \par Course:	CSD2401A
 \date			26-09-2023
 \brief

**************************************************************************/

//#include <cmath> // For sin, cos
#include "LitterBox/Utils/Math.h"

/***************************************************************************************************
*
* Matrix Class declaration
*
***************************************************************************************************/

namespace LB
{
	template<typename T>
	class Matrix3x3
	{
	public:
		#define a m[0][0]
		#define b m[0][1]
		#define c m[0][2]
		#define d m[1][0]
		#define e m[1][1]
		#define f m[1][2]
		#define g m[2][0]
		#define h m[2][1]
		#define i m[2][2]

		T m[3][3];

		Matrix3x3 Zero(); //WORK
		Matrix3x3 Identity(); //WORK
		double	  Determinant();
		Matrix3x3 Inverse();
		Matrix3x3 Transpose();

		void Set(const T& am, const T& bm, const T& cm,
			const T& dm, const T& em, const T& fm,
			const T& gm, const T& hm, const T& im);

		void SetScale(T x, T y);
		void SetTranslate(T x, T y);
		void SetDegRotate(double angle);
		void SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& scale, Matrix3x3<T> const& rot);
		
		void displayMatrix(); //This is for printing of Matrix for checking
	};

	template<typename T>
	Matrix3x3<T> SetScale(T x, T y);

	template<typename T>
	Matrix3x3<T> SetDegRotate(double angle);

	template<typename T>
	Matrix3x3<T> SetTranslate(T x, T y);

	template<typename T>
	Matrix3x3<T> SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& scale, Matrix3x3<T> const& rot);
}

/***************************************************************************************************
*
* Matrix definitions
*
***************************************************************************************************/

namespace LB
{
	//	a	b	c
	//	d	e	f
	//	g	h	i

	template<typename T>
	void Matrix3x3<T>::Set(const T& am, const T& bm, const T& cm,
		const T& dm, const T& em, const T& fm,
		const T& gm, const T& hm, const T& im)
	{
		this->a = am; this->b = bm; this->c = cm;
		this->d = dm; this->e = em; this->f = fm;
		this->g = gm; this->h = hm; this->i = im;
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Zero() //WORK
	{
		for (int x = 0; x < 3; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				m[x][y] = 0;
			}
		}
		return *this;
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Identity() //WORK
	{
		Zero();
		a = 1;
		e = 1;
		i = 1;
		return *this;
	}

	template<typename T>
	double Matrix3x3<T>::Determinant() //WORK
	{
		return ((a * ((e * i) - (f * h))) -
			   (b * ((d * i) - (g * f))) +
			   (c * ((d * h) - (e * g))));
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Inverse() //WORK
	{
		T det = Determinant();
		Matrix3x3<T> inverseMatrix;

		inverseMatrix.a = (e * i - h * f) / det;
		inverseMatrix.b = (g * f - d * i) / det;
		inverseMatrix.c = (d * h - g * e) / det;

		inverseMatrix.d = (h * b - b * i) / det;
		inverseMatrix.e = (a * i - g * c) / det;
		inverseMatrix.f = (g * b - a * h) / det;

		inverseMatrix.g = (b * f - e * c) / det;
		inverseMatrix.h = (d * c - a * f) / det;
		inverseMatrix.i = (a * e - d * b) / det;

		*this = inverseMatrix;
		return *this;
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Transpose() //WORK
	{
		this->displayMatrix();
		Matrix3x3<T> transposeResult{};
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				transposeResult.m[row][col] = m[col][row];
			}
		}
		return transposeResult;
	}

	template<typename T>
	void Matrix3x3<T>::SetScale(T x, T y) //WORK
	{
		Matrix3x3<T> scaleMatrix;
		scaleMatrix.Identity();
		scaleMatrix.a = x;
		scaleMatrix.e = y;

		*this = scaleMatrix;
	}

	template<typename T>
	void Matrix3x3<T>::SetTranslate(T x, T y) //WORK
	{
		Matrix3x3<T> transMatrix;
		transMatrix.Identity();

		transMatrix.c = x;
		transMatrix.f = y;

		*this = transMatrix;
	}

	template<typename T>
	void Matrix3x3<T>::SetDegRotate(double angle) //in degree
	{
		Matrix3x3<T> rotMatrix;
		angle *= PI / 180;

		rotMatrix.a = cos(angle);
		rotMatrix.b = -sin(angle);
		rotMatrix.d = sin(angle);
		rotMatrix.e = cos(angle);
		rotMatrix.i = 1;

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
		scaleMatrix.Identity();
		scaleMatrix.a = x;
		scaleMatrix.e = y;

		return scaleMatrix;
	}

	template<typename T>
	Matrix3x3<T> SetDegRotate(double angle) //in degree
	{
		Matrix3x3<T> rotMatrix;
		angle *= PI / 180;

		rotMatrix.a = cos(angle);
		rotMatrix.b = -sin(angle);
		rotMatrix.d = sin(angle);
		rotMatrix.e = cos(angle);
		rotMatrix.i = 1;

		return rotMatrix;
	}

	template<typename T>
	Matrix3x3<T> SetTranslate(T x, T y)
	{
		Matrix3x3<T> transMatrix;
		transMatrix.Identity();

		transMatrix.c = x;
		transMatrix.f = y;

		return transMatrix;
	}

	template<typename T>
	Matrix3x3<T> SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& scale, Matrix3x3<T> const& rot)
	{
		Matrix3x3<T> transMatrix;
		return transMatrix = trans * scale * rot;
	}

	//This is for printing of Matrix for checking
	template <typename T>
	void Matrix3x3<T>::displayMatrix() {
		for (int row = 0; row < 3; ++row) {
			for (int col = 0; col < 3; ++col) {
				std::cout << m[row][col] << "\t";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}