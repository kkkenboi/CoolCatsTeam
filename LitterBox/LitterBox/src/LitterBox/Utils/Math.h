/*!************************************************************************
 \file			Math.h
 \author		
 \par DP email: 
 \par Course:	CSD2401A
 \date			10-09-2023
 \brief

**************************************************************************/

#pragma once

namespace LB 
{
	/***************************************************************************************************
	*
	* Vec 2
	* - takes in a template class
	*
	***************************************************************************************************/

	template<class T>
	struct Vec2 
	{
		T x;
		T y;
			
		Vec2(); //Default constructor
		Vec2(T x, T y); //Parameterized Constructor
		Vec2(const Vec2<T>& rhs); //Copy constructor
		Vec2& operator=(const Vec2<T>& rhs); //Copy assignment
		Vec2 operator+(Vec2<T> rhs) const;
		Vec2 operator-(Vec2<T> rhs) const;
		T& operator[](size_t index); //can edit the actual thing
		T operator[](size_t index) const;
		
		Vec2<T> Zero(); //fill up all to 0
		Vec2<T> One(); //fill up all to 1

		//OPERATOR OVER LOADS FOR VECTOR MULTIPLICATION
		Vec2& operator-=(const Vec2<T> rhs);
		Vec2& operator+=(const Vec2<T> rhs);
		Vec2& operator*=(const Vec2<T> rhs);
		Vec2& operator/=(const Vec2<T> rhs);
	};

