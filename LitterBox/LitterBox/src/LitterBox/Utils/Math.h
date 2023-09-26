/*!************************************************************************
 \file			Math.h
 \author		Vanessa Chua Siew Jin, Justine
 \par DP email:
 \par Course:	CSD2401A
 \date			24-09-2023
 \brief

**************************************************************************/

#pragma once
#include <cmath> // For sin, cos, sqrt

namespace LB 
{
	#define PI 3.14159265358979323846

	/***********************************************************************/
	//Justine's
	template <typename T>
	T Clamp(T value, T min, T max);
	/***********************************************************************/

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

		/***********************************************************************/
		//Justine's
		Vec2<T>& Normalise()	 const;
		T		 Length()		 const;
		T		 LengthSquared() const;
		/***********************************************************************/

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

	/***********************************************************************/
	//Justine's
	template<typename T>
	Vec2<T> Normalise(Vec2<T> const& rhs);

	template<typename T>
	T Distance(Vec2<T> const& lhs, Vec2<T> const& rhs);

	template<typename T>
	T DotProduct(Vec2<T> const& lhs, Vec2<T> const& rhs);
	/***********************************************************************/


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

		/***********************************************************************/
		//Justine's
		Vec3<T>& Normalise()	 const;
		T		 Length()		 const;
		T		 LengthSquared() const;
		/***********************************************************************/

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

	/***********************************************************************/
	//Justine's
	template<typename T>
	Vec3<T> Normalise(Vec3<T> const& rhs);

	template<typename T>
	T Distance(Vec3<T> const& lhs, Vec3<T> const& rhs);

	template<typename T>
	T DotProduct(Vec3<T> const& lhs, Vec3<T> const& rhs);

	template<typename T>
	Vec3<T> CrossProduct(Vec3<T> const& lhs, Vec3<T> const& rhs);
	/***********************************************************************/


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

		/***********************************************************************/
		//Justine's
		Vec4<T>& Normalise()	 const;
		T		 Length()		 const;
		T		 LengthSquared() const;
		/***********************************************************************/

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

	/***********************************************************************/
	//Justine's
	template<typename T>
	Vec4<T> Normalise(Vec4<T> const& rhs);

	template<typename T>
	T Distance(Vec4<T> const& lhs, Vec4<T> const& rhs);

	template<typename T>
	T DotProduct(Vec4<T> const& lhs, Vec4<T> const& rhs);
	/***********************************************************************/

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

/***************************************************************************************************
*
* Vec definitions
*
***************************************************************************************************/

namespace LB
{
	/***********************************************************************/
	//Justine's
	template <typename T>
	T Clamp(T value, T min, T max)
	{
		return (value < min) ? min : (value > max) ? max : value;
	}
	//************************************************************************

	/***************************************************************************************************
	*
	* Vec 2
	*
	*
	***************************************************************************************************/

	/**************************************************************************************************/
	// Vec2 Constructors
	// Default constructor 
	template<typename T>
	Vec2<T>::Vec2() : x((T)0), y((T)0) {}

	// Parameterized Constructor
	template<typename T>
	Vec2<T>::Vec2(T x, T y) : x(x), y(y) {}

	// Copy constructor
	template<typename T>
	Vec2<T>::Vec2(const Vec2<T>& rhs) : x(rhs.x), y(rhs.y) {}

