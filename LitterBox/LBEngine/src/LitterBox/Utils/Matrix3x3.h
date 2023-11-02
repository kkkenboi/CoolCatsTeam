/*!************************************************************************
 \file				Matrix3x3.h
 \author(s)			Vanessa Chua Siew Jin
 \par DP email(s):	vanessasiewjin.chua@digipen.edu
 \par Course:		CSD2401A
 \date				27-09-2023
 \brief
 The functions in the Matrix class include:

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "Math.h"	// For PI, sin, cos
#include "tuple"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 A 3x3 Matrix class that holds any type T, used for calculating TRS for transform
	*************************************************************************/
	template<typename T>
	class Matrix3x3
	{
	public:
			// Shortform alphabets for different elements
		T	& a{ m[0][0] }, & b{ m[0][1] }, & c{ m[0][2] },
			& d{ m[1][0] }, & e{ m[1][1] }, & f{ m[1][2] },
			& g{ m[2][0] }, & h{ m[2][1] }, & i{ m[2][2] };

		// 2D array, rows, col
		T m[3][3];

		/*!***********************************************************************
		 \brief
		 Default constructor, sets all elements to 0
		*************************************************************************/
		Matrix3x3();

		/*!***********************************************************************
		 \brief
		 Default constructor, sets all elements to 0
		*************************************************************************/
		Matrix3x3(const T& am, const T& bm, const T& cm,
				  const T& dm, const T& em, const T& fm,
				  const T& gm, const T& hm, const T& im); //default constructor


		/*!***********************************************************************
		 \brief
		 Sets all elements in the matrix to 0
		 0 0 0
		 0 0 0
		 0 0 0
		*************************************************************************/
		Matrix3x3 Zero();

		/*!***********************************************************************
		 \brief
		 Sets an Identity Matrix
		 1 0 0
		 0 1 0
		 0 0 1
		*************************************************************************/
		Matrix3x3 Identity();

		/*!***********************************************************************
		 \brief
		 Giving the determinant for the matrix, will use this to calculate
		 the inverse
		*************************************************************************/
		double Determinant();

		/*!***********************************************************************
		 \brief
		 Inverse of the Matrix, returning a matrix as a result
		*************************************************************************/
		Matrix3x3 Inverse(); //Inverse Matrix

		/*!***********************************************************************
		 \brief
		 Transpose of the Matrix
		*************************************************************************/
		Matrix3x3 Transpose(); //Transpose Matrix

		/*!***********************************************************************
		 \brief
		 Displaying of the Matrix - usually for checking of matrix calculation
		*************************************************************************/
		void	  Display();

		/*!***********************************************************************
		 \brief
		 Scaling of the Matrix
		 x 0 0
		 0 y 0
		 0 0 1
		*************************************************************************/
		void	  SetScale(T x, T y);

		/*!***********************************************************************
		 \brief
		 Translation of the Matrix
		 1 0 x
		 0 1 y
		 0 0 1
		*************************************************************************/
		void	  SetTranslate(T x, T y);							 //Translating of the Matrix

		/*!***********************************************************************
		 \brief
		 Rotating of the Matrix (degree)
		 cos(angle)		-sin(angle)		 0
		 sin(angle)		cos(angle)		 0
			 0				0			 1
		*************************************************************************/
		void	  SetDegRotate(double angle);						 //Rotating the Matrix in Degree

		/*!***********************************************************************
		 \brief
		 Rotating of the Matrix (radian)
		 cos(angle)		-sin(angle)		 0
		 sin(angle)		cos(angle)		 0
			 0				0			 1
		*************************************************************************/
		void	  SetRadRotate(double angle);						 //Rotating the Matrix in Radian

		/*!***********************************************************************
		 \brief
		 Takes in 3 parameters for translation, rotation and scale. They will multiply to get the Transformation
		 In order of S*R*T
		*************************************************************************/
		void	  SetTransform(Matrix3x3<T> const& trans,
			Matrix3x3<T> const& rot,
			Matrix3x3<T> const& scale);			 //Transforming the Matrix = T*R*S

		/*!***********************************************************************
		 \brief
		 Basic operator(s) *...
		*************************************************************************/
		Matrix3x3 operator*	(Matrix3x3<T> rhs) const; //MATRIX(itself) * MATRIX

		//additional operators that may need in the future
		Matrix3x3& operator= (Matrix3x3<T> rhs); //EQUAL TO MATRIX
		Vec2<T> operator*	 (Vec2<T> rhs); //MATRIX * VECTOR2
		Vec3<T> operator*	 (Vec3<T> rhs); //MATRIX * VECTOR3
	};

	/****************************************NON-MEMBER*****************************************/

	/*!***********************************************************************
	\brief
	 Non member operators overloads *
	 - MATRIX * MATRIX
	 - MATRIX * VEC2
	 - MATRIX * VEC3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> operator*	 (Matrix3x3<T> lhs, Matrix3x3<T> rhs); //MATRIX * MATRIX

	template<typename T>
	Vec2<T>		 operator*	 (Matrix3x3<T> lhs, Vec2<T> rhs); //MATRIX * VECTOR2

	template<typename T>
	Vec3<T>		 operator*	 (Matrix3x3<T> lhs, Vec3<T> rhs); //MATRIX * VECTOR3

	/*!***********************************************************************
	\brief
	 Set Scale (Non-member functions)
	 Setting scale on a another Matrix
	 x 0 0
	 0 y 0
	 0 0 1
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> SetScale(T x, T y); //scaling of matrix

	/*!***********************************************************************
	 \brief
	 Rotating of the Matrix (degree, Non-member functions)
	 cos(angle)		-sin(angle)		 0
	 sin(angle)		cos(angle)		 0
		 0				0			 1
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> SetDegRotate(double angle); //Rotating of matrix in degree

	/*!***********************************************************************
	 \brief
	 Rotating of the Matrix (radian, Non-member functions)
	 cos(angle)		-sin(angle)		 0
	 sin(angle)		cos(angle)		 0
		 0				0			 1
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> SetRadRotate(double angle); //Rotating of matrix in radian

	/*!***********************************************************************
	 \brief
	 Translation of the Matrix (Non-member functions)
	 1 0 x
	 0 1 y
	 0 0 1
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> SetTranslate(T x, T y); //Translating of matrix

	/*!***********************************************************************
	 \brief
	 (Non-member functions)
	 Takes in 3 parameters for translation, rotation and scale. They will multiply to get the Transformation
	 In order of S*R*T
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& rot, Matrix3x3<T> const& scale); //Transforming of Matrix = trans * rot * scale
}

/***************************************************************************************************
*
*		   Matrix definitions:
* 		MATRIX				VECTOR
*		|a	 b	 c |		|x|
*		|d	 e	 f |		|y|
*		|g	 h	 i |		|z|
*
***************************************************************************************************/

