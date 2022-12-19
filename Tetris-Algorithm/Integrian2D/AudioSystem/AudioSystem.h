#pragma once

#include "../Integrian2D_API.h"
#include <string>

namespace Integrian2D
{
	class INTEGRIAN2D_API AudioSystem
	{
	public:
		using SoundID = unsigned int;
		using MusicID = unsigned int;

		virtual ~AudioSystem() = default;

		virtual SoundID AddSound(const std::string&) = 0;
		virtual MusicID AddMusic(const std::string&) = 0;

		virtual void Update() = 0;

		virtual void PlaySound(const SoundID, const bool = false, const int = 0, const int = 100) = 0;
		virtual void PlayMusic(const MusicID, const bool = false, const int = 0, const int = 100) = 0;

		virtual void PauseMusic() = 0;
		virtual void PauseSound(const SoundID) = 0;

		virtual void RewindMusic() = 0;
		virtual void SetMusicPosition(double) = 0;

		virtual void SetSoundVolume(const SoundID, const int) = 0;
		virtual void SetMusicVolume(const int) = 0;

		virtual bool IsMusicPlaying() const = 0;
		virtual bool IsSoundPlaying(const SoundID) const = 0;

		virtual int GetSoundVolume(const SoundID) const = 0;
		virtual int GetMusicVolume() const = 0;
	};
}