	// Copy assignment constructor
	template<typename T>
	Vec2<T>& Vec2<T>::operator=(const Vec2<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	/**************************************************************************************************/
	// Vec2 Basic operators + - []

	template<typename T>
	Vec2<T> Vec2<T>::operator+(Vec2<T> rhs) const
	{
		return Vec2<T>(x + rhs.x, y + rhs.y);
	}

	template<typename T>
	Vec2<T> Vec2<T>::operator+(T rhs) const
	{
		return Vec2<T>(x + rhs, y + rhs);
	}

	template<typename T>
	Vec2<T> Vec2<T>::operator-(Vec2<T> rhs) const
	{
		return Vec2<T>(x - rhs.x, y - rhs.y);
	}

	template<typename T>
	Vec2<T> Vec2<T>::operator-(T rhs) const
	{
		return Vec2<T>(x - rhs, y - rhs);
	}

	template<typename T>
	T& Vec2<T>::operator[](size_t index)
	{
		if (index == 0)
		{
			return this->x;
		}
		return this->y;
	}

	template<typename T>
	T Vec2<T>::operator[](size_t index) const
	{
		if (index == 0)
		{
			return this->x;
		}
		return this->y;
	}

	/**************************************************************************************************/
	// Vec2 Compound assignment operators -= += *= /=

	template<typename T>
	Vec2<T>& Vec2<T>::operator-=(const Vec2<T> rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;

		return *this;
	}

	template<typename T>
	Vec2<T>& Vec2<T>::operator+=(const Vec2<T> rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;

		return *this;
	}

	/**************************************************************************************************/
	// Vec2 Comparison operators < > == != <= >=

	template<typename T>
	bool Vec2<T>::operator==(const Vec2<T>& rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y);
	}

	template<typename T>
	bool Vec2<T>::operator!=(Vec2<T> const& rhs) const
	{
		return !(this == rhs);
	}

	template<typename T>
	bool Vec2<T>::operator< (const Vec2<T>& rhs) const
	{
		return this->LengthSquared() < rhs.LengthSquared(); //cheaper to use LengthSquared, dont waste time calculating sqrt
	}

	template<typename T>
	bool Vec2<T>::operator<=(Vec2<T> const& rhs) const
	{
		return this->LengthSquared() <= rhs.LengthSquared();
	}

	template<typename T>
	bool Vec2<T>::operator> (Vec2<T> const& rhs) const
	{
		return rhs <= this;
	}

	template<typename T>
	bool Vec2<T>::operator>=(Vec2<T> const& rhs) const
	{
		return rhs < this;
	}

	/**************************************************************************************************/
	// Vec2 Member functions

	template<typename T>
	Vec2<T> Vec2<T>::Zero()
	{
		return Vec2<T>((T)0, (T)0);
	}

	template<typename T>
	Vec2<T> Vec2<T>::One()
	{
		return Vec2<T>((T)1, (T)1);
	}

	//***********************************************************************
	//Justine's
	template<typename T>
	Vec2<T>& Vec2<T>::Normalise() const
	{
		T length = Length();

		x /= length;
		y /= length;

		return *this;
	}

	template<typename T>
	T Vec2<T>::Length() const
	{
		return (T)sqrt((double)LengthSquared());
	}

	template<typename T>
	T Vec2<T>::LengthSquared() const
	{
		return (x * x + y * y);
	}
	//***********************************************************************

	template<typename T>
	Vec2<T>& Vec2<T>::Set(T _x, T _y)
	{
		x = _x;
		y = _y;

		return *this;
	}

