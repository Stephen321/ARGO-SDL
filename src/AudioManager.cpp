#include "AudioManager.h"

//#include "ResourceManager.h"
//#include "utilities.h"

// Initialize our static variables
AudioManager* AudioManager::_audioManagerInstance = 0;
AudioManager::AudioState AudioManager::_currentState = ERROR;

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	Mix_CloseAudio();
}

// Singleton pattern
AudioManager* AudioManager::GetInstance()
{
	if (_audioManagerInstance == 0)
	{
		_audioManagerInstance = new AudioManager;
		_audioManagerInstance->InitAudioDevice();
	}
	return _audioManagerInstance;
}

void AudioManager::InitAudioDevice()
{
	if (SDL_Init(SDL_INIT_AUDIO) != -1)
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			std::cerr << "Error initializing audio device...\n";
			_currentState = ERROR;
		}
		else
		{
			_currentState = WAITING;
		}
	}
	else
	{
		std::cerr << "Error initializing SDL audio subsystem...\n";
		_currentState = ERROR;
	}

	LoadSounds();
}

void AudioManager::LoadSounds()
{
	_soundMap.insert(std::make_pair("Hum", Mix_LoadWAV("Media/Audio/Hum.wav")));
	_humVolume = 64;
	Mix_VolumeChunk(_soundMap.at("Hum"), _humVolume);

	_musicVolume = 64;
	Mix_VolumeMusic(_musicVolume);


	_bulletVolume = 64;
}

void AudioManager::PlayMusic(const std::string& fileName)
{
	if (_currentState != ERROR)
	{
		Mix_Music * music = Mix_LoadMUS(fileName.c_str());
		// If no music is playing, play it
		if (Mix_PlayingMusic() == 0)
		{
			if (music == NULL)
			{
				printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
			}
			//Play music
			Mix_PlayMusic(music, -1);
		}
		else
		{
			// If music is playing, pause it
			this->PauseMusic();
		}
	}
}

void AudioManager::PauseMusic()
{
	if (_currentState != ERROR)
	{
		// If music is playing, handle the pause request
		if (Mix_PlayingMusic() == 1)
		{
			if (Mix_PausedMusic() == 1)
			{
				// If we receive a pause request and the music is already paused, resume it.
				Mix_ResumeMusic();
				_currentState = PLAYING;
			}
			else
			{
				// Otherwise, pause the music
				Mix_PauseMusic();
				_currentState = PAUSED;
			}
		}
	}
}

void AudioManager::StopMusic(Mix_Chunk* sound)
{
	if (_currentState != ERROR)
	{
		Mix_HaltMusic();
		_currentState = STOPPED;
	}
}

void AudioManager::PlayFX(const std::string& fileName)
{
	if (_currentState != ERROR)
	{
		int SChannel = 0;

		if (_soundMap.at(fileName) == NULL)
		{
			_currentState = ERROR;
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		}

		else if(Mix_Playing(SChannel) == 0)
		{
			SChannel = Mix_PlayChannel(-1, _soundMap.at(fileName), 0);
			_currentState = PLAYING;
		}
	}
}

void AudioManager::SetMusicVolume(bool volume)
{
	//if (!volume) 
	//{
	//	printf("volume was    : %d\n", Mix_VolumeMusic(Mix_VolumeMusic(-1) * 1));
	//	printf("volume is now : %d\n", Mix_VolumeMusic(-1));
	//	_volume = Mix_VolumeMusic(-1);
	//}

	//else if (volume) 
	//{
	//	// set the music volume to 1/2 maximum, and then check it
	//	printf("volume was    : %d\n", Mix_VolumeMusic(Mix_VolumeMusic(-1) + 1));
	//	printf("volume is now : %d\n", Mix_VolumeMusic(-1));
	//	_volume = Mix_VolumeMusic(-1);
	//}

	if (!volume)
	{
		if (_musicVolume >= 0 + 4) { _musicVolume -= 4; }
		printf("volume was    : %d\n", Mix_VolumeMusic(_musicVolume));
	}

	else if (volume)
	{
		if (_musicVolume <= 128 - 4) { _musicVolume += 4; }
		printf("volume was    : %d\n", Mix_VolumeMusic(_musicVolume));
	}
}

int AudioManager::GetMusicVolume()
{
	return _musicVolume;
}

void AudioManager::SetHumVolume(bool volume)
{
	if (!volume)
	{
		if (_humVolume >= 0 + 4) { _humVolume -= 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Hum"), _humVolume));
	}

	else if (volume)
	{
		if (_humVolume <= 128 - 4) { _humVolume += 4; }
		printf("volume was    : %d\n", Mix_VolumeChunk(_soundMap.at("Hum"), _humVolume));
	}
}

int AudioManager::GetHumVolume()
{
	return _humVolume;
}