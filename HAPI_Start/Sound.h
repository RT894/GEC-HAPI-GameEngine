#pragma once
#include <HAPI_lib.h>

class Sound {
private:
	std::string m_fileName;
	HAPISPACE::HAPI_TSoundOptions m_options;
public:
	Sound(const std::string& f, bool l, float pan, float pitch, float v);
	void Play();
};