	template<typename T>
	Vec2<T>& Vec2<T>::Set(Vec2<T> const& rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	/**************************************************************************************************/
	// Vec2 Non-member functions

	template<typename T>
	Vec2<T> Lerp(Vec2<T> const& lhs, Vec2<T> const& rhs, float percentage) //lhs - startpos, rhs - endpos, percentage 0-1
	{
		Vec2<T> result;
		result.x = lhs.x + (T)((rhs.x - lhs.x) * percentage);
		result.y = lhs.y + (T)((rhs.y - lhs.y) * percentage);
		return result;
	}

	/*********************************************************************************************/
	//Justine's
	template<typename T>
	Vec2<T> Normalise(Vec2<T> const& rhs)
	{
		return Vec2<T>(rhs).Normalise();
	}

	template<typename T>
	T Distance(Vec2<T> const& lhs, Vec2<T> const& rhs)
	{
		return (T)sqrt((double)((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y)));
	}

	template<typename T>
	T DotProduct(Vec2<T> const& lhs, Vec2<T> const& rhs)
	{
		return ((lhs.x * rhs.x) + (lhs.y * rhs.y));
	}
	/**********************************************************************************************/

	template<typename T1, typename T2>
	Vec2<T1> operator* (Vec2<T1> lhs, T2 rhs)
	{
		return Vec2<T1>(lhs.x * (T1)rhs, lhs.y * (T1)rhs);
	}

	template<typename T1, typename T2>
	Vec2<T1> operator*=(Vec2<T1> lhs, T2 rhs)
	{
		lhs.x *= (T1)rhs;
		lhs.y *= (T1)rhs;

		return lhs;
	}

	template<typename T1, typename T2>
	Vec2<T1> operator/ (Vec2<T1> lhs, T2 rhs)
	{
		return Vec2<T1>(lhs.x / (T1)rhs, lhs.y / (T1)rhs);
	}

	template<typename T1, typename T2>
	Vec2<T1> operator/=(Vec2<T1> lhs, T2 rhs)
	{
		lhs.x /= (T1)rhs;
		lhs.y /= (T1)rhs;

		return lhs;
	}

	/***************************************************************************************************
	*
	* Vec 3
	*
	*
	***************************************************************************************************/
	// Vec3 Constructors
	// Default constructor 
	template<typename T>
	Vec3<T>::Vec3() : x((T)0), y((T)0), z((T)0) {}

	// Parameterized Constructor
	template<typename T>
	Vec3<T>::Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

	// Copy constructor
	template<typename T>
	Vec3<T>::Vec3(const Vec3<T>& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

	// Copy assignment constructor
	template<typename T>
	Vec3<T>& Vec3<T>::operator=(const Vec3<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return *this;
	}

	/**************************************************************************************************/
	// Vec3 Basic operators + - []

	template<typename T>
	Vec3<T> Vec3<T>::operator+(Vec3<T> rhs) const
	{
		return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	template<typename T>
	Vec3<T> Vec3<T>::operator+(T rhs) const
	{
		return Vec3<T>(x + rhs, y + rhs, z + rhs);
	}

	template<typename T>
	Vec3<T> Vec3<T>::operator-(Vec3<T> rhs) const
	{
		return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	template<typename T>
	Vec3<T> Vec3<T>::operator-(T rhs) const
	{
		return Vec3<T>(x - rhs, y - rhs, z - rhs);
	}

	template<typename T>
	T& Vec3<T>::operator[](size_t index)
	{
		if (index == 0)
		{
			return this->x;
		}
		else if (index == 1)
		{
			return this->y;
		}
		return this->z;
	}

	template<typename T>
	T Vec3<T>::operator[](size_t index) const
	{
		if (index == 0)
		{
			return this->x;
		}
		else if (index == 1)
		{
			return this->y;
		}
		return this->z;
	}

	/**************************************************************************************************/
	// Vec3 Compound assignment operators -= += *= /=

	template<typename T>
	Vec3<T>& Vec3<T>::operator-=(const Vec3<T> rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;

		return *this;
	}

	template<typename T>
	Vec3<T>& Vec3<T>::operator+=(const Vec3<T> rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;

		return *this;
	}

	/**************************************************************************************************/
	// Vec3 Comparison operators < > == != <= >=

	template<typename T>
	bool Vec3<T>::operator==(const Vec3<T>& rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
	}

	template<typename T>
	bool Vec3<T>::operator!=(Vec3<T> const& rhs) const
	{
		return !(this == rhs);
	}

	template<typename T>
	bool Vec3<T>::operator< (const Vec3<T>& rhs) const
	{
		return this->LengthSquared() < rhs.LengthSquared(); //cheaper to use LengthSquared, dont waste time calculating sqrt
	}

	template<typename T>
	bool Vec3<T>::operator<=(Vec3<T> const& rhs) const
	{
		return this->LengthSquared() <= rhs.LengthSquared();
	}

	template<typename T>
	bool Vec3<T>::operator> (Vec3<T> const& rhs) const
	{
		return rhs <= this;
	}

	template<typename T>
	bool Vec3<T>::operator>=(Vec3<T> const& rhs) const
	{
		return rhs < this;
	}

	/**************************************************************************************************/
	// Vec3 Member functions

	template<typename T>
	Vec3<T> Vec3<T>::Zero()
	{
		return Vec3<T>((T)0, (T)0, (T)0);
	}

	template<typename T>
	Vec3<T> Vec3<T>::One()
	{
		return Vec3<T>((T)1, (T)1, (T)1);
	}
	/*********************************************************************************************/
	//Justine's
	template<typename T>
	Vec3<T>& Vec3<T>::Normalise() const
	{
		T length = Length();

		x /= length;
		y /= length;
		z /= length;

		return *this;
	}

	template<typename T>
	T Vec3<T>::Length() const
	{
		return (T)sqrt((double)LengthSquared());
	}

	template<typename T>
	T Vec3<T>::LengthSquared() const
	{
		return (x * x + y * y + z * z);
	}
	/*********************************************************************************************/
	template<typename T>
	Vec3<T>& Vec3<T>::Set(T _x, T _y, T _z)
	{
		x = _x;
		y = _y;
		z = _z;

		return *this;
	}

	template<typename T>
	Vec3<T>& Vec3<T>::Set(Vec3<T> const& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return *this;
	}

	/**************************************************************************************************/
	// Vec3 Non-member functions

	template<typename T>
	Vec3<T> Lerp(Vec3<T> const& lhs, Vec3<T> const& rhs, float percentage) //lhs - startpos, rhs - endpos, percentage 0-1
	{
		Vec3<T> result;
		result.x = lhs.x + (T)((rhs.x - lhs.x) * percentage);
		result.y = lhs.y + (T)((rhs.y - lhs.y) * percentage);
		result.z = lhs.z + (T)((rhs.z - lhs.z) * percentage);
		return result;
	}

	/*********************************************************************************************/
	//Justine's
	template<typename T>
	Vec3<T> Normalise(Vec3<T> const& rhs)
	{
		return Vec3<T>(rhs).Normalise();
	}

	template<typename T>
	T Distance(Vec3<T> const& lhs, Vec3<T> const& rhs)
	{
		return (T)sqrt((double)(((lhs.x - rhs.x) * (lhs.x - rhs.x)) + ((lhs.y - rhs.y) * (lhs.y - rhs.y)) + ((lhs.z - rhs.z) * (lhs.z - rhs.z))));
	}

	template<typename T>
	T DotProduct(Vec3<T> const& lhs, Vec3<T> const& rhs)
	{
		return ((lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z));
	}

	template<typename T>
	Vec3<T> CrossProduct(Vec3<T> const& lhs, Vec3<T> const& rhs)
	{
		Vec3<T> result;
		result.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
		result.y = (lhs.z * rhs.x) - (lhs.x * rhs.z);
		result.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);
		return result;

	}
	/*********************************************************************************************/

	template<typename T1, typename T2>
	Vec3<T1> operator* (Vec3<T1> lhs, T2 rhs)
	{
		return Vec3<T1>(lhs.x * (T1)rhs, lhs.y * (T1)rhs, lhs.z * (T1)rhs);
	}

	template<typename T1, typename T2>
	Vec3<T1> operator*=(Vec3<T1> lhs, T2 rhs)
	{
		lhs.x *= (T1)rhs;
		lhs.y *= (T1)rhs;
		lhs.z *= (T1)rhs;

		return lhs;
	}

	template<typename T1, typename T2>
	Vec3<T1> operator/ (Vec3<T1> lhs, T2 rhs)
	{
		return Vec3<T1>(lhs.x / (T1)rhs, lhs.y / (T1)rhs, lhs.z / (T1)rhs);
	}

	template<typename T1, typename T2>
	Vec3<T1> operator/=(Vec3<T1> lhs, T2 rhs)
	{
		lhs.x /= (T1)rhs;
		lhs.y /= (T1)rhs;
		lhs.z /= (T1)rhs;

		return lhs;
	}

	/***************************************************************************************************
	*
	* Vec 4
	*
	*
	***************************************************************************************************/
	// Vec4 Constructors
	// Default constructor 
	template<typename T>
	Vec4<T>::Vec4() : x((T)0), y((T)0), z((T)0), w((T)0) {}

	// Parameterized Constructor
	template<typename T>
	Vec4<T>::Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

	// Copy constructor
	template<typename T>
	Vec4<T>::Vec4(const Vec4<T>& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}

	// Copy assignment constructor
	template<typename T>
	Vec4<T>& Vec4<T>::operator=(const Vec4<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;

		return *this;
	}

	/**************************************************************************************************/
	// Vec4 Basic operators + - []

	template<typename T>
	Vec4<T> Vec4<T>::operator+(Vec4<T> rhs) const
	{
		return Vec4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	template<typename T>
	Vec4<T> Vec4<T>::operator+(T rhs) const
	{
		return Vec4<T>(x + rhs, y + rhs, z + rhs, w + rhs);
	}

	template<typename T>
	Vec4<T> Vec4<T>::operator-(Vec4<T> rhs) const
	{
		return Vec4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	template<typename T>
	Vec4<T> Vec4<T>::operator-(T rhs) const
	{
		return Vec4<T>(x - rhs, y - rhs, z - rhs, w - rhs);
	}

	template<typename T>
	T& Vec4<T>::operator[](size_t index)
	{
		if (index == 0)
		{
			return this->x;
		}
		else if (index == 1)
		{
			return this->y;
		}
		else if (index == 2)
		{
			return this->z;
		}
		return this->w;
	}

	template<typename T>
	T Vec4<T>::operator[](size_t index) const
	{
		if (index == 0)
		{
			return this->x;
		}
		else if (index == 1)
		{
			return this->y;
		}
		else if (index == 2)
		{
			return this->z;
		}
		return this->w;
	}

	/**************************************************************************************************/
	// Vec4 Compound assignment operators -= += *= /=

	template<typename T>
	Vec4<T>& Vec4<T>::operator-=(const Vec4<T> rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->w -= rhs.w;

		return *this;
	}

	template<typename T>
	Vec4<T>& Vec4<T>::operator+=(const Vec4<T> rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->w += rhs.w;

		return *this;
	}

	/**************************************************************************************************/
	// Vec4 Comparison operators < > == != <= >=

	template<typename T>
	bool Vec4<T>::operator==(const Vec4<T>& rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w);
	}

	template<typename T>
	bool Vec4<T>::operator!=(Vec4<T> const& rhs) const
	{
		return !(this == rhs);
	}

	template<typename T>
	bool Vec4<T>::operator< (const Vec4<T>& rhs) const
	{
		return this->LengthSquared() < rhs.LengthSquared(); //cheaper to use LengthSquared, dont waste time calculating sqrt
	}

	template<typename T>
	bool Vec4<T>::operator<=(Vec4<T> const& rhs) const
	{
		return this->LengthSquared() <= rhs.LengthSquared();
	}

	template<typename T>
	bool Vec4<T>::operator> (Vec4<T> const& rhs) const
	{
		return rhs <= this;
	}

	template<typename T>
	bool Vec4<T>::operator>=(Vec4<T> const& rhs) const
	{
		return rhs < this;
	}

	/**************************************************************************************************/
	// Vec4 Member functions

	template<typename T>
	Vec4<T> Vec4<T>::Zero()
	{
		return Vec4<T>((T)0, (T)0, (T)0, (T)0);
	}

	template<typename T>
	Vec4<T> Vec4<T>::One()
	{
		return Vec4<T>((T)1, (T)1, (T)1, (T)1);
	}

	/*********************************************************************************************/
	//Justine's
	template<typename T>
	Vec4<T>& Vec4<T>::Normalise() const
	{
		T length = Length();

		x /= length;
		y /= length;
		z /= length;
		w /= length;

		return *this;
	}

	template<typename T>
	T Vec4<T>::Length() const
	{
		return (T)sqrt((double)LengthSquared());
	}

	template<typename T>
	T Vec4<T>::LengthSquared() const
	{
		return (x * x + y * y + z * z + w * w);
	}
	/*********************************************************************************************/

	template<typename T>
	Vec4<T>& Vec4<T>::Set(T _x, T _y, T _z, T _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;

		return *this;
	}

	template<typename T>
	Vec4<T>& Vec4<T>::Set(Vec4<T> const& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;

		return *this;
	}

	/**************************************************************************************************/
	// Vec4 Non-member functions

	template<typename T>
	Vec4<T> Lerp(Vec4<T> const& lhs, Vec4<T> const& rhs, float percentage) //lhs - startpos, rhs - endpos, percentage 0-1
	{
		Vec4<T> result;
		result.x = lhs.x + (T)((rhs.x - lhs.x) * percentage);
		result.y = lhs.y + (T)((rhs.y - lhs.y) * percentage);
		result.z = lhs.z + (T)((rhs.z - lhs.z) * percentage);
		result.w = lhs.w + (T)((rhs.w - lhs.w) * percentage);
		return result;
	}

	/*********************************************************************************************/
	//Justine's
	template<typename T>
	Vec4<T> Normalise(Vec4<T> const& rhs)
	{
		return Vec4<T>(rhs).Normalise();
	}

	template<typename T>
	T Distance(Vec4<T> const& lhs, Vec4<T> const& rhs)
	{
		return (T)sqrt((double)(((lhs.x - rhs.x) * (lhs.x - rhs.x)) + ((lhs.y - rhs.y) * (lhs.y - rhs.y)) +
			((lhs.z - rhs.z) * (lhs.z - rhs.z)) + ((lhs.w - rhs.w) * (lhs.w - rhs.w))));
	}

	template<typename T>
	T DotProduct(Vec4<T> const& lhs, Vec4<T> const& rhs)
	{
		return ((lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w));
	}
	/*********************************************************************************************/

	template<typename T1, typename T2>
	Vec4<T1> operator* (Vec4<T1> lhs, T2 rhs)
	{
		return Vec4<T1>(lhs.x * (T1)rhs, lhs.y * (T1)rhs, lhs.z * (T1)rhs, lhs.w * (T1)rhs);
	}

	template<typename T1, typename T2>
	Vec4<T1> operator*=(Vec4<T1> lhs, T2 rhs)
	{
		lhs.x *= (T1)rhs;
		lhs.y *= (T1)rhs;
		lhs.z *= (T1)rhs;
		lhs.w *= (T1)rhs;

		return lhs;
	}

	template<typename T1, typename T2>
	Vec4<T1> operator/ (Vec4<T1> lhs, T2 rhs)
	{
		return Vec4<T1>(lhs.x / (T1)rhs, lhs.y / (T1)rhs, lhs.z / (T1)rhs, lhs.w / (T1)rhs);
	}

	template<typename T1, typename T2>
	Vec4<T1> operator/=(Vec4<T1> lhs, T2 rhs)
	{
		lhs.x /= (T1)rhs;
		lhs.y /= (T1)rhs;
		lhs.z /= (T1)rhs;
		lhs.w /= (T1)rhs;

		return lhs;
	}

	/***************************************************************************************************
	*
	* Vec casting
	*
	***************************************************************************************************/
	template<typename T>
	Vec2<T> ToVector2(Vec3<T> const& rhs) // Vec3 to Vec2
	{
		return Vec2<T>(rhs.x, rhs.y);
	}

	template<typename T>
	Vec2<T> ToVector2(Vec4<T> const& rhs) // Vec4 to Vec2
	{
		return Vec2<T>(rhs.x, rhs.y);
	}

	template<typename T>
	Vec3<T> ToVector3(Vec2<T> const& rhs) // Vec2 to Vec3
	{
		return Vec3<T>(rhs.x, rhs.y, (T)0);
	}

	template<typename T>
	Vec3<T> ToVector3(Vec4<T> const& rhs) // Vec4 to Vec3
	{
		return Vec3<T>(rhs.x, rhs.y, rhs.z);
	}

	template<typename T>
	Vec4<T> ToVector4(Vec2<T> const& rhs) // Vec2 to Vec4
	{
		return Vec4<T>(rhs.x, rhs.y, (T)0, (T)0);
	}

	template<typename T>
	Vec4<T> ToVector4(Vec3<T> const& rhs) // Vec3 to Vec4
	{
		return Vec4<T>(rhs.x, rhs.y, rhs.z, (T)0);
	}
}