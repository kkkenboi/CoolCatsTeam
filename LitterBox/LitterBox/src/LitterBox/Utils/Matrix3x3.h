#include "LitterBox/Utils/Math.h"

namespace LB
{
	template<typename T>
	class Matrix3x3
	{
		public:
		T m[3][3];
		T a, b, c, d, e, f, g, h, i;
		Matrix3x3(T a, T b, T c, T d, T e, T f, T g, T h, T i);

		Matrix3x3 Identity();
		Matrix3x3 Zero();
		double	  Determinant();
		Matrix3x3 Inverse();
		Matrix3x3 Transpose();

		void SetScale	  (T x, T y);
		void SetTranslate (T x, T y);
		void SetRotate	  (double angle);
		void SetTransform (Matrix3x3<T> const& trans, Matrix3x3<T> const& scale, Matrix3x3<T> const& rot);
	};

	template<typename T>
	Matrix3x3<T> SetScale(T x, T y);

	template<typename T>
	Matrix3x3<T> SetRotate(double angle);

	template<typename T>
	Matrix3x3<T> SetTranslate(T x, T y);

	template<typename T>
	Matrix3x3<T> SetTransform(Matrix3x3<T> const& trans, Matrix3x3<T> const& scale, Matrix3x3<T> const& rot);
}
