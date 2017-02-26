#include "AudioManager.h"

//#include "ResourceManager.h"
//#include "utilities.h"

// Initialize our static variables
AudioManager* AudioManager::audioManagerInstance = 0;
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
	if (audioManagerInstance == 0)
	{
		audioManagerInstance = new AudioManager;
		audioManagerInstance->InitAudioDevice();
	}
	return audioManagerInstance;
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

		//Mix_Chunk *sound = Mix_LoadWAV(fileName.c_str());

		if (_soundMap.at(fileName) == NULL)
		{
			//Mix_FreeChunk(sound);
			_currentState = ERROR;
			printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		}

		else if(Mix_Playing(SChannel) == 0)
		{
			if (_previousSound != nullptr)
			{
				//Mix_FreeChunk(_previousSound);
			}

			SChannel = Mix_PlayChannel(-1, _soundMap.at(fileName), 0);
			_currentState = PLAYING;
			//_previousSound = sound;
		}

		else if (Mix_Playing(SChannel) != 0)
		{
			//Mix_FreeChunk(sound);
		}
	}
}

void AudioManager::SetVolume(bool volume)
{
	if (volume) 
	{
		// set the music volume to 1/2 maximum, and then check it
		printf("volume was    : %d\n", Mix_VolumeMusic(Mix_VolumeMusic(-1) / 2));
		printf("volume is now : %d\n", Mix_VolumeMusic(-1));
	}

	else if (!volume) 
	{
		// set the music volume to 1/2 maximum, and then check it
		printf("volume was    : %d\n", Mix_VolumeMusic(Mix_VolumeMusic(-1) * 2));
		printf("volume is now : %d\n", Mix_VolumeMusic(-1));

		if (Mix_VolumeMusic(-1) == 0)
		{
			Mix_VolumeMusic(Mix_VolumeMusic(-1) + 1);
		}
	}
}