namespace LB
{
	/****************************************MEMBER*****************************************/

	/*!***********************************************************************
	  \brief
	 Default Constructor that initialise elements in the Matrix to be set to 0.
	 0 0 0
	 0 0 0
	 0 0 0
	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T>::Matrix3x3()
	{
		a = b = c = d = e = f = g = h = i = (T)0;
	}

	/*!***********************************************************************
	  \brief
	 Constructor that sets elements in the Matrix.
	 a b c
	 d e f
	 g h i
	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T>::Matrix3x3(const T& am, const T& bm, const T& cm,
		const T& dm, const T& em, const T& fm,
		const T& gm, const T& hm, const T& im)
	{
		//Zero();
		a = am; b = bm; c = cm;
		d = dm; e = em; f = fm;
		g = gm; h = hm; i = im;
	}

	/*!***********************************************************************
	 \brief
	 Member Basic operator *
	 Where it multiplies itself and another Matrix
	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::operator*(Matrix3x3<T> rhs) const
	{
		Matrix3x3<T> result;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				result.m[i][j] = 0;
				for (int k = 0; k < 3; ++k)
				{
					result.m[i][j] += m[i][k] * rhs.m[k][j];
				}
			}
		}
		return result;
	}

	/*!***********************************************************************
	 \brief
	 Member Basic operator =
	 Where it sets a matrix to itself
	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T> //MATRIX * MATRIX
	Matrix3x3<T>& Matrix3x3<T>::operator=(Matrix3x3<T> rhs) //EQUAL TO MATRIX
	{
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
			{
				m[row][col] = rhs.m[row][col];
			}
		}
		return *this;
	}

	/*!***********************************************************************
	 \brief
	 Member Basic operator *
	 Where it multiplies Matrix and a Vec2
	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Vec2<T> Matrix3x3<T>::operator*(Vec2<T> rhs) //MATRIX * VECTOR2
	{
		Vec2<T> result;

		result.x = this->a * rhs.x + this->b * rhs.y + this->c; // x = (a*x) + (b*y) + (c*z)
		result.y = this->d * rhs.x + this->e * rhs.y + this->f; // y = (d*x) + (e*y) + (f*z)

		return result;
	}

	/*!***********************************************************************
	 \brief
	 Member Basic operator *
	 Where it multiplies Matrix and a Vec3
	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Vec3<T> Matrix3x3<T>::operator*(Vec3<T> rhs) //MATRIX * VECTOR3
	{
		Vec3<T> result;

		result.x = this->a * rhs.x + this->b * rhs.y + this->c * rhs.z;
		result.y = this->d * rhs.x + this->e * rhs.y + this->f * rhs.z;
		result.z = 1;

		return result;
	}

	/*!***********************************************************************
	 \brief
	 Member Zero function
	 that sets all elements in the matrix to 0
	 \return
	 Matrix3x3
	*************************************************************************/
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