	template<typename T1, typename T2>
	Vec2<T1> operator*(Vec2<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec2<T1> operator/(Vec2<T1> lhs, T2 rhs);

	/***************************************************************************************************
	*
	* Vec 3
	* - takes in a template class
	*
	***************************************************************************************************/

	template<class T>
	struct Vec3
	{
		
		T x;
		T y;
		T z;

		Vec3(); //Default constructor
		Vec3(T x, T y, T z); //Parameterized Constructor
		Vec3(const Vec3<T>& rhs); //Copy constructor
		Vec3& operator=(const Vec3<T>& rhs); //Copy assignment
		Vec3 operator+(Vec3<T> rhs) const;
		Vec3 operator-(Vec3<T> rhs) const;
		T& operator[](size_t index); //can edit the actual thing
		T operator[](size_t index) const;

		Vec3<T> Zero(); //fill up all to 0
		Vec3<T> One(); //fill up all to 1

		Vec3& operator-=(const Vec3<T> rhs);
		Vec3& operator+=(const Vec3<T> rhs);
		Vec3& operator*=(const Vec3<T> rhs);
		Vec3& operator/=(const Vec3<T> rhs);
	};

	template<typename T1, typename T2>
	Vec3<T1> operator*(Vec3<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec3<T1> operator/(Vec3<T1> lhs, T2 rhs);

	/***************************************************************************************************
	*
	* Vec 4
	* - takes in a template class
	*
	***************************************************************************************************/

	template<class T>
	struct Vec4
	{
		T x;
		T y;
		T z;
		T w;

		Vec4(); //Default constructor
		Vec4(T x, T y, T z, T w); //Parameterized Constructor
		Vec4(const Vec4<T>& rhs); //Copy constructor
		Vec4& operator=(const Vec4<T>& rhs); //Copy assignment
		Vec4 operator+(Vec4<T> rhs) const;
		Vec4 operator-(Vec4<T> rhs) const;
		T& operator[](size_t index); //can edit the actual thing
		T operator[](size_t index) const;

		Vec4<T> Zero(); //fill up all to 0
		Vec4<T> One(); //fill up all to 1

		Vec4& operator-=(const Vec4<T> rhs);
		Vec4& operator+=(const Vec4<T> rhs);
		Vec4& operator*=(const Vec4<T> rhs);
		Vec4& operator/=(const Vec4<T> rhs);
	};

	template<typename T1, typename T2>
	Vec4<T1> operator*(Vec4<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec4<T1> operator/(Vec4<T1> lhs, T2 rhs);
}



///*!***********************************************************************
//\brief
//	Compare the equality of both Vector 4
//\return
//	Return true if both vector are equal. Else return false.
//*************************************************************************/
//template<class T>
//bool operator==(const Vec4<T>& lhs, const Vec4<T>& rhs)
//{
//	if (lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z) {
//		return true;
//	}
//	return false;
//}
//
///*!***********************************************************************
//\brief
//	Compare the equality of both Vector 3
//\return
//	Return true if both vector are equal. Else return false.
//*************************************************************************/
//template<class T>
//bool operator==(const Vec3<T>& lhs, const Vec3<T>& rhs)
//{
//	return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
//}
//
///*!***********************************************************************
//\brief
//	Compare the equality of both Vector 2
//\return
//	Return true if both vector are equal. Else return false.
//*************************************************************************/
//template<class T>
//bool operator==(const Vec2<T>& lhs, const Vec2<T>& rhs)
//{
//	return (lhs.x == rhs.x && lhs.y == rhs.y);
//}
//
//template<class T>
//bool operator!=(const Vec2<T>& lhs, const Vec2<T>& rhs)
//{
//	return !(lhs.x == rhs.x && lhs.y == rhs.y);
//}
//
///*============================================================================
//* OPERATOR OVER LOADS FOR VECTOR ADDITION
//*/
//template<class T>
//Vec2<T>& Vec2<T>::operator+=(Vec2<T>const& rhs)		//vec2+=vec2
//{	
//	x += rhs.x;
//	y += rhs.y;
//	return*this;
//
//}
//
//template <class T>
//Vec3<T>& Vec3<T>::operator+=(Vec2<T> const& rhs)	//vec3+=vec2
//{
//	x += rhs.x;
//	y += rhs.y;
//	return*this;
//}
//
//template <class T>
//Vec3<T>& Vec3<T>::operator+=(Vec3<T> const& rhs) 	//vec3+=vec3
//{
//	x += rhs.x;
//	y += rhs.y;
//	z += rhs.z;
//	return*this;
//}
//
//template <class T>
//Vec4<T>& Vec4<T>::operator+=(Vec2<T> const& rhs) 	//vec4+=vec2
//{
//	x += rhs.x;
//	y += rhs.y;
//	return *this;
//}
//
//template <class T>
//Vec4<T>& Vec4<T>::operator+=(Vec3<T> const& rhs) 	//vec4+=vec3
//{
//	x += rhs.x;
//	y += rhs.y;
//	z += rhs.z;
//	return *this;
//}
//
//template <class T>
//Vec4<T>& Vec4<T>::operator+=(Vec4<T> const& rhs) 	//vec4+=vec4
//{
//	w += rhs.w;
//	x += rhs.x;
//	y += rhs.y;
//	z += rhs.z;
//	return*this;
//}
//
////BINARY ADDITION
//template<class T>
//Vec2<T> operator+(Vec2<T> const& lhs, Vec2<T>const& rhs)	//vec2+vec2
//{	
//	Vec2<T> temp{ lhs };
//	temp += rhs;
//	return temp;
//}
//
//template<class T>
//Vec3<T> operator+(Vec3<T> const& lhs, Vec3<T>const& rhs)	//vec3+vec3
//{	
//	Vec3<T> temp{ lhs };
//	temp += rhs;
//	return temp;
//}
//
//template<class T>
//Vec3<T> operator+(Vec3<T> const& lhs, Vec2<T>const& rhs)	//vec3+vec2
//{	
//	Vec3<T> temp{ lhs };
//	temp += rhs;
//	return temp;
//}
//
//template<class T>
//Vec3<T> operator+(Vec2<T> const& lhs, Vec3<T>const& rhs)	//vec2+vec3
//{	
//	Vec3<T> temp{ rhs };
//	temp += lhs;
//	return temp;
//}
//
//template<class T>
//Vec4<T> operator+(Vec3<T> const& lhs, Vec4<T>const& rhs)	//vec3+vec4
//{	
//	Vec4<T> temp{ rhs };
//	temp += lhs;
//	return temp;
//}
//
//template<class T>
//Vec4<T> operator+(Vec2<T> const& lhs, Vec4<T>const& rhs)	//vec2+vec4
//{	
//	Vec4<T> temp{ rhs };
//	temp += lhs;
//	return temp;
//}
//
//template<class T>
//Vec4<T> operator+(Vec4<T> const& lhs, Vec2<T>const& rhs)	//vec4+vec2
//{	
//	Vec4<T> temp{ lhs };
//	temp += rhs;
//	return temp;
//}
//
//template<class T>
//Vec4<T> operator+(Vec4<T> const& lhs, Vec3<T>const& rhs)	//vec4+vec3
//{	
//	Vec4<T> temp{ lhs };
//	temp += rhs;
//	return temp;
//}
//
//template<class T>
//Vec4<T> operator+(Vec4<T> const& lhs, Vec4<T>const& rhs)	//vec4+vec4
//{	
//	Vec4<T> temp{ lhs };
//	temp += rhs;
//	return temp;
//}
//
///*============================================================================
//* OPERATOR OVER LOADS FOR VECTOR SUBTRACTION
//*/
//template<class T>
//Vec2<T>& Vec2<T>::operator-=(Vec2<T>const& rhs)		//vec2-=vec2
//{	
//	x -= rhs.x;
//	y -= rhs.y;
//	return*this;
//}
//
//template <class T>
//Vec3<T>& Vec3<T>::operator-=(Vec2<T> const& rhs) 	// vec3-=vec2
//{
//	x -= rhs.x;
//	y -= rhs.y;
//	return *this;
//}
//template<class T>
//Vec3<T>& Vec3<T>::operator-=(Vec3<T>const& rhs)		//vec3-=vec3
//{ 	
//	x -= rhs.x;
//	y -= rhs.y;
//	z -= rhs.z;
//	return*this;
//}
//template <class T>
//Vec4<T>& Vec4<T>::operator-=(Vec2<T> const& rhs) 	//vec4-=vec2
//{
//	x -= rhs.x;
//	y -= rhs.y;
//	return *this;
//}
//template <class T>
//Vec4<T>& Vec4<T>::operator-=(Vec3<T> const& rhs) 	//vec4-=vec3
//{
//	x -= rhs.x;
//	y -= rhs.y;
//	z -= rhs.z;
//	return *this;
//}
//template<class T>
//Vec4<T>& Vec4<T>::operator-=(Vec4<T>const& rhs)		//vec4-=vec4
//{ 
//	w -= rhs.w;
//	x -= rhs.x;
//	y -= rhs.y;
//	z -= rhs.z;
//	return*this;
//}
//
////BINARY SUBTRACTION
//template<class T>
//Vec2<T> operator-(Vec2<T> const& lhs, Vec2<T>const& rhs)	//vec2-vec2
//{	
//	Vec2<T> temp{ lhs };
//	temp -= rhs;
//	return temp;
//}
//
//template<class T>
//Vec3<T> operator-(Vec3<T> const& lhs, Vec2<T>const& rhs)	//vec3-vec2
//{	
//	Vec3<T> temp{ lhs };
//	temp -= rhs;
//	return temp;
//}
//
//template<class T>
//Vec3<T> operator-(Vec3<T> const& lhs, Vec3<T>const& rhs)	//vec3-vec3
//{	
//	Vec3<T> temp{ lhs };
//	temp -= rhs;
//	return temp;
//}
//
//template<class T>
//Vec4<T> operator-(Vec4<T> const& lhs, Vec2<T>const& rhs)	//vec4-vec2
//{	
//	Vec4<T> temp{ lhs };
//	temp -= rhs;
//	return temp;
//}
//
//template<class T>
//Vec4<T> operator-(Vec4<T> const& lhs, Vec3<T>const& rhs)	//vec4-vec3
//{	
//	Vec4<T> temp{ lhs };
//	temp -= rhs;
//	return temp;
//}
//
//template<class T>
//Vec4<T> operator-(Vec4<T> const& lhs, Vec4<T>const& rhs)	//vec4-vec4
//{	
//	Vec4<T> temp{ lhs };
//	temp -= rhs;
//	return temp;
//}
//
///*============================================================================
//* OPERATOR OVER LOADS FOR VECTOR MULTIPLICATION
//*/
//template <class T>
//Vec2<T>& Vec2<T>::operator*=(T rhs)
//{
//	x *= rhs;
//	y *= rhs;
//	return *this;
//}
//
//template <class T>
//Vec2<T>& Vec2<T>::operator/=(Vec2<T> const& rhs)
//{
//	x /= rhs;
//	y /= rhs;
//	return *this;
//}
//
//template <class T>
//Vec3<T>& Vec3<T>::operator*=(T rhs)
//{
//	x *= rhs;
//	y *= rhs;
//	z *= rhs;
//	return *this;
//}
//
//template <class T>
//Vec3<T>& Vec3<T>::operator/=(Vec2<T> const& rhs)
//{
//	x /= rhs;
//	y /= rhs;
//	z /= rhs;
//	return *this;
//}
//
//template <class T>
//Vec4<T>& Vec4<T>::operator*=(T rhs)
//{
//	x *= rhs;
//	y *= rhs;
//	z *= rhs;
//	w *= rhs;
//	return *this;
//}
//
//template<class T>
//Vec4<T>& Vec4<T>::operator/=(Vec2<T> const& rhs)
//{
//	x /= rhs;
//	y /= rhs;
//	z /= rhs;
//	w /= rhs;
//	return *this;
//}
//
//
//template<class T>
//Vec2<T> operator*(Vec2<T> const& lhs, T rhs) {
//	Vec2<T> temp{ lhs };
//	temp.x *= rhs;
//	temp.y *= rhs;
//	return temp;
//}
//
//template<class T>
//Vec3<T> operator*(Vec3<T> const& lhs, T rhs) {
//	Vec3<T> temp{ lhs };
//	temp.x *= rhs;
//	temp.y *= rhs;
//	temp.z *= rhs;
//	return temp;
//}
//
//template<class T>
//Vec4<T> operator*(Vec4<T> const& lhs, T rhs) {
//	Vec4<T> temp{ lhs };
//	temp.x *= rhs;
//	temp.y *= rhs;
//	temp.z *= rhs;
//	temp.w *= rhs;
//	return temp;
//}
//
///*============================================================================
//* OPERATOR OVERLOADS FOR VECTOR COMPARISONS
//*/
//template<class T>
//bool operator<(Vec2<T> const& lhs, Vec2<T> const& rhs) {
//	if (lhs.x < rhs.x) return true;
//	if (lhs.x == rhs.x) {
//		return (lhs.y < rhs.y);
//	}
//	return false;
//}
//
///*============================================================================
//* OPERATOR OVER LOADS FOR VECTOR TO OUTPUT STREAM
//*/
//
///*!***********************************************************************
//\brief
//	Vec2 to output stream.
//*************************************************************************/
//template <class T>
//inline std::ostream& operator<<(std::ostream& os, Vec2<T> const& rhs)
//{
//	os << "(x,y) : ";
//	return os << "(" << rhs.x << "," << rhs.y << ")";
//}
//
///*!***********************************************************************
//\brief
//	Vec3 to output stream.
//*************************************************************************/
//template <class T>
//inline std::ostream& operator<<(std::ostream& os, Vec3<T> const& rhs)
//{
//	os << "(x,y,z) : ";
//	return os << "(" << rhs.x << "," << rhs.y << "," << rhs.z << ")";
//}
//
///*!***********************************************************************
//\brief
//	Vec4 to output stream.
//*************************************************************************/
//template <class T>
//inline std::ostream& operator<<(std::ostream& os, Vec4<T> const& rhs)
//{
//	os << "(x,y,z,w) : ";
//	return os << "(" << rhs.x << "," << rhs.y << "," << rhs.z << "," << rhs.w << ")";
//}
//
///*!***********************************************************************
//\brief
//	Check of a vec2 point is within a area.
//	Pivot: Top left.
//\return
//	True / false.
//*************************************************************************/
//template <typename T>
//inline bool IsPointWithinRect(Vec2<T> point, Vec2<T> pos, Vec2<T> size) 
//{
//	if (point.x < pos.x || point.x > pos.x + size.x ||
//		point.y < pos.y - size.y || point.y > pos.y) return false;
//	return true;
//}
