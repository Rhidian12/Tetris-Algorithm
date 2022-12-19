#pragma once

#include "../Integrian2D_API.h"
#include "../AudioSystem/AudioSystem.h"

namespace Integrian2D
{
	class AudioSystem;

	class AudioLocator final
	{
	public:
		INTEGRIAN2D_API static AudioLocator* const GetInstance() noexcept;
		static void Cleanup() noexcept;
		INTEGRIAN2D_API ~AudioLocator();

		INTEGRIAN2D_API void Provide(AudioSystem* const pAudioSystem) noexcept;

		INTEGRIAN2D_API AudioSystem* const GetAudio() const noexcept;

	private:
		AudioLocator() = default;

		class NullAudioSystem final : public AudioSystem
		{
		public:
			virtual SoundID AddSound(const std::string&) { return 0; }
			virtual MusicID AddMusic(const std::string&) { return 0; }

			virtual void Update() {}

			virtual void PlaySound(const SoundID, const bool = false, const int = 0, const int = 100) {}
			virtual void PlayMusic(const MusicID, const bool = false, const int = 0, const int = 100) {}

			virtual void PauseMusic() {}
			virtual void PauseSound(const SoundID) {}

			virtual void RewindMusic() {}
			virtual void SetMusicPosition(double) {}

			virtual void SetSoundVolume(const SoundID, const int) {}
			virtual void SetMusicVolume(const int) {}

			virtual bool IsMusicPlaying() const { return false; }
			virtual bool IsSoundPlaying(const SoundID) const { return false; }

			virtual int GetSoundVolume(const SoundID) const { return -1; }
			virtual int GetMusicVolume() const { return -1; }
		};

		inline static AudioLocator* m_pInstance{};

		NullAudioSystem* m_pNullAudioSystem{ new NullAudioSystem{} };
		AudioSystem* m_pAudioSystem{ m_pNullAudioSystem };
	};
}