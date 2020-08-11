#pragma once
#include "DxLib.h"

struct Color {
	Color() : m_r(0), m_g(0), m_b(0), m_color(0U){
	}
	Color(int r, int g, int b) {
		Set(r, g, b);
	}
	Color(unsigned int color) {
		m_color = color;
		GetColor2(color, &m_r, &m_g, &m_b);
	}
	Color& operator =(const unsigned int& q) {
		m_color = q;
		GetColor2(q, &m_r, &m_g, &m_b);
		return *this;
	}
	void Set(int r, int g, int b) {
		m_r = r, m_g = g, m_b = b;
		m_color = DxLib::GetColor(r, g, b);
	}
	unsigned int Get() {
		return m_color;
	}
	void Get(int& r, int& g, int& b) {
		r = m_r;
		g = m_g;
		b = m_b;
	}
	int GetR() { return m_r; }
	int GetG() { return m_g; }
	int GetB() { return m_b; }
	COLOR_U8 GetU8(int a) {
		return GetColorU8(m_r, m_g, m_b, a);
	}

private:
	int m_r, m_g, m_b;
	//int m_h, m_s, m_v;
	unsigned int m_color;
};

struct ColorU8 {
	ColorU8() {
		m_color = GetColorU8(0, 0, 0, 0);
	}
	ColorU8(int r, int g, int b, int a) {
		m_color = GetColorU8(r, g, b, a);
	}
	ColorU8(COLOR_U8 color) {
		m_color = color;
	}
	ColorU8& operator =(const COLOR_U8& q) {
		m_color = q;
		return *this;
	}
	/*ColorU8 &operator =(Color &q) {
		ColorU8(q.GetR(), q.GetG(), q.GetB(), 255);
		return *this;
	}*/
	COLOR_U8 Get() { return m_color; }
private:
	COLOR_U8 m_color;
};

typedef struct {
	Color col1, col2, col3, col4;
}ColorRect_t;