	/*!***********************************************************************
	 \brief
	 Member Identity function
	 that sets element a, e, i as 1 and the others to 0
	 1 0 0
	 0 1 0
	 0 0 1
	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Identity() //OK
	{
		Zero();
		a = 1;
		e = 1;
		i = 1;
		return *this;
	}

	/*!***********************************************************************
	 \brief
	 Member Determinant function
	 Calculates the determinant of the Matrix, will be used in Inverse function
	 \return
	 double
	*************************************************************************/
	template<typename T>
	double Matrix3x3<T>::Determinant() //WORK
	{
		return ((a * ((e * i) - (f * h))) -
				(b * ((d * i) - (g * f))) +
				(c * ((d * h) - (e * g))));
	}

	/*!***********************************************************************
	 \brief
	 Member Inverse function
	 Returns a Matrix that calculates the inverse of the Matrix
	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> Matrix3x3<T>::Inverse() //WORK
	{
		T det = Determinant();
		if (det == 0)
		{
			return *this;
		}

		Matrix3x3<T> inverseMatrix;

		inverseMatrix.a = (e * i - h * f) / det;
		inverseMatrix.b = -(b * i - c * h) / det;
		inverseMatrix.c = (b * f - c * e) / det;

		inverseMatrix.d = -(d * i - f * g) / det;
		inverseMatrix.e = (a * i - c * g) / det;
		inverseMatrix.f = -(a * f - c * d) / det;

		inverseMatrix.g = (d * h - e * g) / det;
		inverseMatrix.h = -(a * h - b * g) / det;
		inverseMatrix.i = (a * e - d * b) / det;

		*this = inverseMatrix;
		return *this;
	}

	/*!***********************************************************************
	 \brief
	 Member Transpose function
	 Technically flip the rows to cols, cols to rows

	 | 1  2  3 |		| 1  4  7 |
	 | 4  5  6 |  ->	| 2  5  8 |
	 | 7  8  9 |		| 3  6  9 |

	 \return
	 Matrix3x3
	*************************************************************************/
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

	/*!***********************************************************************
	 \brief
	 Member SetScale function
	 Scaling of the matrix

	 | 1  0  0 |		| x  0  y |
	 | 0  1  0 |   ->	| 0  y  0 |
	 | 0  0  1 |		| 0  0  1 |

	 \return
	 void
	*************************************************************************/
	template<typename T>
	void Matrix3x3<T>::SetScale(T x, T y) //WORK
	{
		Identity();
		a = x;
		e = y;
	}

	/*!***********************************************************************
	 \brief
	 Member SetTranslate function
	 Translation of the Matrix, when you set x and y to its value, it will change the matrix to set the translate Matrix

	 | 1  0  0 |		| 1  0  x |
	 | 0  1  0 |   ->	| 0  1  y |
	 | 0  0  1 |		| 0  0  1 |

	 \return
	 void
	*************************************************************************/
	template<typename T>
	void Matrix3x3<T>::SetTranslate(T x, T y) //WORK
	{
		Identity();
		c = x;
		f = y;
	}

	/*!***********************************************************************
	 \brief (DEGREE, where angle = angle * PI/180)
	 Member SetDegRotate function
	 Translation of the Matrix, when you set x and y to its value, it will change the matrix to set the translate Matrix

	 | 1  0  0 |		| cos  -sin  x |
	 | 0  1  0 |   ->	| sin  cos	 y |
	 | 0  0  1 |		| 0		0	 1 |

	 \return
	 void
	*************************************************************************/
	template<typename T>
	void Matrix3x3<T>::SetDegRotate(double angle) //in degree
	{
		angle *= PI / 180.0;
		SetRadRotate(angle);
	}

	/*!***********************************************************************
	 \brief (RADIAN)
	 Member SetRadRotate function
	 Translation of the Matrix, when you set x and y to its value, it will change the matrix to set the translate Matrix

	 | 1  0  0 |		| cos  -sin  x |
	 | 0  1  0 |   ->	| sin  cos	 y |
	 | 0  0  1 |		| 0		0	 1 |

	 \return
	 void
	*************************************************************************/
	template<typename T>
	void Matrix3x3<T>::SetRadRotate(double angle) //in rad
	{
		Identity();
		a = cos(angle);
		b = -sin(angle);
		d = sin(angle);
		e = cos(angle);
	}

