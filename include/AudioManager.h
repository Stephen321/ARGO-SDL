#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

#include <string>
#include <iostream>
#include <map>
#include <vector>


class AudioManager
{
public:
										AudioManager();
										~AudioManager();

	static AudioManager*				GetInstance();

	void								PlayMusic(const std::string& fileName);
	void								PauseMusic();
	void								StopMusic(Mix_Chunk* sound);
	void								PlayFX(const std::string& fileName);

	void								SetVolume(bool volume);

private:
	static AudioManager*				audioManagerInstance;

	void								InitAudioDevice();
	void								LoadSounds();

	enum AudioState
	{
		ERROR = 0,
		WAITING,
		PAUSED,
		STOPPED,
		PLAYING
	};

	static AudioState					_currentState;
	std::string							_sCurrentMusicFilename = "";

	Mix_Chunk*							_previousSound;

	std::map<std::string, Mix_Chunk*>	_soundMap;
};

