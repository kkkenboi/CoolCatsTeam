/*!************************************************************************
 \file				Matrix3x3.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin.chua@digipen.edu
 \par Course:		CSD2401A
 \date				27-09-2023
 \brief
 The functions in the Matrix class include:

**************************************************************************/

#include "Math.h"

namespace LB
{
	/***************************************************************************************************
	*
	* Matrix declarations
	*
	***************************************************************************************************/
	template<typename T>
	class Matrix3x3
	{
		#define PI 3.14159265358979323846
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

		Matrix3x3			 (); //default constructor

		Matrix3x3 operator*	 (Matrix3x3<T> rhs) const; //WORK ITSELF*MATRIX


		Matrix3x3 Zero		 (); //WORK
		Matrix3x3 Identity	 (); //WORK
		double	  Determinant(); //WORK
		Matrix3x3 Inverse	 (); //WORK
		Matrix3x3 Transpose	 (); //WORK

		void	  Set		 (const T& am, const T& bm, const T& cm,
							  const T& dm, const T& em, const T& fm,
							  const T& gm, const T& hm, const T& im); //WORK

		void	  Display	 (); //WORK
		void	  SetScale	 (T x, T y); //WORK
		void	  SetTranslate(T x, T y); //WORK
		void	  SetDegRotate(double angle); //WORK
		void	  SetRadRotate(double angle);
		void	  SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& rot, Matrix3x3<T> const& scale); //WORK
	
		//Matrix3x3 operator=	 (Matrix3x3<T> rhs) const; //EQUAL TO MATRIX
		//Vec2<T> operator*	 (Vec2<T> rhs); //MATRIX * VECTOR2
		//Vec3<T> operator*	 (Vec3<T> rhs); //MATRIX * VECTOR3
	};

	template<typename T>
	Matrix3x3<T> operator*	 (Matrix3x3<T> lhs, Matrix3x3<T> rhs); //MATRIX * MATRIX

	template<typename T>
	Vec2<T>		 operator*	 (Matrix3x3<T> lhs, Vec2<T> rhs); //MATRIX * VECTOR2

	template<typename T>
	Vec3<T>		 operator*	 (Matrix3x3<T> lhs, Vec3<T> rhs); //MATRIX * VECTOR3

	template<typename T>
	Matrix3x3<T> SetScale	 (T x, T y); //scaling of matrix

	template<typename T>
	Matrix3x3<T> SetDegRotate(double angle); //Rotating of matrix in degree

	template<typename T>
	Matrix3x3<T> SetRadRotate(double angle); //Rotating of matrix in radian

	template<typename T>
	Matrix3x3<T> SetTranslate(T x, T y); //Translating of matrix

	template<typename T>
	Matrix3x3<T> SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& rot, Matrix3x3<T> const& scale); //Transforming of Matrix = trans * rot * scale
}

/***************************************************************************************************
*
* Matrix definitions
*
***************************************************************************************************/

namespace LB
{
	// |a	b	c | |x|
	// |d	e	f | |y|
	// |g	h	i | |z|

	/****************************************MEMBER*****************************************/

	template<typename T>
	Matrix3x3<T>::Matrix3x3()
	{
		Zero();
	}