	/*!***********************************************************************
	 \brief
	 Member SetTransform function
	 Transforming of the Matrix, where you multiply S*R*T

	 \return
	 void
	*************************************************************************/
	template<typename T>
	void Matrix3x3<T>::SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& rot, Matrix3x3<T> const& scale)
	{
		*this = trans * rot * scale;
	}


	/*!***********************************************************************
	 \brief
	 Member Display function
	 This function is to display the matrix, mostly for checking in the future

	 \return
	 void
	*************************************************************************/
	//This is for printing of Matrix
	template <typename T>
	void Matrix3x3<T>::Display()
	{
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
	/*******************************************************************************************/


	/****************************************NON-MEMBER*****************************************/

	/*!***********************************************************************
	 \brief
	 Non-Member Basic operator *
	 Where it multiplies a Matrix and another Matrix
	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> operator*(Matrix3x3<T> lhs, Matrix3x3<T> rhs)
	{
		Matrix3x3<T> result;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				result.m[i][j] = 0;
				for (int k = 0; k < 3; ++k)
				{
					result.m[i][j] += lhs.m[i][k] * rhs.m[k][j];
				}
			}
		}
		return result;
	}

	/*!***********************************************************************
	 \brief
	 Non-Member Basic operator *
	 Where it multiplies a Matrix and a Vector2
	 \return
	 Vec2
	*************************************************************************/
	template<typename T> //MATRIX * VECTOR2
	Vec2<T> operator*(Matrix3x3<T> lhs, Vec2<T> rhs)
	{
		Vec2<T> result;

		result.x = lhs.a * rhs.x + lhs.b * rhs.y + lhs.c; // x = (a*x) + (b*y) + (c*z)
		result.y = lhs.d * rhs.x + lhs.e * rhs.y + lhs.f; // y = (d*x) + (e*y) + (f*z)

		return result;
	}

	/*!***********************************************************************
	 \brief
	 Non-Member Basic operator *
	 Where it multiplies a Matrix and a Vector3
	 \return
	 Vec3
	*************************************************************************/
	template<typename T> //MATRIX * VECTOR3
	Vec3<T> operator*(Matrix3x3<T> lhs, Vec3<T> rhs)
	{
		Vec3<T> result;

		result.x = lhs.a * rhs.x + lhs.b * rhs.y + lhs.c * rhs.z;
		result.y = lhs.d * rhs.x + lhs.e * rhs.y + lhs.f * rhs.z;
		result.z = 1;

		return result;
	}

	/*!***********************************************************************
	 \brief
	 Non-Member SetScale function
	 Scaling of the matrix

	 | 1  0  0 |		| x  0  y |
	 | 0  1  0 |   ->	| 0  y  0 |
	 | 0  0  1 |		| 0  0  1 |

	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> SetScale(T x, T y)
	{
		return Matrix3x3<T>().SetScale(x, y);
	}

	/*!***********************************************************************
	 \brief (DEGREE, where angle = angle * PI/180)
	 Non-Member SetDegRotate function
	 Translation of the Matrix, when you set x and y to its value, it will change the matrix to set the translate Matrix

	 | 1  0  0 |		| cos  -sin  x |
	 | 0  1  0 |   ->	| sin  cos	 y |
	 | 0  0  1 |		| 0		0	 1 |

	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> SetDegRotate(double angle) //in degree
	{
		return Matrix3x3<T>().SetDegRotate(angle);
	}

	/*!***********************************************************************
	 \brief (RADIAN)
	 Non-Member SetRadRotate function
	 Translation of the Matrix, when you set x and y to its value, it will change the matrix to set the translate Matrix

	 | 1  0  0 |		| cos  -sin  x |
	 | 0  1  0 |   ->	| sin  cos	 y |
	 | 0  0  1 |		| 0		0	 1 |

	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> SetRadRotate(double angle) //in degree
	{
		return Matrix3x3<T>().SetRadRotate(angle);
	}

	/*!***********************************************************************
	 \brief
	 Non-Member SetTranslate function
	 Translation of the Matrix, when you set x and y to its value, it will change the matrix to set the translate Matrix

	 | 1  0  0 |		| 1  0  x |
	 | 0  1  0 |   ->	| 0  1  y |
	 | 0  0  1 |		| 0  0  1 |

	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> SetTranslate(T x, T y)
	{
		return Matrix3x3<T>().SetTranslate(x, y);
	}

	/*!***********************************************************************
	 \brief
	 Non-Member SetTransform function
	 Transforming of the Matrix, where you multiply S*R*T

	 \return
	 Matrix3x3
	*************************************************************************/
	template<typename T>
	Matrix3x3<T> SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& rot, Matrix3x3<T> const& scale)
	{
		return Matrix3x3<T>().SetTransform(trans, rot, scale);
	}
	/*******************************************************************************************/
}