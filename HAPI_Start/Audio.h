#pragma once
#include <map>
#include <HAPI_lib.h>

#include "Sound.h"

class Audio {
private:
	std::map<std::string, std::shared_ptr<Sound>> m_sounds;
public:
	bool LoadSound(const std::string& s, const std::string& i, bool l, float pan, float pitch, float v);
	void PlaySound(const std::string& i);
};