#include "LitterBox/Utils/Math.h"

//diff orientation
//diff sizes
//diff location

//TSR
// T - Translate
// S - Scale
// R - Rotate
// 2 axis (x,y)

namespace LB
{
	template<typename T>
	union Matrix3x3
	{
		struct
		{
			T m00, m01, m02;
			T m10, m11, m12;
			T m20, m21, m22;
		};
		T arr[3][3];
		T matrix[9];


	};

}
