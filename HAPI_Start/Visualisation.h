#pragma once
#include <memory>
#include <map>
#include <HAPI_lib.h>

using namespace HAPISPACE;

class Sprite;
class Vector3;

class Visualisation {
private:
	std::map<std::string, std::shared_ptr<Sprite>> m_sprites;
	int m_screenWidth;
	int m_screenHeight;
	BYTE* m_screen;
	float kEyeDistance{ 200.0f };
public:
	struct Stars {
		float x, y, z;
	};

	Visualisation(int w = 1024, int h = 768);
	void GenerateSprite(std::string d, std::string i, int w, int h, bool t, int aw, int ah, bool a);
	void ClearToColour(const HAPI_TColour& c, int w, int h);
	bool RenderTexture(const Vector3& p, std::string n, int f);
	void RenderDefault(const Vector3& p, const unsigned int w, const unsigned int h, HAPI_TColour& c);
	void ClearPointers();
	void RenderStars(int width, int height, float cx, float cy, Stars stars[1000], BYTE* screen);
};