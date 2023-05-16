/**
+* @file VecMath.h
+*/
#ifndef VECMATH_H_INCLUDED
#define VECMATH_H_INCLUDED
#include <cmath>

namespace VecMath
{
	inline constexpr float pi = 3.14159265f; // ‰~ü—¦
	inline constexpr float invPi = 1 / pi;   // ‰~ü—¦‚Ì‹t”

	//“x”–@‚©‚çŒÊ“x–@‚Ö•ÏŠ·
	inline constexpr float radians(float d) { return d / 180.0f * pi; }

	//ŒÊ“x–@‚©‚ç“x”–@‚Ö•ÏŠ·
	inline constexpr float degrees(float r) { return r / pi * 180.0f; }

	/**
	* 2DƒxƒNƒgƒ‹
	*/
	struct vec2
	{
		float x, y;

		vec2() = default;
		explicit vec2(float s) : x(s), y(s) {}
		vec2(float x, float y) : x(x), y(y) {}

		//æZ
		vec2 & operator*=(const vec2 & other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
		friend vec2 operator*(vec2 a, const vec2& b) { return a *= b; }
		vec2& operator*=(float other) { return *this *= vec2(other); }
		friend vec2 operator*(vec2 a, float b) { return a *= b; }
		friend vec2 operator*(float a, const vec2 & b) { return vec2(a) *= b; }

		//œZ
		vec2& operator/=(const vec2& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}
		friend vec2 operator/(vec2 a, const vec2& b) { return a /= b; }
		vec2& operator/=(float other) { return *this /= vec2(other); }
		friend vec2 operator/(vec2 a, float b) { return a /= b; }
		friend vec2 operator/(float a, const vec2& b) { return vec2(a) /= b; }


		//‰ÁZ
		vec2 & operator+=(const vec2 & other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		friend vec2 operator+(vec2 a, const vec2 & b) { return a += b; }
		vec2 & operator+=(float other) { return *this += vec2(other); }
		friend vec2 operator+(vec2 a, float b) { return a += b; }
		friend vec2 operator+(float a, const vec2 & b) { return vec2(a) += b; }

		//Œ¸Z
		vec2 & operator-=(const vec2 & other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		friend vec2 operator-(vec2 a, const vec2 & b) { return a -= b; }
		vec2 & operator-=(float other) { return *this -= vec2(other); }
		friend vec2 operator-(vec2 a, float b) { return a -= b; }
		friend vec2 operator-(float a, const vec2 & b) { return vec2(a) -= b; }

		//’P€ƒ}ƒCƒiƒX‰‰Zq
		vec2 operator-() const { return vec2(-x, -y); }

		//“Yš‰‰Zq
		float& operator[](size_t n) { return *(&x + n); }
		const float& operator[](size_t n) const { return *(&x + n); }
	};
	
	/**
	* 3DƒxƒNƒgƒ‹
	*/
	struct vec3
	{
		float x, y, z;

		vec3() = default;
		explicit vec3(float s) : x(s), y(s), z(s) {}
		vec3(float x, float y, float z) : x(x), y(y), z(z) {}
		vec3(const vec2 & xy, float z) : x(xy.x), y(xy.y), z(z) {}

		//æZ
		vec3 & operator*=(const vec3 & other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		friend vec3 operator*(vec3 a, const vec3 & b) { return a *= b; }
		vec3 & operator*=(float other) { return *this *= vec3(other); }
		friend vec3 operator*(vec3 a, float b) { return a *= b; }
		friend vec3 operator*(float a, const vec3 & b) { return vec3(a) *= b; }

		//œZ
		vec3& operator/=(const vec3& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}
		friend vec3 operator/(vec3 a, const vec3& b) { return a /= b; }
		vec3& operator/=(float other) { return *this /= vec3(other); }
		friend vec3 operator/(vec3 a, float b) { return a /= b; }
		friend vec3 operator/(float a, const vec3& b) { return vec3(a) /= b; }

		//‰ÁZ
		vec3& operator+=(const vec3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		friend vec3 operator+(vec3 a, const vec3& b) { return a += b; }
		vec3& operator+=(float other) { return *this += vec3(other); }
		friend vec3 operator+(vec3 a, float b) { return a += b; }
		friend vec3 operator+(float a, const vec3& b) { return vec3(a) += b; }

		//Œ¸Z
		vec3& operator-=(const vec3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		friend vec3 operator-(vec3 a, const vec3& b) { return a -= b; }
		vec3& operator-=(float other) { return *this -= vec3(other); }
		friend vec3 operator-(vec3 a, float b) { return a -= b; }
		friend vec3 operator-(float a, const vec3& b) { return vec3(a) -= b; }

		//’P€ƒ}ƒCƒiƒX‰‰Zq
		vec3 operator-() const { return vec3(-x, -y, -z); }

		//“Yš‰‰Zq
		float& operator[](size_t n) { return *(&x + n); }
		const float& operator[](size_t n) const { return *(&x + n); }
	};

	//“àÏ
	inline float dot(const vec3 & a, const vec3 & b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	
	//’·‚³
	inline float length(const vec3 & v)
	{
		return sqrt(dot(v, v));
	}
	
	//³‹K‰»
	inline vec3 normalize(const vec3 & v)
	{
		const float d = 1.0f / length(v);
		return v * d;
	}
	
	//ŠOÏ
	inline vec3 cross(const vec3 & a, const vec3 & b)
	{
		return vec3(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	/**
	* 4DƒxƒNƒgƒ‹
	*/
	struct vec4
	{
		float x, y, z, w;
		
		vec4() = default;
		explicit vec4(float s) : x(s), y(s), z(s), w(s){}
		vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){}
		vec4(const vec3 & xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w){}

		//æZ
		vec4& operator*=(const vec4& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}

		friend vec4 operator*(vec4 a, const vec4& b) { return a *= b; }
		vec4& operator*=(float other) { return *this *= vec4(other); }
		friend vec4 operator*(vec4 a, float b) { return a *= b; }
		friend vec4 operator*(float a, const vec4& b) { return vec4(a) *= b; }

		//‰ÁZ
		vec4& operator+=(const vec4& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return *this;
		}
		friend vec4 operator+(vec4 a, const vec4& b) { return a += b; }
		vec4& operator+=(float other) { return *this += vec4(other); }
		friend vec4 operator+(vec4 a, float b) { return a += b; }
		friend vec4 operator+(float a, const vec4& b) { return vec4(a) += b; }

		//œZ
		vec4& operator/=(const vec4& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w /= other.w;
			return *this;
		}
		friend vec4 operator/(vec4 a, const vec4& b) { return a /= b; }
		vec4& operator/=(float other) { return *this /= vec4(other); }
		friend vec4 operator/(vec4 a, float b) { return a /= b; }
		friend vec4 operator/(float a, const vec4& b) { return vec4(a) /= b; }

		//Œ¸Z
		vec4& operator-=(const vec4& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return *this;
		}
		friend vec4 operator-(vec4 a, const vec4& b) { return a -= b; }
		vec4& operator-=(float other) { return *this -= vec4(other); }
		friend vec4 operator-(vec4 a, float b) { return a -= b; }
		friend vec4 operator-(float a, const vec4& b) { return vec4(a) -= b; }

		//’P€ƒ}ƒCƒiƒX‰‰Zq
		vec4 operator-() const { return vec4(-x, -y, -z, -w); }

		//“Yš‰‰Zq
		float& operator[](size_t n) { return *(&x + n); }
		const float& operator[](size_t n) const { return *(&x + n); }
	};

	/**
	* 3x3s—ñ
	*/
	struct mat3
	{
		vec3 data[3];

		//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
		mat3() = default;
		mat3(const vec3& v0, const vec3& v1, const vec3& v2)
		{
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
		}
		explicit mat3(float s)
		{
			data[0] = vec3(s, 0, 0);
			data[1] = vec3(0, s, 0);
			data[2] = vec3(0, 0, s);
		}
		//“Yš‰‰Zq
		vec3& operator[](size_t n) { return data[n]; }
		const vec3& operator[](size_t n) const { return data[n]; }
	};

	/**
	* 4x4s—ñ
	*/
	struct mat4
	{
		vec4 data[4];

		mat4() = default;
		mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3)
		{
			data[0] = v0;
			data[1] = v1;
			data[2] = v2;
			data[3] = v3;
		}
		explicit mat4(float s)
		{
			data[0] = vec4(s, 0, 0, 0);
			data[1] = vec4(0, s, 0, 0);
			data[2] = vec4(0, 0, s, 0);
			data[3] = vec4(0, 0, 0, s);
		}

		//“Yš‰‰Zq
		vec4& operator[](size_t n) { return data[n]; }
		const vec4& operator[](size_t n) const { return data[n]; }

		//s—ñ‚ÆƒxƒNƒgƒ‹‚ÌæZ
		friend vec4 operator*(const mat4 & m, const vec4 & v) 
		{
			return m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w;
		}

		//s—ñ‚Æs—ñ‚ÌæZ
		mat4 & operator*=(const mat4 & other);
		friend mat4 operator*(const mat4 & m0, const mat4 & m1) 
		{
			return mat4(m0) *= m1;
		}

		static mat4 Translate(const vec3& v);
		static mat4 Scale(const vec3& v);
		static mat4 RotateX(float angle);
		static mat4 RotateY(float angle);
		static mat4 RotateZ(float angle);
		static mat4 LookAt(const vec3& eye, const vec3& target, const vec3& up);
		static mat4 Orthogonal(float left, float right, float bottom, float top, float zNear, float zFar);
	};
}//namespace VecMath
#endif // VECMATH_H_INCLUDED
