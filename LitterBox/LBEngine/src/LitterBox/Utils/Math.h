/*!************************************************************************
 \file				Math.h
 \author(s)			Vanessa Chua Siew Jin,
					Carlo Villa Ilao Justine,
					CHIA Amadeus Jinhan
 \par DP email(s):	vanessasiewjin.chua@digipen.edu,
					justine.c@digipen.edu,
					amadeusjinhan.chia@digipen.edu
 \par Course:		CSD2401A
 \date				24-09-2023
 \brief
 The functions in the Math class include:

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include <cmath>		// For sin, cos, sqrt
#include <string>		//for custom to string functions

#pragma warning(push, 0)
#include "../../dependencies/RapidJSON/include/rapidjson.h" // For serialization
#include "../../dependencies/RapidJSON/include/document.h"
#pragma warning(pop)

namespace LB
{
	#define PI 3.14159265358979323846 // Used for rotation
	#define EPSILON_F 0.00001f
	#define EPSILON_D 0.00001

	/*!***********************************************************************
	 \brief
	 A Clamp function that limits the value to its min or max
	*************************************************************************/
	template <typename T>
	T Clamp(T value, T min, T max);

	/***************************************************************************************************
	*
	* Vec 2
	*	|x|
	*	|y|
	*
	***************************************************************************************************/

	/*!***********************************************************************
	 \brief
	 A Vec2 class that holds any type T
	*************************************************************************/
	template<typename T>
	class Vec2
	{
	public:
		/*!***********************************************************************
		 \brief
		  Data members of x and y
		*************************************************************************/
		T x;
		T y;

		/*!***********************************************************************
		 \brief
		  Default constructor set x and y values to 0
		*************************************************************************/
		Vec2();

		/*!***********************************************************************
		 \brief
		  Parameterized Constructor with x and y values
		*************************************************************************/
		Vec2(T x, T y);

		/*!***********************************************************************
		 \brief
		  Copy constructor to another Vec2
		*************************************************************************/
		Vec2(const Vec2<T>& rhs);

		/*!***********************************************************************
		 \brief
		  Copy assignment from another Vec2
		*************************************************************************/
		Vec2& operator=(const Vec2<T>& rhs); // Copy assignment

		// Basic operators + - []
		/*!***********************************************************************
		 \brief
		  + operator to add from a Vec2 to itself
		*************************************************************************/
		Vec2 operator+(Vec2<T> rhs)   const;

		/*!***********************************************************************
		 \brief
		  + operator to add from a variable to a Vec2
		*************************************************************************/
		Vec2 operator+(T rhs)		  const;

		/*!***********************************************************************
		 \brief
		  - operator to minus from a Vec2 to itself
		*************************************************************************/
		Vec2 operator-(Vec2<T> rhs)   const;

		/*!***********************************************************************
		 \brief
		  - operator to minus from a variable to a Vec2
		*************************************************************************/
		Vec2 operator-(T rhs)		  const;

		/*!***********************************************************************
		 \brief
		  [] operator to access element, a copy
		*************************************************************************/
		T    operator[](size_t index) const;

		/*!***********************************************************************
		 \brief
		  [] operator to access element, can modify itself
		*************************************************************************/

		T& operator[](size_t index);		 // can edit the actual thing

		// Compound assignment operators -= +=
		/*!***********************************************************************
		 \brief
		  -= operator to subtract a Vec2 and modify itself
		*************************************************************************/
		Vec2& operator-=(const Vec2<T> rhs);

		/*!***********************************************************************
		 \brief
		  += operator to add a Vec2 and modify itself
		*************************************************************************/
		Vec2& operator+=(const Vec2<T> rhs);

		/*!***********************************************************************
		 \brief
		  - unary operator
		*************************************************************************/
		Vec2 operator-()   const;

		// Comparison operators < > == != <= >=
		/*!***********************************************************************
		 \brief
		  == operator to check if its the same
		*************************************************************************/
		bool operator==(Vec2<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  != operator to check if its not the same
		*************************************************************************/
		bool operator!=(Vec2<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  < operator to compare if its lesser than the other vector
		*************************************************************************/
		bool operator< (Vec2<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  <= operator to compare if its lesser or same than the other vector
		*************************************************************************/
		bool operator<=(Vec2<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  > operator to compare if its more than the other vector
		*************************************************************************/
		bool operator> (Vec2<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  >= operator to compare if its more or same than the other vector
		*************************************************************************/
		bool operator>=(Vec2<T> const& rhs) const;

		// Vec2 Member functions
		/*!***********************************************************************
		 \brief
		  A function that fills up x and y to 0 in the Vec2
		*************************************************************************/
		Vec2<T> Zero(); // fill up all to 0

		/*!***********************************************************************
		 \brief
		  A function that fills up x and y to 1 in the Vec2
		*************************************************************************/
		Vec2<T> One();  // fill up all to 1

		/*!***********************************************************************
		 \brief
		  A function that returns a stringn containing the vector data
		*************************************************************************/
		std::string ToString();

		/*!***********************************************************************
		 \brief
		  A function that Normalise Vec2, to find direction
		*************************************************************************/
		Vec2<T>& Normalise();

		/*!***********************************************************************
		 \brief
		  A function that finds the magnitude of a Vec2
		*************************************************************************/
		T		 Length();

		/*!***********************************************************************
		 \brief
		  A function that finds the squared magnitude of a Vec2, this is when it did
		  not sqrt.
		*************************************************************************/
		T		 LengthSquared();

		/*!***********************************************************************
		 \brief
		  A function that finds the distance between 2 vectors
		*************************************************************************/
		static T Distance(Vec2<T> const& lhs, Vec2<T> const& rhs);

		/*!***********************************************************************
		 \brief
		  A function that sets x and y values to itself
		*************************************************************************/
		Vec2<T>& Set(T x, T y);

		/*!***********************************************************************
		 \brief
		  A function that sets x and y values from another vector to itself
		*************************************************************************/
		Vec2<T>& Set(Vec2<T> const& rhs);

		//Serializing & Deserializing
		/*!***********************************************************************
		 \brief
		  A function that serializes vector into json data
		*************************************************************************/
		bool Serialize(rapidjson::Value&, rapidjson::Document::AllocatorType&);

		/*!***********************************************************************
		 \brief
		  A function that deserializes json data into a vector
		*************************************************************************/
		bool Deserialize(const rapidjson::Value&);

	};

	// Vec2 Non-member operator overloads * / *= /=
	/*!***********************************************************************
	\brief
	 * operator to multiply lhs Vec2 with rhs Vec2
	*************************************************************************/
	template<typename T1, typename T2>
	Vec2<T1> operator*(Vec2<T1> lhs, T2 rhs);

	/*!***********************************************************************
	\brief
	 *= operator to multiply and assign lhs Vec2 after multiplying with rhs variable
	*************************************************************************/
	template<typename T1, typename T2>
	Vec2<T1>& operator*=(Vec2<T1>& lhs, T2 rhs);

	/*!***********************************************************************
	\brief
	 / operator to divide rhs variable to lhs Vec2
	*************************************************************************/
	template<typename T1, typename T2>
	Vec2<T1> operator/(Vec2<T1> lhs, T2 rhs);

	/*!***********************************************************************
	\brief
	 /= operator to divide and assign lhs Vec2 after division with rhs variable
	*************************************************************************/
	template<typename T1, typename T2>
	Vec2<T1>& operator/=(Vec2<T1>& lhs, T2 rhs);

	// Vec2 Non-member functions 
	/*!***********************************************************************
	\brief
	 Lerp Function that ease transition between 2 vectors * percentage
	*************************************************************************/
	template<typename T>
	Vec2<T> Lerp(Vec2<T> const& lhs, Vec2<T> const& rhs, float percentage);

	/*!***********************************************************************
	 \brief
	  A function that Normalise Vec2, to find direction
	*************************************************************************/
	template<typename T>
	Vec2<T> Normalise(Vec2<T> const& rhs);

	/*!***********************************************************************
	 \brief
	  A function that finds the distance from one Vec2 to another Vec2
	*************************************************************************/
	template<typename T>
	T Distance(Vec2<T> const& lhs, Vec2<T> const& rhs);

	/*!***********************************************************************
	 \brief
	  A function that do dot product to check for game object facing same direction
	*************************************************************************/
	template<typename T>
	T DotProduct(Vec2<T> const& lhs, Vec2<T> const& rhs);


	/***************************************************************************************************
	*
	* Vec 3
	* |x|
	* |y|
	* |z|
	*
	***************************************************************************************************/
	template <typename T>
	class Vec3
	{
	public:
		/*!***********************************************************************
		 \brief
		  Data members of x, y and z
		*************************************************************************/
		T x;
		T y;
		T z;

		/*!***********************************************************************
		 \brief
		  Default constructor set x, y and z values to 0
		*************************************************************************/
		Vec3();								 // Default constructor

		/*!***********************************************************************
		 \brief
		  Parameterized Constructor with x, y and z values
		*************************************************************************/
		Vec3(T x, T y, T z);				 // Parameterized Constructor

		/*!***********************************************************************
		 \brief
		Copy constructor to another Vec3
		*************************************************************************/
		Vec3(const Vec3<T>& rhs);			 // Copy constructor

		/*!***********************************************************************
		\brief
		Copy assignment from another Vec3
		*************************************************************************/
		Vec3& operator=(const Vec3<T>& rhs); // Copy assignment

		// Basic operators + - []
		/*!***********************************************************************
		 \brief
		  + operator to add from a Vec3 to itself
		*************************************************************************/
		Vec3 operator+(Vec3<T> rhs)   const;

		/*!***********************************************************************
		 \brief
		  + operator to add from a variable to a Vec3
		*************************************************************************/
		Vec3 operator+(T rhs)		  const;

		/*!***********************************************************************
		 \brief
		  - operator to minus from a Vec3 to itself
		*************************************************************************/
		Vec3 operator-(Vec3<T> rhs)   const;

		/*!***********************************************************************
		 \brief
		  - operator to minus from a variable to a Vec3
		*************************************************************************/
		Vec3 operator-(T rhs)		  const;

		/*!***********************************************************************
		 \brief
		  [] operator to access element, a copy
		*************************************************************************/
		T    operator[](size_t index) const;

		/*!***********************************************************************
		 \brief
		  [] operator to access element, can modify itself
		*************************************************************************/
		T& operator[](size_t index);		 // can edit the actual thing

		// Compound assignment operators -= +=
		/*!***********************************************************************
		 \brief
		  -= operator to subtract a Vec3 and modify itself
		*************************************************************************/
		Vec3& operator-=(const Vec3<T> rhs);

		/*!***********************************************************************
		 \brief
		  += operator to add a Vec3 and modify itself
		*************************************************************************/
		Vec3& operator+=(const Vec3<T> rhs);

		/*!***********************************************************************
		 \brief
		  - unary operator
		*************************************************************************/
		Vec3 operator-()   const;


		// Comparison operators < > == != <= >=
		/*!***********************************************************************
		 \brief
		  == operator to check if its the same
		*************************************************************************/
		bool operator==(Vec3<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  != operator to check if its not the same
		*************************************************************************/
		bool operator!=(Vec3<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  < operator to compare if its lesser than the other vector
		*************************************************************************/
		bool operator< (Vec3<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  <= operator to compare if its lesser or same than the other vector
		*************************************************************************/
		bool operator<=(Vec3<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  > operator to compare if its more than the other vector
		*************************************************************************/
		bool operator> (Vec3<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  >= operator to compare if its more or same than the other matrix
		*************************************************************************/
		bool operator>=(Vec3<T> const& rhs) const;

		// Vec3 Member functions
		/*!***********************************************************************
		 \brief
		  A function that fills up x and y to 0 in the Vec3
		*************************************************************************/
		Vec3<T> Zero(); // fill up all to 0

		/*!***********************************************************************
		\brief
		A function that fills up x and y to 1 in the Vec3
		*************************************************************************/
		Vec3<T> One();  // fill up all to 1

		/*!***********************************************************************
		 \brief
		  A function that returns a stringn containing a vector
		*************************************************************************/
		std::string ToString();

		/*!***********************************************************************
		 \brief
		  A function that Normalise Vec3, to find direction
		*************************************************************************/
		Vec3<T>& Normalise();

		/*!***********************************************************************
		 \brief
		  A function that finds the magnitude of a Vec3
		*************************************************************************/
		T		 Length();

		/*!***********************************************************************
		 \brief
		  A function that finds the squared magnitude of a Vec3, this is when it did
		  not sqrt.
		*************************************************************************/
		T		 LengthSquared();

		/*!***********************************************************************
		 \brief
		  A function that sets x and y values to itself
		*************************************************************************/
		Vec3<T>& Set(T x, T y, T z);

		/*!***********************************************************************
		 \brief
		  A function that sets x and y values from another vector to itself
		*************************************************************************/
		Vec3<T>& Set(Vec3<T> const& rhs);

		//Serializing & Deserializing
		/*!***********************************************************************
		 \brief
		  A function that serializes the vector into json data
		*************************************************************************/
		bool Serialize(rapidjson::Value&, rapidjson::Document::AllocatorType&);

		/*!***********************************************************************
		 \brief
		  A function that deserializes the json data into the vector
		*************************************************************************/
		bool Deserialize(const rapidjson::Value&);
	};

	// Vec3 Non-member operator overloads * / *= /=
	/*!***********************************************************************
	\brief
	 * operator to multiply lhs Vec3 with rhs Vec3
	*************************************************************************/
	template<typename T1, typename T2>
	Vec3<T1> operator*(Vec3<T1> lhs, T2 rhs);

	/*!***********************************************************************
	\brief
	 *= operator to multiply and assign lhs Vec3 after multiply with rhs variable
	*************************************************************************/
	template<typename T1, typename T2>
	Vec3<T1>& operator*=(Vec3<T1>& lhs, T2 rhs);

	/*!***********************************************************************
	\brief
	 / operator to divide rhs variable to lhs Vec3
	*************************************************************************/
	template<typename T1, typename T2>
	Vec3<T1> operator/(Vec3<T1> lhs, T2 rhs);

	/*!***********************************************************************
	\brief
	 /= operator to divide and assign lhs Vec3 after division with rhs variable
	*************************************************************************/
	template<typename T1, typename T2>
	Vec3<T1>& operator/=(Vec3<T1>& lhs, T2 rhs);

	// Vec3 Non-member functions 
	/*!***********************************************************************
	\brief
	 Lerp Function that ease transition between 2 vectors * percentage
	*************************************************************************/
	template<typename T>
	Vec3<T> Lerp(Vec3<T> const& lhs, Vec3<T> const& rhs, float percentage);

	/*!***********************************************************************
	 \brief
	  A function that Normalise Vec3, to find direction
	*************************************************************************/
	template<typename T>
	Vec3<T> Normalise(Vec3<T> const& rhs);

	/*!***********************************************************************
	 \brief
	  A function that finds the distance from one Vec3 to another Vec3
	*************************************************************************/
	template<typename T>
	T Distance(Vec3<T> const& lhs, Vec3<T> const& rhs);

	/*!***********************************************************************
	 \brief
	  A function that do dot product to check for game object facing same direction
	*************************************************************************/
	template<typename T>
	T DotProduct(Vec3<T> const& lhs, Vec3<T> const& rhs);

	/*!***********************************************************************
	 \brief
	  A function that calculates the cross product of 2 Vec3
	*************************************************************************/
	template<typename T>
	Vec3<T> CrossProduct(Vec3<T> const& lhs, Vec3<T> const& rhs);
	/***********************************************************************/


	/***************************************************************************************************
	*
	* Vec 4
	* |x|
	* |y|
	* |z|
	* |w|
	*
	***************************************************************************************************/
	template <typename T>
	class Vec4
	{
	public:
		/*!***********************************************************************
		 \brief
		  Data members of x, y, z and w
		*************************************************************************/
		T x;
		T y;
		T z;
		T w;

		/*!***********************************************************************
		 \brief
		  Default constructor set x, y, z and w values to 0
		*************************************************************************/
		Vec4();								 // Default constructor

		/*!***********************************************************************
		 \brief
		  Parameterized Constructor with x, y, z and w values
		*************************************************************************/
		Vec4(T x, T y, T z, T w);				 // Parameterized Constructor

		/*!***********************************************************************
		 \brief
		  Copy constructor to another Vec4
		*************************************************************************/
		Vec4(const Vec4<T>& rhs);			 // Copy constructor

		/*!***********************************************************************
		 \brief
		  Copy assignment from another Vec4
		*************************************************************************/
		Vec4& operator=(const Vec4<T>& rhs); // Copy assignment

		// Basic operators + - []
		/*!***********************************************************************
		 \brief
		  + operator to add from a Vec4 to itself
		*************************************************************************/
		Vec4 operator+(Vec4<T> rhs)   const;

		/*!***********************************************************************
		 \brief
		  + operator to add from a variable to a Vec4
		*************************************************************************/
		Vec4 operator+(T rhs)		  const;

		/*!***********************************************************************
		 \brief
		  - operator to minus from a Vec4 to itself
		*************************************************************************/
		Vec4 operator-(Vec4<T> rhs)   const;

		/*!***********************************************************************
		 \brief
		  - operator to minus from a variable to a Vec4
		*************************************************************************/
		Vec4 operator-(T rhs)		  const;

		/*!***********************************************************************
		 \brief
		  [] operator to access element, a copy
		*************************************************************************/
		T    operator[](size_t index) const;

		/*!***********************************************************************
		 \brief
		  [] operator to access element, can modify itself
		*************************************************************************/
		T& operator[](size_t index);		 // can edit the actual thing

		// Compound assignment operators -= +=
		/*!***********************************************************************
		 \brief
		  -= operator to subtract a Vec4 and modify itself
		*************************************************************************/
		Vec4& operator-=(const Vec4<T> rhs);

		/*!***********************************************************************
		 \brief
		  += operator to add a Vec4 and modify itself
		*************************************************************************/
		Vec4& operator+=(const Vec4<T> rhs);

		/*!***********************************************************************
		 \brief
		  - unary operator
		*************************************************************************/
		Vec4 operator-()   const;

		// Comparison operators < > == != <= >=
		/*!***********************************************************************
		 \brief
		  == operator to check if its the same
		*************************************************************************/
		bool operator==(Vec4<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  != operator to check if its not the same
		*************************************************************************/
		bool operator!=(Vec4<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  < operator to compare if its lesser than the other vector
		*************************************************************************/
		bool operator< (Vec4<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  <= operator to compare if its lesser or same than the other vector
		*************************************************************************/
		bool operator<=(Vec4<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  > operator to compare if its more than the other vector
		*************************************************************************/
		bool operator> (Vec4<T> const& rhs) const;

		/*!***********************************************************************
		 \brief
		  >= operator to compare if its more or same than the other matrix
		*************************************************************************/
		bool operator>=(Vec4<T> const& rhs) const;

		// Vec4 Member functions
		/*!***********************************************************************
		 \brief
		  A function that fills up x, y, z and w to 0 in the Vec4
		*************************************************************************/
		Vec4<T> Zero(); // fill up all to 0

		/*!***********************************************************************
		 \brief
		  A function that fills up x, y, z and w to 1 in the Vec4
		*************************************************************************/
		Vec4<T> One();  // fill up all to 1

		/*!***********************************************************************
		 \brief
		 A function that returns a string containing the vector data
		*************************************************************************/
		std::string ToString();

		/*!***********************************************************************
		 \brief
		  A function that Normalise Vec4, to find direction
		*************************************************************************/
		Vec4<T>& Normalise();

		/*!***********************************************************************
		 \brief
		  A function that finds the magnitude of a Vec4
		*************************************************************************/
		T		 Length();

		/*!***********************************************************************
		 \brief
		  A function that finds the squared magnitude of a Vec4, this is when it did
		  not sqrt.
		*************************************************************************/
		T		 LengthSquared();

		/*!***********************************************************************
		 \brief
		  A function that sets x, y, z and w values to itself
		*************************************************************************/
		Vec4<T>& Set(T x, T y, T z, T w);

		/*!***********************************************************************
		 \brief
		  A function that sets x, y, z and w values from another vector to itself
		*************************************************************************/
		Vec4<T>& Set(Vec4<T> const& rhs);


		//Serializing & Deserializing
		/*!***********************************************************************
		 \brief
		  A function that serializes the vector to json data
		*************************************************************************/
		bool Serialize(rapidjson::Value&, rapidjson::Document::AllocatorType&);

		/*!***********************************************************************
		 \brief
		  A function that deserializes json data to the vector
		*************************************************************************/
		bool Deserialize(const rapidjson::Value&);
	};

	// Vec4 Non-member operator overloads * / *= /=
	/*!***********************************************************************
	\brief
	 * operator to multiply lhs Vec4 with rhs Vec4
	*************************************************************************/
	template<typename T1, typename T2>
	Vec4<T1> operator*(Vec4<T1> lhs, T2 rhs);

	/*!***********************************************************************
	\brief
	 *= operator to multiply and assign lhs Vec4 after multiplying with rhs variable
	*************************************************************************/
	template<typename T1, typename T2>
	Vec4<T1>& operator*=(Vec4<T1>& lhs, T2 rhs);

	/*!***********************************************************************
	\brief
	 / operator to divide rhs variable to lhs Vec4
	*************************************************************************/
	template<typename T1, typename T2>
	Vec4<T1> operator/(Vec4<T1> lhs, T2 rhs);

	/*!***********************************************************************
	\brief
	 /= operator to divide and assign lhs Vec4 after division with rhs variable
	*************************************************************************/
	template<typename T1, typename T2>
	Vec4<T1>& operator/=(Vec4<T1>& lhs, T2 rhs);

	// Vec4 Non-member functions 
	/*!***********************************************************************
	\brief
	 Lerp Function that ease transition between 2 vectors * percentage
	*************************************************************************/
	template<typename T>
	Vec4<T> Lerp(Vec4<T> const& lhs, Vec4<T> const& rhs, float percentage);

	/*!***********************************************************************
	 \brief
	  A function that Normalise Vec4, to find direction
	*************************************************************************/
	template<typename T>
	Vec4<T> Normalise(Vec4<T> const& rhs);

	/*!***********************************************************************
	 \brief
	  A function that finds the distance from one Vec4 to another Vec4
	*************************************************************************/
	template<typename T>
	T Distance(Vec4<T> const& lhs, Vec4<T> const& rhs);

	/*!***********************************************************************
	 \brief
	  A function that do dot product to check for game object facing same direction
	*************************************************************************/
	template<typename T>
	T DotProduct(Vec4<T> const& lhs, Vec4<T> const& rhs);
	/***********************************************************************/

	/***************************************************************************************************
	*
	* Vec casting
	*
	***************************************************************************************************/

	/*!***********************************************************************
	 \brief
	  A function that convert Vec3 to Vec2
	*************************************************************************/
	template<typename T>
	Vec2<T> ToVector2(Vec3<T> const& rhs); // Vec3 to Vec2

	/*!***********************************************************************
	 \brief
	  A function that convert Vec4 to Vec2
	*************************************************************************/
	template<typename T>
	Vec2<T> ToVector2(Vec4<T> const& rhs); // Vec4 to Vec2

	/*!***********************************************************************
	 \brief
	  A function that convert Vec2 to Vec3
	*************************************************************************/
	template<typename T>
	Vec3<T> ToVector3(Vec2<T> const& rhs); // Vec2 to Vec3

	/*!***********************************************************************
	 \brief
	  A function that convert Vec4 to Vec3
	*************************************************************************/
	template<typename T>
	Vec3<T> ToVector3(Vec4<T> const& rhs); // Vec4 to Vec3

	/*!***********************************************************************
	 \brief
	  A function that convert Vec2 to Vec4
	*************************************************************************/
	template<typename T>
	Vec4<T> ToVector4(Vec2<T> const& rhs); // Vec2 to Vec4

	/*!***********************************************************************
	 \brief
	  A function that convert Vec3 to Vec4
	*************************************************************************/
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
	/*!***********************************************************************
	 \brief
	 A Clamp function that limits the value to its min or max
	*************************************************************************/
	template <typename T>
	T Clamp(T value, T min, T max)
	{
		return (value < min) ? min : (value > max) ? max : value;
	}

	/***************************************************************************************************
	*
	* Vec 2
	* |x|
	* |y|
	*
	***************************************************************************************************/

	/***************************************************************************************************/
	// Vec2 Constructors

	/*!***********************************************************************
	\brief
	 Default constructed has been constructed to set to 0
	*************************************************************************/
	template<typename T>
	Vec2<T>::Vec2() : x((T)0), y((T)0) {}


	/*!***********************************************************************
	 \brief
	  Parameterized Constructor with x and y values, the values x and y has been
	  set to whatever x and y has been set
	*************************************************************************/
	template<typename T>
	Vec2<T>::Vec2(T x, T y) : x(x), y(y) {}

	/*!***********************************************************************
	 \brief
	  Copy constructor to another Vec2, copy from one vector to itself
	*************************************************************************/
	template<typename T>
	Vec2<T>::Vec2(const Vec2<T>& rhs) : x(rhs.x), y(rhs.y) {}

	/*!***********************************************************************
	 \brief
	  Copy assignment from another Vec2 of x and y and return itself
	 \return
	 Vec2<T>&
	*************************************************************************/
	template<typename T>
	Vec2<T>& Vec2<T>::operator=(const Vec2<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	/**************************************************************************************************/
	// Vec2 Basic operators + - []

	/*!***********************************************************************
	\brief
	 + operator to add from another Vec2 x and y to itself x and y
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> Vec2<T>::operator+(Vec2<T> rhs) const
	{
		return Vec2<T>(x + rhs.x, y + rhs.y);
	}

	/*!***********************************************************************
	\brief
	 + operator to add from a variable to a Vec2 to itself x and y
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> Vec2<T>::operator+(T rhs) const
	{
		return Vec2<T>(x + rhs, y + rhs);
	}

	/*!***********************************************************************
	\brief
	 - operator to minus from a Vec2 of x and y to itself x and y.
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> Vec2<T>::operator-(Vec2<T> rhs) const
	{
		return Vec2<T>(x - rhs.x, y - rhs.y);
	}

	/*!***********************************************************************
	\brief
	 - operator to minus from a variable to a Vec2 x and y
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> Vec2<T>::operator-(T rhs) const
	{
		return Vec2<T>(x - rhs, y - rhs);
	}

	/*!***********************************************************************
	\brief
	 - unary operator, for negating the vector values
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> Vec2<T>::operator-() const
	{
		return Vec2<T>(-x, -y);
	}

	/*!***********************************************************************
	\brief
	 - [] operator to access element itself
	\return
	 T&
	*************************************************************************/
	template<typename T>
	T& Vec2<T>::operator[](size_t index)
	{
		if (index == 0)
		{
			return this->x;
		}
		return this->y;
	}

	/*!***********************************************************************
	\brief
	 [] operator to access element, a copy
	\return
	 T
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 -= operator to subtract a Vec2 and modify itself
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T>& Vec2<T>::operator-=(const Vec2<T> rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;

		return *this;
	}

	/*!***********************************************************************
	\brief
	 += operator to add a Vec2 and modify itself
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T>& Vec2<T>::operator+=(const Vec2<T> rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;

		return *this;
	}

	/**************************************************************************************************/
	// Vec2 Comparison operators < > == != <= >=

	/*!***********************************************************************
	\brief
	 == operator to check if its the same
	\return
	 bool (returns true when same, else false if different)
	*************************************************************************/
	template<typename T>
	bool Vec2<T>::operator==(const Vec2<T>& rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y);
	}

	/*!***********************************************************************
	\brief
	 != operator to check if its not the same
	\return
	 bool (returns true if different, else false if same)
	*************************************************************************/
	template<typename T>
	bool Vec2<T>::operator!=(Vec2<T> const& rhs) const
	{
		return !(this == rhs);
	}

	/*!***********************************************************************
	\brief
	 < operator to compare if its lesser than the other vector
	\return
	 bool (returns true if LengthSquared is less than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec2<T>::operator< (const Vec2<T>& rhs) const
	{
		return this->LengthSquared() < rhs.LengthSquared(); //cheaper to use LengthSquared, dont waste time calculating sqrt
	}

	/*!***********************************************************************
	\brief
	 <= operator to compare if its lesser than the other vector
	\return
	 bool (returns true if LengthSquared is less and equal than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec2<T>::operator<=(Vec2<T> const& rhs) const
	{
		return this->LengthSquared() <= rhs.LengthSquared();
	}

	/*!***********************************************************************
	\brief
	 > operator to compare if its more than the other vector
	\return
	 bool (returns true if LengthSquared is more than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec2<T>::operator> (Vec2<T> const& rhs) const
	{
		return rhs.LengthSquared() <= this->LengthSquared();
	}

	/*!***********************************************************************
	\brief
	 >= operator to compare if its more and equal than the other vector
	\return
	 bool (returns true if LengthSquared is more and equal than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec2<T>::operator>=(Vec2<T> const& rhs) const
	{
		return rhs.LengthSquared() < this->LengthSquared();
	}

	/**************************************************************************************************/
	// Vec2 Member functions

	/*!***********************************************************************
	\brief
	 This function sets all elements in the vector to 0
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> Vec2<T>::Zero()
	{
		return Vec2<T>((T)0, (T)0);
	}

	/*!***********************************************************************
	\brief
	 This function sets all elements in the vector to 1
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> Vec2<T>::One()
	{
		return Vec2<T>((T)1, (T)1);
	}

	/*!***********************************************************************
	\brief
	 This function returns a string containing the vector
	\return
	 string containing the vector
	*************************************************************************/
	template<typename T>
	std::string Vec2<T>::ToString()
	{
		return '(' + std::to_string(x) + "," + std::to_string(y) + ')';
	}

	/*!***********************************************************************
	\brief
	 This function normalise the vector, usually to maintain its length to 1
	\return
	 Vec2<T>&
	*************************************************************************/
	//Vec2<T>& Normalise()
	template<typename T>
	Vec2<T>& Vec2<T>::Normalise()
	{
		T length = Length();

		x /= length;
		y /= length;

		return *this;
	}

	/*!***********************************************************************
	\brief
	 This function calculates the length of the vector
	\return
	 T
	*************************************************************************/
	template<typename T>
	T Vec2<T>::Length()
	{
		return (T)sqrt((double)LengthSquared());
	}

	/*!***********************************************************************
	\brief
	 This function calculates the squared length of the vector
	\return
	 T
	*************************************************************************/
	template<typename T>
	T Vec2<T>::LengthSquared()
	{
		return (x * x + y * y);
	}

	/*!***********************************************************************
	\brief
	 This function calculates the distance between 2 points MEMBER FUNCTION
	\return
	 T
	*************************************************************************/
	template<typename T>
	T Vec2<T>::Distance(Vec2<T> const& lhs, Vec2<T> const& rhs)
	{
		return (T)sqrt((double)((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y)));
	}

	/*!***********************************************************************
	\brief
	 This function set the x and y into the vector
	\return
	 Vec2<T>&
	*************************************************************************/
	template<typename T>
	Vec2<T>& Vec2<T>::Set(T _x, T _y)
	{
		x = _x;
		y = _y;

		return *this;
	}

	/*!***********************************************************************
	\brief
	 This function set the rhs's x and y into the vector
	\return
	 Vec2<T>&
	*************************************************************************/
	template<typename T>
	Vec2<T>& Vec2<T>::Set(Vec2<T> const& rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	/**************************************************************************************************/
	// Vec2 Non-member functions

	/*!***********************************************************************
	\brief
	 This function is for lerping, limitation of the vector
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> Lerp(Vec2<T> const& lhs, Vec2<T> const& rhs, float percentage) //lhs - startpos, rhs - endpos, percentage 0-1
	{
		Vec2<T> result;
		result.x = lhs.x + (T)((rhs.x - lhs.x) * percentage);
		result.y = lhs.y + (T)((rhs.y - lhs.y) * percentage);
		return result;
	}

	/*!***********************************************************************
	\brief
	 This function normalise the vector, usually to maintain its length to 1
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> Normalise(Vec2<T> const& rhs)
	{
		return Vec2<T>(rhs).Normalise();
	}

	/*!***********************************************************************
	\brief
	 This function calculates the distance between 2 points
	\return
	 T
	*************************************************************************/
	template<typename T>
	T Distance(Vec2<T> const& lhs, Vec2<T> const& rhs)
	{
		return (T)sqrt((double)(((lhs.x - rhs.x) * (lhs.x - rhs.x)) + ((lhs.y - rhs.y) * (lhs.y - rhs.y))));
	}

	/*!***********************************************************************
	\brief
	 This function calculates the dot product of Vec2
	\return
	 T
	*************************************************************************/
	template<typename T>
	T DotProduct(Vec2<T> const& lhs, Vec2<T> const& rhs)
	{
		return ((lhs.x * rhs.x) + (lhs.y * rhs.y));
	}

	/*!***********************************************************************
	\brief
	 * operator, multiply a vector and a variable
	\return
	 Vec2<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec2<T1> operator* (Vec2<T1> lhs, T2 rhs)
	{
		return Vec2<T1>(lhs.x * (T1)rhs, lhs.y * (T1)rhs);
	}

	/*!***********************************************************************
	\brief
	 *= operator, multiply and assign back with a vector and a variable
	\return
	 Vec2<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec2<T1>& operator*=(Vec2<T1>& lhs, T2 rhs)
	{
		lhs.x *= (T1)rhs;
		lhs.y *= (T1)rhs;

		return lhs;
	}

	/*!***********************************************************************
	\brief
	 / operator, division from a vector and a variable
	\return
	 Vec2<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec2<T1> operator/ (Vec2<T1> lhs, T2 rhs)
	{
		return Vec2<T1>(lhs.x / (T1)rhs, lhs.y / (T1)rhs);
	}

	/*!***********************************************************************
	\brief
	 /= operator, division and assign back with a vector and a variable
	\return
	 Vec2<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec2<T1>& operator/=(Vec2<T1>& lhs, T2 rhs)
	{
		lhs.x /= (T1)rhs;
		lhs.y /= (T1)rhs;

		return lhs;
	}

	/*!***********************************************************************
	\brief
	This function serializes the vector into json data
	\return
	returns false if serialization  has failed, true if it has succeeded
	*************************************************************************/
	template<typename T>
	bool Vec2<T>::Serialize(rapidjson::Value& data, rapidjson::Document::AllocatorType& alloc)
	{
		data.SetObject();
		data.AddMember("x", x, alloc);
		data.AddMember("y", y, alloc);
		return true;
	}

	/*!***********************************************************************
	\brief
	 This function deserializes the json data into the vector
	\return
	returns false if deserialization has failed, true if it has succeeded
	*************************************************************************/
	template<typename T>
	bool Vec2<T>::Deserialize(const rapidjson::Value& data)
	{
		bool HasX = data.HasMember("x");
		bool HasY = data.HasMember("y");
		if (data.IsObject())
		{
			if (HasX && HasY)
			{
				x = data["x"].Get<T>();
				y = data["y"].Get<T>();
				return true;
			}
		}
		return false;
	}

	/***************************************************************************************************
	*
	* Vec 3
	* |x|
	* |y|
	* |z|
	*
	***************************************************************************************************/
	// Vec3 Constructors

	/*!***********************************************************************
	\brief
	 Default constructed has been constructed to set to 0
	*************************************************************************/
	template<typename T>
	Vec3<T>::Vec3() : x((T)0), y((T)0), z((T)0) {}

	/*!***********************************************************************
	 \brief
	  Parameterized Constructor with x, y, z values, the values x, y, z has been
	  set to whatever x, y, z has been set
	*************************************************************************/
	template<typename T>
	Vec3<T>::Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

	/*!***********************************************************************
	 \brief
	  Copy constructor to another Vec3, copy from one vector to itself
	*************************************************************************/
	template<typename T>
	Vec3<T>::Vec3(const Vec3<T>& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

	/*!***********************************************************************
	 \brief
	  Copy assignment from another Vec3 of x and y and return itself
	 \return
	 Vec3<T>&
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 + operator to add from another Vec3 x, y, z to itself x, y, z
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> Vec3<T>::operator+(Vec3<T> rhs) const
	{
		return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	/*!***********************************************************************
	\brief
	 + operator to add from a variable to a Vec3 to itself x, y, z
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> Vec3<T>::operator+(T rhs) const
	{
		return Vec3<T>(x + rhs, y + rhs, z + rhs);
	}

	/*!***********************************************************************
	\brief
	 - operator to minus from a Vec3 of x, y, z to itself x, y, z.
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> Vec3<T>::operator-(Vec3<T> rhs) const
	{
		return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	/*!***********************************************************************
	\brief
	 - operator to minus from a variable to a Vec3 x, y, z
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> Vec3<T>::operator-(T rhs) const
	{
		return Vec3<T>(x - rhs, y - rhs, z - rhs);
	}

	/*!***********************************************************************
	\brief
	 - unary operator, for negating the vector values
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> Vec3<T>::operator-() const
	{
		return Vec3<T>(-x, -y, -z);
	}

	/*!***********************************************************************
	\brief
	 - [] operator to access element itself
	\return
	 T&
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 [] operator to access element, a copy
	\return
	 T
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 -= operator to subtract a Vec3 and modify itself
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T>& Vec3<T>::operator-=(const Vec3<T> rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;

		return *this;
	}

	/*!***********************************************************************
	\brief
	 += operator to add a Vec3 and modify itself
	\return
	 Vec3<T>
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 == operator to check if its the same
	\return
	 bool (returns true when same, else false if different)
	*************************************************************************/
	template<typename T>
	bool Vec3<T>::operator==(const Vec3<T>& rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
	}

	/*!***********************************************************************
	\brief
	 != operator to check if its not the same
	\return
	 bool (returns true if different, else false if same)
	*************************************************************************/
	template<typename T>
	bool Vec3<T>::operator!=(Vec3<T> const& rhs) const
	{
		return !(this == rhs);
	}

	/*!***********************************************************************
	\brief
	 < operator to compare if its lesser than the other vector
	\return
	 bool (returns true if LengthSquared is less than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec3<T>::operator< (const Vec3<T>& rhs) const
	{
		return this->LengthSquared() < rhs.LengthSquared(); //cheaper to use LengthSquared, dont waste time calculating sqrt
	}

	/*!***********************************************************************
	\brief
	 <= operator to compare if its lesser than the other vector
	\return
	 bool (returns true if LengthSquared is less and equal than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec3<T>::operator<=(Vec3<T> const& rhs) const
	{
		return this->LengthSquared() <= rhs.LengthSquared();
	}

	/*!***********************************************************************
	\brief
	 > operator to compare if its more than the other vector
	\return
	 bool (returns true if LengthSquared is more than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec3<T>::operator> (Vec3<T> const& rhs) const
	{
		return rhs.LengthSquared() <= this->LengthSquared();
	}

	/*!***********************************************************************
	\brief
	 >= operator to compare if its more and equal than the other vector
	\return
	 bool (returns true if LengthSquared is more and equal than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec3<T>::operator>=(Vec3<T> const& rhs) const
	{
		return rhs.LengthSquared() < this->LengthSquared();
	}

	/**************************************************************************************************/
	// Vec3 Member functions

	/*!***********************************************************************
	\brief
	 This function sets all elements in the vector to 0
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> Vec3<T>::Zero()
	{
		return Vec3<T>((T)0, (T)0, (T)0);
	}

	/*!***********************************************************************
	\brief
	 This function sets all elements in the vector to 1
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> Vec3<T>::One()
	{
		return Vec3<T>((T)1, (T)1, (T)1);
	}

	/*!***********************************************************************
	\brief

	\return

	*************************************************************************/
	template<typename T>
	std::string Vec3<T>::ToString()
	{
		return '(' + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ')';
	}

	/*!***********************************************************************
	\brief
	 This function normalise the vector, usually to maintain its length to 1
	\return
	 Vec3<T>&
	*************************************************************************/
	template<typename T>
	Vec3<T>& Vec3<T>::Normalise()
	{
		T length = Length();

		x /= length;
		y /= length;
		z /= length;

		return *this;
	}

	/*!***********************************************************************
	\brief
	 This function calculates the length of the vector
	\return
	 T
	*************************************************************************/
	template<typename T>
	T Vec3<T>::Length()
	{
		return (T)sqrt((double)LengthSquared());
	}

	/*!***********************************************************************
	\brief
	 This function calculates the squared length of the vector
	\return
	 T
	*************************************************************************/
	template<typename T>
	T Vec3<T>::LengthSquared()
	{
		return (x * x + y * y + z * z);
	}

	/*!***********************************************************************
	\brief
	 This function set the x,y,z into the vector
	\return
	Vec3<T>&
	*************************************************************************/
	template<typename T>
	Vec3<T>& Vec3<T>::Set(T _x, T _y, T _z)
	{
		x = _x;
		y = _y;
		z = _z;

		return *this;
	}

	/*!***********************************************************************
	\brief
	 This function set the rhs's x, y, z into the vector
	\return
	 Vec3<T>&
	*************************************************************************/
	template<typename T>
	Vec3<T>& Vec3<T>::Set(Vec3<T> const& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return *this;
	}

	/*!***********************************************************************
	\brief
	This function serializes the vector into json data
	\return
	returns false if serialization has failed, true if it has succeeded
	*************************************************************************/
	template<typename T>
	bool Vec3<T>::Serialize(rapidjson::Value& data, rapidjson::Document::AllocatorType& alloc)
	{
		data.SetObject();
		data.AddMember("x", x, alloc);
		data.AddMember("y", y, alloc);
		data.AddMember("z", z, alloc);
		return true;
	}

	/*!***********************************************************************
	\brief
	This function deserializes json data into the vector
	\return
	returns false if deserialization has failed, true if it has succeeded
	*************************************************************************/
	template<typename T>
	bool Vec3<T>::Deserialize(const rapidjson::Value& data)
	{
		bool HasX = data.HasMember("x");
		bool HasY = data.HasMember("y");
		bool HasZ = data.HasMember("z");
		if (data.IsObject())
		{
			if (HasX && HasY && HasZ)
			{
				x = data["x"].Get<T>();
				y = data["y"].Get<T>();
				z = data["z"].Get<T>();
				return true;
			}
		}
		return false;
	}

	/**************************************************************************************************/
	// Vec3 Non-member functions

	/*!***********************************************************************
	\brief
	 This function is for lerping, limitation of the vector
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> Lerp(Vec3<T> const& lhs, Vec3<T> const& rhs, float percentage) //lhs - startpos, rhs - endpos, percentage 0-1
	{
		Vec3<T> result;
		result.x = lhs.x + (T)((rhs.x - lhs.x) * percentage);
		result.y = lhs.y + (T)((rhs.y - lhs.y) * percentage);
		result.z = lhs.z + (T)((rhs.z - lhs.z) * percentage);
		return result;
	}

	/*!***********************************************************************
	\brief
	 This function normalise the vector, usually to maintain its length to 1
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> Normalise(Vec3<T> const& rhs)
	{
		return Vec3<T>(rhs).Normalise();
	}

	/*!***********************************************************************
	\brief
	 This function calculates the distance between 2 points
	\return
	 T
	*************************************************************************/
	template<typename T>
	T Distance(Vec3<T> const& lhs, Vec3<T> const& rhs)
	{
		return (T)sqrt((double)(((lhs.x - rhs.x) * (lhs.x - rhs.x)) + ((lhs.y - rhs.y) * (lhs.y - rhs.y)) + ((lhs.z - rhs.z) * (lhs.z - rhs.z))));
	}

	/*!***********************************************************************
	\brief
	 This function calculates the dot product of Vec3
	\return
	 T
	*************************************************************************/
	template<typename T>
	T DotProduct(Vec3<T> const& lhs, Vec3<T> const& rhs)
	{
		return ((lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z));
	}

	/*!***********************************************************************
	\brief
	 This function calculates the cross product of Vec3
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> CrossProduct(Vec3<T> const& lhs, Vec3<T> const& rhs)
	{
		Vec3<T> result;
		result.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
		result.y = (lhs.z * rhs.x) - (lhs.x * rhs.z);
		result.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);
		return result;

	}

	/*!***********************************************************************
	\brief
	 * operator, multiply a vector and a variable
	\return
	 Vec3<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec3<T1> operator* (Vec3<T1> lhs, T2 rhs)
	{
		return Vec3<T1>(lhs.x * (T1)rhs, lhs.y * (T1)rhs, lhs.z * (T1)rhs);
	}

	/*!***********************************************************************
	\brief
	 *= operator, multiply and assign back with a vector and a variable
	\return
	 Vec3<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec3<T1>& operator*=(Vec3<T1>& lhs, T2 rhs)
	{
		lhs.x *= (T1)(rhs);
		lhs.y *= (T1)(rhs);
		lhs.z *= (T1)(rhs);

		return lhs;
	}

	/*!***********************************************************************
	\brief
	 / operator, division from a vector and a variable
	\return
	 Vec3<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec3<T1> operator/ (Vec3<T1> lhs, T2 rhs)
	{
		return Vec3<T1>(lhs.x / (T1)rhs, lhs.y / (T1)rhs, lhs.z / (T1)rhs);
	}

	/*!***********************************************************************
	\brief
	 /= operator, division and assign back with a vector and a variable
	\return
	 Vec3<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec3<T1>& operator/=(Vec3<T1>& lhs, T2 rhs)
	{
		lhs.x /= (T1)rhs;
		lhs.y /= (T1)rhs;
		lhs.z /= (T1)rhs;

		return lhs;
	}

	/***************************************************************************************************
	*
	* Vec 4
	* |x|
	* |y|
	* |z|
	* |w|
	*
	***************************************************************************************************/
	// Vec4 Constructors

	/*!***********************************************************************
	\brief
	 Default constructed has been constructed to set to 0
	*************************************************************************/
	template<typename T>
	Vec4<T>::Vec4() : x((T)0), y((T)0), z((T)0), w((T)0) {}

	/*!***********************************************************************
	 \brief
	  Parameterized Constructor with x, y, z, w values, the values x, y, z, w has been
	  set to whatever x, y, z, w has been set
	*************************************************************************/
	template<typename T>
	Vec4<T>::Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

	/*!***********************************************************************
	 \brief
	  Copy constructor to another Vec4, copy from one vector to itself
	*************************************************************************/
	template<typename T>
	Vec4<T>::Vec4(const Vec4<T>& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}

	/*!***********************************************************************
	 \brief
	  Copy assignment from another Vec4 of x, y, z, w and return itself
	 \return
	 Vec4<T>&
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 + operator to add from another Vec4 x, y, z , w to itself x, y, z, w
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T> Vec4<T>::operator+(Vec4<T> rhs) const
	{
		return Vec4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	/*!***********************************************************************
	\brief
	 + operator to add from a variable to a Vec4 to itself x, y, z, w
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T> Vec4<T>::operator+(T rhs) const
	{
		return Vec4<T>(x + rhs, y + rhs, z + rhs, w + rhs);
	}

	/*!***********************************************************************
	\brief
	 - operator to minus from a Vec4 of x, y, z, w to itself x, y, z, w
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T> Vec4<T>::operator-(Vec4<T> rhs) const
	{
		return Vec4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	/*!***********************************************************************
	\brief
	 - operator to minus from a variable to a Vec4 x, y, z, w
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T> Vec4<T>::operator-(T rhs) const
	{
		return Vec4<T>(x - rhs, y - rhs, z - rhs, w - rhs);
	}

	/*!***********************************************************************
	\brief
	 - unary operator, for negating the vector values
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T> Vec4<T>::operator-() const
	{
		return Vec4<T>(-x, -y, -z, -w);
	}

	/*!***********************************************************************
	\brief
	 - [] operator to access element itself
	\return
	 T&
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 [] operator to access element, a copy
	\return
	 T
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 -= operator to subtract a Vec4 and modify itself
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T>& Vec4<T>::operator-=(const Vec4<T> rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->w -= rhs.w;

		return *this;
	}

	/*!***********************************************************************
	\brief
	 += operator to add a Vec4 and modify itself
	\return
	 Vec4<T>
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 == operator to check if its the same
	\return
	 bool (returns true when same, else false if different)
	*************************************************************************/
	template<typename T>
	bool Vec4<T>::operator==(const Vec4<T>& rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w);
	}

	/*!***********************************************************************
	\brief
	 != operator to check if its not the same
	\return
	 bool (returns true if different, else false if same)
	*************************************************************************/
	template<typename T>
	bool Vec4<T>::operator!=(Vec4<T> const& rhs) const
	{
		return !(this == rhs);
	}

	/*!***********************************************************************
	\brief
	 < operator to compare if its lesser than the other vector
	\return
	 bool (returns true if LengthSquared is less than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec4<T>::operator< (const Vec4<T>& rhs) const
	{
		return this->LengthSquared() < rhs.LengthSquared(); //cheaper to use LengthSquared, dont waste time calculating sqrt
	}

	/*!***********************************************************************
	\brief
	 <= operator to compare if its lesser than the other vector
	\return
	 bool (returns true if LengthSquared is less and equal than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec4<T>::operator<=(Vec4<T> const& rhs) const
	{
		return this->LengthSquared() <= rhs.LengthSquared();
	}

	/*!***********************************************************************
	\brief
	 > operator to compare if its more than the other vector
	\return
	 bool (returns true if LengthSquared is more than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec4<T>::operator> (Vec4<T> const& rhs) const
	{
		return rhs.LengthSquared() <= this->LengthSquared();
	}

	/*!***********************************************************************
	\brief
	 >= operator to compare if its more and equal than the other vector
	\return
	 bool (returns true if LengthSquared is more and equal than the other LengthSquared)
	*************************************************************************/
	template<typename T>
	bool Vec4<T>::operator>=(Vec4<T> const& rhs) const
	{
		return rhs.LengthSquared() < this->LengthSquared();
	}

	/**************************************************************************************************/
	// Vec4 Member functions

	/*!***********************************************************************
	\brief
	 This function sets all elements in the vector to 0
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T> Vec4<T>::Zero()
	{
		return Vec4<T>((T)0, (T)0, (T)0, (T)0);
	}

	/*!***********************************************************************
	\brief
	 This function sets all elements in the vector to 1
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T> Vec4<T>::One()
	{
		return Vec4<T>((T)1, (T)1, (T)1, (T)1);
	}

	/*!***********************************************************************
	\brief
	This function creates a string with the vector
	\return
	string containing the vector
	*************************************************************************/
	template<typename T>
	std::string Vec4<T>::ToString()
	{
		return '(' + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(w) + ')';
	}

	/*!***********************************************************************
	\brief
	 This function normalise the vector, usually to maintain its length to 1
	\return
	 Vec4<T>&
	*************************************************************************/
	template<typename T>
	Vec4<T>& Vec4<T>::Normalise()
	{
		T length = Length();

		x /= length;
		y /= length;
		z /= length;
		w /= length;

		return *this;
	}

	/*!***********************************************************************
	\brief
	 This function calculates the length of the vector
	\return
	 T
	*************************************************************************/
	template<typename T>
	T Vec4<T>::Length()
	{
		return (T)sqrt((double)LengthSquared());
	}

	/*!***********************************************************************
	\brief
	 This function calculates the squared length of the vector
	\return
	 T
	*************************************************************************/
	template<typename T>
	T Vec4<T>::LengthSquared()
	{
		return (x * x + y * y + z * z + w * w);
	}

	/*!***********************************************************************
	\brief
	 This function set the x,y,z,w into the vector
	\return
	Vec4<T>&
	*************************************************************************/
	template<typename T>
	Vec4<T>& Vec4<T>::Set(T _x, T _y, T _z, T _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;

		return *this;
	}

	/*!***********************************************************************
	\brief
	 This function set the rhs's x, y, z, w into the vector
	\return
	 Vec4<T>&
	*************************************************************************/
	template<typename T>
	Vec4<T>& Vec4<T>::Set(Vec4<T> const& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;

		return *this;
	}

	/*!***********************************************************************
	\brief
	This function serializes the vector into json data
	\return
	returns false if serialization has failed, true if it has succeeded
	*************************************************************************/
	template<typename T>
	bool Vec4<T>::Serialize(rapidjson::Value& data, rapidjson::Document::AllocatorType& alloc)
	{
		data.SetObject();
		data.AddMember("x", x, alloc);
		data.AddMember("y", y, alloc);
		data.AddMember("z", z, alloc);
		data.AddMember("w", w, alloc);
		return true;
	}

	/*!***********************************************************************
	\brief
	This function deserializes json data into the vector
	\return
	returns false if deserialization has failed, true if it has succeeded
	*************************************************************************/
	template<typename T>
	bool Vec4<T>::Deserialize(const rapidjson::Value& data)
	{
		bool HasX = data.HasMember("x");
		bool HasY = data.HasMember("y");
		bool HasZ = data.HasMember("z");
		bool HasW = data.HasMember("w");
		if (data.IsObject())
		{
			if (HasX && HasY && HasZ && HasW)
			{
				x = data["x"].Get<T>();
				y = data["y"].Get<T>();
				z = data["z"].Get<T>();
				w = data["w"].Get<T>();
				return true;
			}
		}
		return false;
	}

	/**************************************************************************************************/
	// Vec4 Non-member functions

	/*!***********************************************************************
	\brief
	 This function is for lerping, limitation of the vector
	\return
	 Vec4<T>
	*************************************************************************/
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

	/*!***********************************************************************
	\brief
	 This function normalise the vector, usually to maintain its length to 1
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T> Normalise(Vec4<T> const& rhs)
	{
		return Vec4<T>(rhs).Normalise();
	}

	/*!***********************************************************************
	\brief
	 This function calculates the distance between 2 points
	\return
	 T
	*************************************************************************/
	template<typename T>
	T Distance(Vec4<T> const& lhs, Vec4<T> const& rhs)
	{
		return (T)sqrt((double)(((lhs.x - rhs.x) * (lhs.x - rhs.x)) + ((lhs.y - rhs.y) * (lhs.y - rhs.y)) +
			((lhs.z - rhs.z) * (lhs.z - rhs.z)) + ((lhs.w - rhs.w) * (lhs.w - rhs.w))));
	}

	/*!***********************************************************************
	\brief
	 This function calculates the dot product of Vec4
	\return
	 T
	*************************************************************************/
	template<typename T>
	T DotProduct(Vec4<T> const& lhs, Vec4<T> const& rhs)
	{
		return ((lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w));
	}
	/*********************************************************************************************/

	/*!***********************************************************************
	\brief
	 * operator, multiply a vector and a variable
	\return
	 Vec4<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec4<T1> operator* (Vec4<T1> lhs, T2 rhs)
	{
		return Vec4<T1>(lhs.x * (T1)rhs, lhs.y * (T1)rhs, lhs.z * (T1)rhs, lhs.w * (T1)rhs);
	}

	/*!***********************************************************************
	\brief
	 *= operator, multiply and assign back with a vector and a variable
	\return
	 Vec4<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec4<T1>& operator*=(Vec4<T1>& lhs, T2 rhs)
	{
		lhs.x *= (T1)rhs;
		lhs.y *= (T1)rhs;
		lhs.z *= (T1)rhs;
		lhs.w *= (T1)rhs;

		return lhs;
	}

	/*!***********************************************************************
	\brief
	 / operator, division from a vector and a variable
	\return
	 Vec4<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec4<T1> operator/ (Vec4<T1> lhs, T2 rhs)
	{
		return Vec4<T1>(lhs.x / (T1)rhs, lhs.y / (T1)rhs, lhs.z / (T1)rhs, lhs.w / (T1)rhs);
	}

	/*!***********************************************************************
	\brief
	 /= operator, division and assign back with a vector and a variable
	\return
	 Vec4<T1>
	*************************************************************************/
	template<typename T1, typename T2>
	Vec4<T1>& operator/=(Vec4<T1>& lhs, T2 rhs)
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

	/*!***********************************************************************
	\brief
	 A function to convert from Vec3 to Vec2
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> ToVector2(Vec3<T> const& rhs) // Vec3 to Vec2
	{
		return Vec2<T>(rhs.x, rhs.y);
	}

	/*!***********************************************************************
	\brief
	 A function to convert from Vec4 to Vec2
	\return
	 Vec2<T>
	*************************************************************************/
	template<typename T>
	Vec2<T> ToVector2(Vec4<T> const& rhs) // Vec4 to Vec2
	{
		return Vec2<T>(rhs.x, rhs.y);
	}

	/*!***********************************************************************
	\brief
	 A function to convert from Vec2 to Vec3
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> ToVector3(Vec2<T> const& rhs) // Vec2 to Vec3
	{
		return Vec3<T>(rhs.x, rhs.y, (T)0);
	}

	/*!***********************************************************************
	\brief
	 A function to convert from Vec4 to Vec3
	\return
	 Vec3<T>
	*************************************************************************/
	template<typename T>
	Vec3<T> ToVector3(Vec4<T> const& rhs) // Vec4 to Vec3
	{
		return Vec3<T>(rhs.x, rhs.y, rhs.z);
	}

	/*!***********************************************************************
	\brief
	 A function to convert from Vec2 to Vec4
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T> ToVector4(Vec2<T> const& rhs) // Vec2 to Vec4
	{
		return Vec4<T>(rhs.x, rhs.y, (T)0, (T)0);
	}

	/*!***********************************************************************
	\brief
	 A function to convert from Vec3 to Vec4
	\return
	 Vec4<T>
	*************************************************************************/
	template<typename T>
	Vec4<T> ToVector4(Vec3<T> const& rhs) // Vec3 to Vec4
	{
		return Vec4<T>(rhs.x, rhs.y, rhs.z, (T)0);
	}

	/*!***********************************************************************
	 \brief
	 Non-Member to convert Degree to Radian

	 \return
	 T
	*************************************************************************/
	template<typename T>
	T DegToRad(T Degangle)
	{
		return ((T)(PI / 180.0) * Degangle);
	}

	/*!***********************************************************************
	 \brief
	 Non-Member to convert Radian to Degree

	 \return
	 T
	*************************************************************************/
	template<typename T>
	T RadToDeg(T Radangle)
	{
		return ((T)(180.0 / PI) * Radangle);
	}

	/*!***********************************************************************
	 \brief
	 Non-Member to Absolute Value

	 \return
	 T
	*************************************************************************/
	template<typename T>
	T AbsValue(T value)
	{
		return value < 0 ? (-value) : value;
	}
}