#include "Matrix3x3.h"
#include <cmath>
#define PI 3.14159265358979323846

namespace LBMatrix
{
	//Matrix3x3::matrix::matrix()
	//{
	//	m00 = 0.0f; m01 = 0.0f; m02 = 0.0f;
	//	m10 = 0.0f; m11 = 0.0f; m12 = 0.0f;
	//	m20 = 0.0f; m21 = 0.0f; m22 = 0.0f;
	//}

	//Matrix3x3::Matrix3x3()
	//{
	//	//matrix.m00 = 0.0f; matrix.m01 = 0.0f; matrix.m02 = 0.0f;
	//	//matrix.m10 = 0.0f; matrix.m11 = 0.0f; matrix.m12 = 0.0f;
	//	//matrix.m20 = 0.0f; matrix.m21 = 0.0f; matrix.m22 = 0.0f;
	//}

	//Matrix3x3::~Matrix3x3() {}

	//Matrix3x3::Matrix3x3(float a11, float a12, float a13, 
	//					float a21, float a22, float a23, 
	//					float a31, float a32, float a33)
	//{
	//	vec[0] = { a11, a12, a13 };
	//	vec[1] = { a21, a22, a23 };
	//	vec[2] = { a31, a32, a33 };
	//}

	//Matrix3x3& Matrix3x3::operator=(const Matrix3x3& rhs)
	//{
	//	// TODO: insert return statement here
	//	matrix.m00 = rhs.matrix.m00; matrix.m01 = rhs.matrix.m01; matrix.m02 = rhs.matrix.m02;
	//	matrix.m10 = rhs.matrix.m10; matrix.m11 = rhs.matrix.m11; matrix.m12 = rhs.matrix.m12;
	//	matrix.m20 = rhs.matrix.m20; matrix.m21 = rhs.matrix.m21; matrix.m22 = rhs.matrix.m22;
	//	return *this;
	//}

	//Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& rhs)
	//{
	//	// TODO: insert return statement here
	//	Matrix3x3 newM;

	//	newM.matrix.m00 = matrix.m00 * rhs.matrix.m00 + matrix.m01 * rhs.matrix.m10 + matrix.m02 * rhs.matrix.m20;
	//	newM.matrix.m10 = matrix.m10 * rhs.matrix.m00 + matrix.m11 * rhs.matrix.m10 + matrix.m12 * rhs.matrix.m20;
	//	newM.matrix.m20 = matrix.m20 * rhs.matrix.m00 + matrix.m21 * rhs.matrix.m10 + matrix.m22 * rhs.matrix.m20;


	//	newM.matrix.m01 = matrix.m00 * rhs.matrix.m01 + matrix.m01 * rhs.matrix.m11 + matrix.m02 * rhs.matrix.m21;
	//	newM.matrix.m11 = matrix.m10 * rhs.matrix.m01 + matrix.m11 * rhs.matrix.m11 + matrix.m12 * rhs.matrix.m21;
	//	newM.matrix.m21 = matrix.m20 * rhs.matrix.m01 + matrix.m21 * rhs.matrix.m11 + matrix.m22 * rhs.matrix.m21;

	//	newM.matrix.m02 = matrix.m00 * rhs.matrix.m02 + matrix.m01 * rhs.matrix.m12 + matrix.m02 * rhs.matrix.m22;
	//	newM.matrix.m12 = matrix.m10 * rhs.matrix.m02 + matrix.m11 * rhs.matrix.m12 + matrix.m12 * rhs.matrix.m22;
	//	newM.matrix.m22 = matrix.m20 * rhs.matrix.m02 + matrix.m21 * rhs.matrix.m12 + matrix.m22 * rhs.matrix.m22;

	//	return newM;
	//}



	
	

	//float Matrix3x3::getMtxElement()
	//{
	//	return 0.0f;
	//}


	//void Matrix3x3::setMtxElement()
	//{
	//}
}