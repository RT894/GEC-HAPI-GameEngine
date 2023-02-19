#pragma once

#include <HAPI_lib.h>

using namespace HAPISPACE;

class Vector3;

class Sprite {
private:
	BYTE* m_texture;
	int m_textureWidth;
	int m_textureHeight;
	bool m_hasTransparency{ false };
	int m_sheetWidth;
	int m_sheetHeight;
	bool m_isAnimation{ false };
	int m_frameCount;
	int m_currentFrame{ 0 };
public:
	Sprite(int aw, int ah, bool i = false, std::string s = "Data\\space.png", int w = 64, int h = 64, bool t = false);
	~Sprite();
	void StepAnimation();
	size_t GetTextureStepOffset();
	void SetFrame(int i);
	void Render(const Vector3& p, int f, BYTE*& s, int sw = 1024, int sh = 768);
};