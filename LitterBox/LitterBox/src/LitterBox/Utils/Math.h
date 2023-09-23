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
	#define PI 3.14159265358979323846

	template <typename T>
	T Clamp(T value, T min, T max);

	/***************************************************************************************************
	*
	* Vec 2
	* 
	*
	***************************************************************************************************/

	template<typename T>
	class Vec2 
	{
		public:
		// Data members
		T x;
		T y;
			
		// Constructors
		Vec2();								 // Default constructor
		Vec2(T x, T y);						 // Parameterized Constructor
		Vec2(const Vec2<T>& rhs);			 // Copy constructor
		Vec2& operator=(const Vec2<T>& rhs); // Copy assignment

		// Basic operators + - []
		Vec2 operator+(Vec2<T> rhs)   const;
		Vec2 operator+(T rhs)		  const;
		Vec2 operator-(Vec2<T> rhs)   const;
		Vec2 operator-(T rhs)		  const;
		T    operator[](size_t index) const;		
		T&   operator[](size_t index);		 // can edit the actual thing

		// Compound assignment operators -= +=
		Vec2& operator-=(const Vec2<T> rhs);
		Vec2& operator+=(const Vec2<T> rhs);

		// Comparison operators < > == != <= >=
		bool operator==(Vec2<T> const& rhs) const;
		bool operator!=(Vec2<T> const& rhs) const;
		bool operator< (Vec2<T> const& rhs) const;
		bool operator<=(Vec2<T> const& rhs) const;
		bool operator> (Vec2<T> const& rhs) const;
		bool operator>=(Vec2<T> const& rhs) const;

		// Vec2 Member functions
		Vec2<T> Zero(); // fill up all to 0
		Vec2<T> One();  // fill up all to 1

		Vec2<T>& Normalise()	 const;
		T		 Length()		 const;
		T		 LengthSquared() const;

		Vec2<T>& Set(T x, T y);
		Vec2<T>& Set(Vec2<T> const& rhs);
	};

	// Vec2 Non-member operator overloads * / *= /=
	template<typename T1, typename T2>
	Vec2<T1> operator*(Vec2<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec2<T1> operator*=(Vec2<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec2<T1> operator/(Vec2<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec2<T1> operator/=(Vec2<T1> lhs, T2 rhs);

	// Vec2 Non-member functions 
	template<typename T>
	Vec2<T> Lerp(Vec2<T> const& lhs, Vec2<T> const& rhs, float percentage);

	template<typename T>
	Vec2<T> Normalise(Vec2<T> const& rhs);

	template<typename T>
	T Distance(Vec2<T> const& lhs, Vec2<T> const& rhs);

	template<typename T>
	T DotProduct(Vec2<T> const& lhs, Vec2<T> const& rhs);


	/***************************************************************************************************
	*
	* Vec 3
	* 
	*
	***************************************************************************************************/
	template <typename T>
	class Vec3
	{
	public:
		// Data members
		T x;
		T y;
		T z;

		// Constructors
		Vec3();								 // Default constructor
		Vec3(T x, T y, T z);				 // Parameterized Constructor
		Vec3(const Vec3<T>& rhs);			 // Copy constructor
		Vec3& operator=(const Vec3<T>& rhs); // Copy assignment

		// Basic operators + - []
		Vec3 operator+(Vec3<T> rhs)   const;
		Vec3 operator+(T rhs)		  const;
		Vec3 operator-(Vec3<T> rhs)   const;
		Vec3 operator-(T rhs)		  const;
		T    operator[](size_t index) const;
		T& operator[](size_t index);		 // can edit the actual thing

		// Compound assignment operators -= +=
		Vec3& operator-=(const Vec3<T> rhs);
		Vec3& operator+=(const Vec3<T> rhs);

		// Comparison operators < > == != <= >=
		bool operator==(Vec3<T> const& rhs) const;
		bool operator!=(Vec3<T> const& rhs) const;
		bool operator< (Vec3<T> const& rhs) const;
		bool operator<=(Vec3<T> const& rhs) const;
		bool operator> (Vec3<T> const& rhs) const;
		bool operator>=(Vec3<T> const& rhs) const;

		// Vec3 Member functions
		Vec3<T> Zero(); // fill up all to 0
		Vec3<T> One();  // fill up all to 1

		Vec3<T>& Normalise()	 const;
		T		 Length()		 const;
		T		 LengthSquared() const;

		Vec3<T>& Set(T x, T y, T z);
		Vec3<T>& Set(Vec3<T> const& rhs);
	};

	// Vec3 Non-member operator overloads * / *= /=
	template<typename T1, typename T2>
	Vec3<T1> operator*(Vec3<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec3<T1> operator*=(Vec3<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec3<T1> operator/(Vec3<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec3<T1> operator/=(Vec3<T1> lhs, T2 rhs);

	// Vec3 Non-member functions 
	template<typename T>
	Vec3<T> Lerp(Vec3<T> const& lhs, Vec3<T> const& rhs, float percentage);

	template<typename T>
	Vec3<T> Normalise(Vec3<T> const& rhs);

	template<typename T>
	T Distance(Vec3<T> const& lhs, Vec3<T> const& rhs);

	template<typename T>
	T DotProduct(Vec3<T> const& lhs, Vec3<T> const& rhs);

	template<typename T>
	Vec3<T> CrossProduct(Vec3<T> const& lhs, Vec3<T> const& rhs);


	/***************************************************************************************************
	*
	* Vec 4
	* 
	*
	***************************************************************************************************/
	template <typename T>
	class Vec4
	{
	public:
		// Data members
		T x;
		T y;
		T z;
		T w;

		// Constructors
		Vec4();								 // Default constructor
		Vec4(T x, T y, T z, T w);				 // Parameterized Constructor
		Vec4(const Vec4<T>& rhs);			 // Copy constructor
		Vec4& operator=(const Vec4<T>& rhs); // Copy assignment

		// Basic operators + - []
		Vec4 operator+(Vec4<T> rhs)   const;
		Vec4 operator+(T rhs)		  const;
		Vec4 operator-(Vec4<T> rhs)   const;
		Vec4 operator-(T rhs)		  const;
		T    operator[](size_t index) const;
		T& operator[](size_t index);		 // can edit the actual thing

		// Compound assignment operators -= +=
		Vec4& operator-=(const Vec4<T> rhs);
		Vec4& operator+=(const Vec4<T> rhs);

		// Comparison operators < > == != <= >=
		bool operator==(Vec4<T> const& rhs) const;
		bool operator!=(Vec4<T> const& rhs) const;
		bool operator< (Vec4<T> const& rhs) const;
		bool operator<=(Vec4<T> const& rhs) const;
		bool operator> (Vec4<T> const& rhs) const;
		bool operator>=(Vec4<T> const& rhs) const;

		// Vec4 Member functions
		Vec4<T> Zero(); // fill up all to 0
		Vec4<T> One();  // fill up all to 1

		Vec4<T>& Normalise()	 const;
		T		 Length()		 const;
		T		 LengthSquared() const;

		Vec4<T>& Set(T x, T y, T z, T w);
		Vec4<T>& Set(Vec4<T> const& rhs);
	};

	// Vec4 Non-member operator overloads * / *= /=
	template<typename T1, typename T2>
	Vec4<T1> operator*(Vec4<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec4<T1> operator*=(Vec4<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec4<T1> operator/(Vec4<T1> lhs, T2 rhs);

	template<typename T1, typename T2>
	Vec4<T1> operator/=(Vec4<T1> lhs, T2 rhs);

	// Vec4 Non-member functions 
	template<typename T>
	Vec4<T> Lerp(Vec4<T> const& lhs, Vec4<T> const& rhs, float percentage);

	template<typename T>
	Vec4<T> Normalise(Vec4<T> const& rhs);

	template<typename T>
	T Distance(Vec4<T> const& lhs, Vec4<T> const& rhs);

	template<typename T>
	T DotProduct(Vec4<T> const& lhs, Vec4<T> const& rhs);

	/***************************************************************************************************
	*
	* Vec casting
	*
	***************************************************************************************************/
	template<typename T>
	Vec2<T> ToVector2(Vec3<T> const& rhs); // Vec3 to Vec2

	template<typename T>
	Vec2<T> ToVector2(Vec4<T> const& rhs); // Vec4 to Vec2

	template<typename T>
	Vec3<T> ToVector3(Vec2<T> const& rhs); // Vec2 to Vec3

	template<typename T>
	Vec3<T> ToVector3(Vec4<T> const& rhs); // Vec4 to Vec3

	template<typename T>
	Vec4<T> ToVector4(Vec2<T> const& rhs); // Vec2 to Vec4

	template<typename T>
	Vec4<T> ToVector4(Vec3<T> const& rhs); // Vec3 to Vec4
}
