#pragma once

namespace kiko {
	class Rect {
	public:
		int x, y, w, h;
	public:
		Rect() : x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 } {}
		Rect(int r, int g, int b, int a = 1) : x{ x }, y{ y }, w{ w }, h{ h } {}

		int operator [] (size_t index) const { return (&x)[index]; }
		int& operator [] (size_t index) { return (&x)[index]; }
	};
}