	template<typename T> //MATRIX * MATRIX
	Matrix3x3<T> Matrix3x3<T>::operator*(Matrix3x3<T> rhs) const
	{
		Matrix3x3 result;
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				result.m[row][col] = m[row][col] * rhs.m[row][col];
			}
		}
		return result;
	}

	// template<typename T>
	// Matrix3x3<T> Matrix3x3<T>::operator=(Matrix3x3<T> rhs) const //EQUAL TO MATRIX
	// {
	// 	Matrix3x3 result;
	// 	for (int row = 0; row < 3; ++row)
	// 	{
	// 		for (int col = 0; col < 3; ++col)
	// 		{
	// 			result.m[row][col] = rhs.m[row][col];
	// 		}
	// 	}
	// 	return result;
	// }

	template<typename T>
	void Matrix3x3<T>::Set(const T& am, const T& bm, const T& cm,
		const T& dm, const T& em, const T& fm,
		const T& gm, const T& hm, const T& im)
	{
		a = am; b = bm; c = cm;
		d = dm; e = em; f = fm;
		g = gm; h = hm; i = im;
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
		if (det == 0)
		{
			//DebuggerLogError("Trying to inverse a matrix with det <= 0!");
			return *this;
		}
		Matrix3x3<T> inverseMatrix;

		inverseMatrix.a =	(e * i - h * f) / det;
		inverseMatrix.b = -	(b * i - c * h) / det;
		inverseMatrix.c =	(b * f - c * e) / det;

		inverseMatrix.d = -	(d * i - f * g) / det;
		inverseMatrix.e =	(a * i - c * g) / det;
		inverseMatrix.f = -	(a * f - c * d) / det;

		inverseMatrix.g =	(d * h - e * g) / det;
		inverseMatrix.h = -	(a * h - b * g) / det;
		inverseMatrix.i =	(a * e - d * b) / det;

		*this = inverseMatrix;
		return *this;
	}

	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Transpose() //WORK
	{
		Matrix3x3<T> transposeMatrix;
		for (int x = 0; x < 3; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				transposeMatrix.m[x][y] = m[y][x];
			}
		}
		*this = transposeMatrix;
		return transposeMatrix;
	}

	template<typename T>
	void Matrix3x3<T>::SetScale(T x, T y) //WORK
	{
		Identity();
		a = x;
		e = y;
	}

	template<typename T>
	void Matrix3x3<T>::SetTranslate(T x, T y) //WORK
	{
		Identity();
		c = x;
		f = y;
	}

	template<typename T>
	void Matrix3x3<T>::SetDegRotate(double angle) //in degree
	{
		angle *= PI / 180.0;
		SetRadRotate(angle);
	}

	template<typename T>
	void Matrix3x3<T>::SetRadRotate(double angle) //in rad
	{
		Identity();
		a = cos(angle);
		b = -sin(angle);
		d = sin(angle);
		e = cos(angle);
	}

	template<typename T>
	void Matrix3x3<T>::SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& rot, Matrix3x3<T> const& scale)
	{
		*this = trans * rot * scale;
	}

	/****************************************NON-MEMBER*****************************************/

	template<typename T>
	Matrix3x3<T> operator*(Matrix3x3<T> lhs, Matrix3x3<T> rhs)
	{
		Matrix3x3 result;
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				result = lhs.m[row][col] * rhs.m[row][col];
			}
		}
		return result;
	}

	template<typename T> //MATRIX * VECTOR2
	Vec2<T> operator*(Matrix3x3<T> lhs, Vec2<T> rhs)
	{
		Vec2 result;

		result.x = lhs.a * rhs.x + lhs.b * rhs.y + lhs.c; // x = (a*x) + (b*y) + (c*z)
		result.y = lhs.d * rhs.x + lhs.e * rhs.y + lhs.f; // y = (d*x) + (e*y) + (f*z)

		return result;
	}

	template<typename T> //MATRIX * VECTOR3
	Vec3<T> operator*(Matrix3x3<T> lhs, Vec3<T> rhs)
	{
		Vec3 result;

		result.x = lhs.a * rhs.x + lhs.b * rhs.y + lhs.c * rhs.z;
		result.y = lhs.d * rhs.x + lhs.e * rhs.y + lhs.f * rhs.z;
		result.z = 1;

		return result;
	}

	template<typename T>
	Matrix3x3<T> SetScale(T x, T y)
	{
		return Matrix3x3<T>().SetScale(x, y);
	}

	template<typename T>
	Matrix3x3<T> SetDegRotate(double angle) //in degree
	{
		return Matrix3x3<T>().SetDegRotate(angle);
	}

	template<typename T>
	Matrix3x3<T> SetRadRotate(double angle) //in degree
	{
		return Matrix3x3<T>().SetRadRotate(angle);
	}

	template<typename T>
	Matrix3x3<T> SetTranslate(T x, T y)
	{
		return Matrix3x3<T>().SetTranslate(x, y);
	}

	template<typename T>
	Matrix3x3<T> SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& rot, Matrix3x3<T> const& scale)
	{
		return Matrix3x3<T>().SetTransform(trans, rot, scale);
	}

	//This is for printing of Matrix
	template <typename T>
	void Matrix3x3<T>::Display() {
		for (int row = 0; row < 3; ++row) 
		{
			for (int col = 0; col < 3; ++col) 
			{
				std::cout << m[row][col] << "\t";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
}