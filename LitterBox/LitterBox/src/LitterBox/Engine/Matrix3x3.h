#include <cmath>
#include <vector>
#include "LitterBox/Utils/Math.h"


//diff orientation
//diff sizes
//diff location

//TSR
// T - Translate
// S - Scale
// R - Rotate
// 2 axis (x,y)

namespace LBMatrix
{
	class Matrix3x3
	{
	public:
		struct matrix
		{
			float m00, m01, m02;
			float m10, m11, m12;
			float m20, m21, m22;

			//matrix();
		} matrix[9];
		
		Matrix3x3();
		~Matrix3x3();

		//float getMtxElement();
		//void setMtxElement();

		//Matrix3x3(const float* pArr);

		Matrix3x3(float a11, float a12, float a13,
				float a21, float a22, float a23,
				float a31, float a32, float a33);

		Matrix3x3& operator=(const Matrix3x3& rhs);

		Matrix3x3(const Matrix3x3& rhs) = default;


		Matrix3x3& operator *= (const Matrix3x3& rhs);


	private:
		float matrix[9];
		std::vector<std::vector<float>> vec;
		//float** data;
	};
		
}


//typedef union Matrix3x3
//{
//	struct
//	{
//		float m00, m01, m02;
//		float m10, m11, m12;
//		float m20, m21, m22;
//	};

//	Matrix3x3() : m00(0.0f), m01(0.0f), m02(0.0f), 
//			m10(0.0f), m11(0.0f), m12(0.0f), 
//			m20(0.0f), m21(0.0f), m22(0.0f) {}
//};

//class Matrix
//{
//	public:
//		Matrix(int numRows, int numCols);
//		~Matrix();
//		float getMtxElement();
//		void setMtxElement(int numRows, int numCols);

//	private:
//		int rows;
//		int cols;
//		float** data;
//};