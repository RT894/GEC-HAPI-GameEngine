#include "Audio.h"

bool Audio::LoadSound(const std::string& s, const std::string& i, bool l, float pan, float pitch, float v)
{
	m_sounds[i] = std::make_shared<Sound>(s, l, pan, pitch, v);

	return true;
}

void Audio::PlaySound(const std::string& i)
{
	m_sounds[i]->Play();
}
