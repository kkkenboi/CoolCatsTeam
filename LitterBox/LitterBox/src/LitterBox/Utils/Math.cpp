#include "./LitterBox/Utils/Math.h"

namespace LB
{
	/***************************************************************************************************
	* 
	* Vec 2
	* - takes in a template class
	* 
	***************************************************************************************************/
	//Default constructor 
	template<class T>
	Vec2<T>::Vec2() : x(0), y(0) {}

	//Parameterized Constructor
	template<class T>
	Vec2<T>::Vec2(T x, T y) : x(x), y(y) {}

	//Copy constructor
	template<class T>
	Vec2<T>::Vec2(const Vec2<T>& rhs) : x(rhs.x), y(rhs.y) {}

	//Copy assignment constructor
	template<class T>
	Vec2<T>& Vec2<T>::operator=(const Vec2<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	template<class T>
	Vec2<T> Vec2<T>::operator+(Vec2<T> rhs) const
	{
		return Vec2<T>(x + rhs.x, y + rhs.y);
	}

	template<class T>
	Vec2<T> Vec2<T>::operator-(Vec2<T> rhs) const
	{
		return Vec2<T>(x - rhs.x, y - rhs.y);
	}

	template<class T>
	T& Vec2<T>::operator[](size_t index)
	{
		if (index == 0)
		{
			return this->x;
		}
		return this->y;
	}
	
	template<class T>
	T Vec2<T>::operator[](size_t index) const
	{
		if (index == 0)
		{
			return this->x;
		}
		return this->y;
	}

	template<class T>
	Vec2<T> Vec2<T>::Zero()
	{
		return Vec2<T>(0, 0);
	}

	template<class T>
	Vec2<T> Vec2<T>::One()
	{
		return Vec2<T>(1, 1);
	}
	
	template<typename T1, typename T2>
	Vec2<T1> operator*(Vec2<T1> lhs, T2 rhs)
	{
		return Vec2<T1>(lhs.x * (T1)rhs, lhs.y * (T1)rhs);
	}

	template<typename T1, typename T2>
	Vec2<T1> operator/(Vec2<T1> lhs, T2 rhs)
	{
		return Vec2<T1>(lhs.x / (T1)rhs, lhs.y / (T1)rhs);
	}

	template<class T>
	Vec2<T>& Vec2<T>::operator-=(const Vec2<T> rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;

		return *this;
	}

	template<class T>
	Vec2<T>& Vec2<T>::operator+=(const Vec2<T> rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;

		return *this;
	}

	template<class T>
	Vec2<T>& Vec2<T>::operator*=(const Vec2<T> rhs)
	{
		this->x *= rhs.x;
		this->y *= rhs.y;

		return *this;
	}

	template<class T>
	Vec2<T>& Vec2<T>::operator/=(const Vec2<T> rhs)
	{
		this->x /= rhs.x;
		this->y /= rhs.y;

		return *this;
	}

	/***************************************************************************************************
	*
	* Vec 3
	* - takes in a template class
	*
	***************************************************************************************************/
	//Default constructor 
	template<class T>
	Vec3<T>::Vec3() : x(0), y(0), z(0) {}

	//Parameterized Constructor
	template<class T>
	Vec3<T>::Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

	//Copy constructor
	template<class T>
	Vec3<T>::Vec3(const Vec3<T>& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

	//Copy assignment constructor
	template<class T>
	Vec3<T>& Vec3<T>::operator=(const Vec3<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	template<class T>
	Vec3<T> Vec3<T>::operator+(Vec3<T> rhs) const
	{
		return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	template<class T>
	Vec3<T> Vec3<T>::operator-(Vec3<T> rhs) const
	{
		return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	template<class T>
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

	template<class T>
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

	template<class T>
	Vec3<T> Vec3<T>::Zero()
	{
		return Vec3<T>(0, 0, 0);
	}

	template<class T>
	Vec3<T> Vec3<T>::One()
	{
		return Vec3<T>(1, 1, 1);
	}

	template<typename T1, typename T2>
	Vec3<T1> operator*(Vec3<T1> lhs, T2 rhs)
	{
		return Vec3<T1>(lhs.x * (T1)rhs, lhs.y * (T1)rhs, lhs.z * (T1)rhs);
	}

	template<typename T1, typename T2>
	Vec3<T1> operator/(Vec3<T1> lhs, T2 rhs)
	{
		return Vec3<T1>(lhs.x / (T1)rhs, lhs.y / (T1)rhs, lhs.z / (T1)rhs);
	}

	template<class T>
	Vec3<T>& Vec3<T>::operator-=(const Vec3<T> rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;

		return *this;
	}

	template<class T>
	Vec3<T>& Vec3<T>::operator+=(const Vec3<T> rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;

		return *this;
	}

	template<class T>
	Vec3<T>& Vec3<T>::operator*=(const Vec3<T> rhs)
	{
		this->x *= rhs.x;
		this->y *= rhs.y;
		this->z *= rhs.z;

		return *this;
	}

	template<class T>
	Vec3<T>& Vec3<T>::operator/=(const Vec3<T> rhs)
	{
		this->x /= rhs.x;
		this->y /= rhs.y;
		this->z /= rhs.z;

		return *this;
	}

	/***************************************************************************************************
	*
	* Vec 4
	* - takes in a template class
	* - colour RGBA (requires Vec4)
	*
	***************************************************************************************************/
	//Default constructor 
		template<class T>
		Vec4<T>::Vec4() : x(0), y(0), z(0), w(0) {}

		//Parameterized Constructor
		template<class T>
		Vec4<T>::Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

		//Copy constructor
		template<class T>
		Vec4<T>::Vec4(const Vec4<T>& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}

		//Copy assignment constructor
		template<class T>
		Vec4<T>& Vec4<T>::operator=(const Vec4<T>& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}

		template<class T>
		Vec4<T> Vec4<T>::operator+(Vec4<T> rhs) const
		{
			return Vec4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
		}

		template<class T>
		Vec4<T> Vec4<T>::operator-(Vec4<T> rhs) const
		{
			return Vec4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}

		template<class T>
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

		template<class T>
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

		template<class T>
		Vec4<T> Vec4<T>::Zero()
		{
			return Vec4<T>(0, 0, 0, 0);
		}

		template<class T>
		Vec4<T> Vec4<T>::One()
		{
			return Vec4<T>(1, 1, 1, 1);
		}

		template<typename T1, typename T2>
		Vec4<T1> operator*(Vec4<T1> lhs, T2 rhs)
		{
			return Vec4<T1>(lhs.x * (T1)rhs, lhs.y * (T1)rhs, lhs.z * (T1)rhs, lhs.w * (T1)rhs);
		}

		template<typename T1, typename T2>
		Vec4<T1> operator/(Vec4<T1> lhs, T2 rhs)
		{
			return Vec4<T1>(lhs.x / (T1)rhs, lhs.y / (T1)rhs, lhs.z / (T1)rhs, lhs.w / (T1)rhs);
		}

		template<class T>
		Vec4<T>& Vec4<T>::operator-=(const Vec4<T> rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			this->w -= rhs.w;

			return *this;
		}

		template<class T>
		Vec4<T>& Vec4<T>::operator+=(const Vec4<T> rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			this->w += rhs.w;

			return *this;
		}

		template<class T>
		Vec4<T>& Vec4<T>::operator*=(const Vec4<T> rhs)
		{
			this->x *= rhs.x;
			this->y *= rhs.y;
			this->z *= rhs.z;
			this->w *= rhs.w;

			return *this;
		}

		template<class T>
		Vec4<T>& Vec4<T>::operator/=(const Vec4<T> rhs)
		{
			this->x /= rhs.x;
			this->y /= rhs.y;
			this->z /= rhs.z;
			this->w /= rhs.w;

			return *this;
		}
}