#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	static AudioManager* GetInstance();

	void PlayMusic(const std::string& fileName);
	void PauseMusic();
	void StopMusic(Mix_Chunk* sound);
	void PlayFX(const std::string& fileName) const;

	bool IsPaused() const;
	bool IsStopped() const;
	bool IsPlaying() const;
	bool InErrorState() const;

private:
	static AudioManager* audioManagerInstance;

	enum AudioState
	{
		ERROR = 0,
		WAITING,
		PAUSED,
		STOPPED,
		PLAYING
	};

	static AudioState currentState;

	std::string m_sCurrentMusicFilename = "";

	void InitAudioDevice();

private:
	AudioManager & operator=(const AudioManager&)
	{
	}
};

