#pragma once
#include <sstream>

namespace kiko {
	class Vector3 {
	public:
		float x, y, z;
	public:
		Vector3() : x{ 0 }, y{ 0 }, z{ 0 } {}
		Vector3(float v) : x{ v }, y{ v }, z{ v } {}
		Vector3(float x, float y) : x{ x }, y{ y }, z{ 0 } {}
		Vector3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}

		float operator [] (size_t index) const { return (&x)[index]; }
		float& operator [] (size_t index) { return (&x)[index]; }

		Vector3 operator - () const { return Vector3(-x, -y); }
			  
		Vector3 operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y); }
		Vector3 operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y); }
		Vector3 operator * (const Vector3& v) const { return Vector3(x * v.x, y * v.y); }
		Vector3 operator / (const Vector3& v) const { return Vector3(x / v.x, y / v.y); }
			  
		Vector3 operator + (float s) const { return Vector3(x + s, y + s); }
		Vector3 operator - (float s) const { return Vector3(x - s, y - s); }
		Vector3 operator * (float s) const { return Vector3(x * s, y * s); }
		Vector3 operator / (float s) const { return Vector3(x / s, y / s); }
			  
		Vector3 operator += (const Vector3& v) { x += v.x; y += v.y; return *this; }
		Vector3 operator -= (const Vector3& v) { x -= v.x; y -= v.y; return *this; }
		Vector3 operator *= (const Vector3& v) { x *= v.x; y *= v.y; return *this; }
		Vector3 operator /= (const Vector3& v) { x /= v.x; y /= v.y; return *this; }

		float LengthSqr() const { return (x * x) + (y * y); }
		float Length() const { return sqrt(LengthSqr()); }

		float DistanceSqr(const Vector3& v) const { return (v - *this).LengthSqr(); }
		float Distance(const Vector3& v) const { return (v - *this).Length(); }

		Vector3 Normalized() const { return *this / Length(); }
		void Normalize() { *this /= Length(); }

		static float Dot(const Vector3& v1, const Vector3& v2);
	};

	// get the dot product beteen v1 and v2 https://www.falstad.com/dotproduct/
	inline float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	inline std::istream& operator >> (std::istream& stream, Vector3& v) {
		std::string line;
		std::getline(stream, line);

		// { ##, ## }
		std::string xs = line.substr(line.find("{") + 1, line.find(",") - (line.find("{") + 1));
		v.x = std::stof(xs);

		std::string ys = line.substr(line.find(",") + 1, line.find("}") - (line.find(",") + 1));
		v.y = std::stof(ys);

		return stream;
	}
	using vec3 = Vector3;
}