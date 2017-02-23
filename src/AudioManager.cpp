#include "AudioManager.h"

//#include "ResourceManager.h"
//#include "utilities.h"

// Initialize our static variables
AudioManager* AudioManager::audioManagerInstance = 0;
AudioManager::AudioState AudioManager::currentState = ERROR;

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
	}
	return audioManagerInstance;
}

void AudioManager::InitAudioDevice()
{
	if (SDL_Init(SDL_INIT_AUDIO) != -1)
	{
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			std::cerr << "Error initializing audio device...\n";
			currentState = ERROR;
		}
		else
		{
			currentState = WAITING;
		}
	}
	else
	{
		std::cerr << "Error initializing SDL audio subsystem...\n";
		currentState = ERROR;
	}
}

void AudioManager::PlayMusic(const std::string& fileName)
{
	if (currentState != ERROR)
	{
		// If no music is playing, play it
		if (Mix_PlayingMusic() == 0)
		{
			// Load music

			Mix_Music* music = Mix_LoadMUS(fileName.c_str());
				
			//Play music
			Mix_PlayMusic(music, -1);
			currentState = PLAYING;
			m_sCurrentMusicFilename = fileName.c_str();
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
	if (currentState != ERROR)
	{
		// If music is playing, handle the pause request
		if (Mix_PlayingMusic() == 1)
		{
			if (Mix_PausedMusic() == 1)
			{
				// If we receive a pause request and the music is already paused, resume it.
				Mix_ResumeMusic();
				currentState = PLAYING;
			}
			else
			{
				// Otherwise, pause the music
				Mix_PauseMusic();
				currentState = PAUSED;
			}
		}
	}
}

void AudioManager::StopMusic(Mix_Chunk* sound)
{
	if (currentState != ERROR)
	{
		Mix_HaltMusic();
		currentState = STOPPED;
		Mix_FreeChunk(sound);
		m_sCurrentMusicFilename = "";
	}
}

void AudioManager::PlayFX(const std::string& fileName) const
{
	if (currentState != ERROR)
	{
		// TODO: Alter this to work for mp3 as well

		Mix_Chunk* fx = Mix_LoadWAV(fileName.c_str());

		Mix_PlayChannel(-1, fx, 0);		
	}
}

bool AudioManager::IsPaused() const
{
	return currentState == PAUSED;
}

bool AudioManager::IsStopped() const
{
	return currentState == STOPPED;
}

bool AudioManager::IsPlaying() const
{
	return currentState == PLAYING;
}

bool AudioManager::InErrorState() const
{
	return currentState == ERROR;
}
