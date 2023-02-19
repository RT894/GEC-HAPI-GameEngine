#include "Sound.h"

Sound::Sound(const std::string& f, bool l, float pan, float pitch, float v)
{
	//Kills the game if a sound that does not exist attempts to load
	if (!HAPI.LoadSound(f)) {
		HAPI.UserMessage("Sound Failed To Load", "ERROR");
		HAPI.Close();
	}

	m_fileName = f;

	m_options.loop = l;
	m_options.pan = pan;
	m_options.pitch = pitch;
	m_options.volume = v;
}

void Sound::Play()
{
	HAPI.PlaySound(m_fileName, m_options);
}
