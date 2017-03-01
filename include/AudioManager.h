#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

#include <string>
#include <iostream>
#include <map>
#include <vector>


class AudioManager
{
private:
	enum AudioState
	{
		ERROR = 0,
		WAITING,
		PAUSED,
		STOPPED,
		PLAYING
	};

public:
										AudioManager();
										~AudioManager();

	static AudioManager*				GetInstance();

	void								PlayMusic(const std::string& fileName);
	void								PauseMusic();
	void								StopMusic(Mix_Chunk* sound);
	void								PlayFX(const std::string& fileName);

	void								SetMusicVolume(bool volume);
	int									GetMusicVolume();

	void								SetHumVolume(bool volume);
	int									GetHumVolume();

	void								SetBulletVolume(bool volume);
	int									GetBulletVolume();

private:
	void								InitAudioDevice();
	void								LoadSounds();

private:
	static AudioManager*				_audioManagerInstance;
	static AudioState					_currentState;
	std::string							_sCurrentMusicFilename = "";

	Mix_Chunk*							_previousSound;

	std::map<std::string, Mix_Chunk*>	_soundMap;

	int									_musicVolume;
	int									_humVolume;
	int									_bulletVolume;
};

