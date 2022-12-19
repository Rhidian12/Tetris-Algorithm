#pragma once

#include "../Integrian2D_API.h"

#include "../AudioSystem/AudioSystem.h"

#include <vector>
#include <map>

struct Mix_Chunk;
struct _Mix_Music;

namespace Integrian2D
{
	class SDLAudioSystem final : public AudioSystem
	{
	public:
		INTEGRIAN2D_API SDLAudioSystem();
		INTEGRIAN2D_API virtual ~SDLAudioSystem();

		/* Add a sound to the SDLAudioSystem */
		INTEGRIAN2D_API virtual SoundID AddSound(const std::string& filePath) override;

		/* Add music to the SDLAudioSystem */
		INTEGRIAN2D_API virtual MusicID AddMusic(const std::string& filePath) override;

		/* Internal Usage */
		INTEGRIAN2D_API virtual void Update() override;

		/* Play a previously added Sound
			The Sound plays either infinitely, or amountOfLoops + 1
			Volume is a percentage in the range of [0, 100] */
		INTEGRIAN2D_API virtual void PlaySound(const SoundID soundID, const bool infiniteLoop = false, const int amountOfLoops = 0, const int volume = 100) override;

		/* Play previously added Music
			The Music plays either infinitely, or amountOfLoops + 1 
			Volume is a percentage in the range of [0, 100] */
		INTEGRIAN2D_API virtual void PlayMusic(const MusicID musicID, const bool infiniteLoop = false, const int amountOfLoops = 0, const int volume = 100) override;

		/* Pause the Music playing */
		INTEGRIAN2D_API virtual void PauseMusic() override;

		/* Pause the Sound playing */
		INTEGRIAN2D_API virtual void PauseSound(const SoundID soundID) override;

		/* Rewind the Music to the start */
		INTEGRIAN2D_API virtual void RewindMusic() override;

		/* Set the Music's position to the passed parameter, measured from the beginning of the song, in seconds */
		INTEGRIAN2D_API virtual void SetMusicPosition(double time) override;

		/* Set the volume of the Sound 
			The passed parameter must be a percentage in the range of [0, 100] */
		INTEGRIAN2D_API virtual void SetSoundVolume(const SoundID soundID, const int volume) override;

		/* Set the volume of the Music
			The passed parameter must be a percentage in the range of [0, 100] */
		INTEGRIAN2D_API virtual void SetMusicVolume(const int volume) override;

		/* Check whether Music is playing */
		INTEGRIAN2D_API virtual bool IsMusicPlaying() const override;

		/* Check whether Sound is playing */
		INTEGRIAN2D_API virtual bool IsSoundPlaying(const SoundID soundID) const override;

		/* Get the volume of the Sound in the range of [0, 100] */
		INTEGRIAN2D_API virtual int GetSoundVolume(const SoundID soundID) const override;

		/* Get the volume of the Music in the range of [0, 100] */
		INTEGRIAN2D_API virtual int GetMusicVolume() const override;

	private:
		struct Channel final
		{
			Channel(const uint16_t index)
				: channelIndex{ index }
			{}

			bool isInUse{ false };
			uint16_t channelIndex; // uint16_t because the range is [0 - 7]
			float timeInUse{};
			float expectedTimeInUse{};
			SoundID soundIDOfChunk{ std::numeric_limits<int>::max() }; // use max to minimize the possibility of it taking an existing ID
		};

		Channel& GetFirstAvailableChannel(std::vector<Channel>& channels);
		float GetChunkTimeInSeconds(Mix_Chunk* pChunk) const; // reference: https://discourse.libsdl.org/t/time-length-of-sdl-mixer-chunks/12852
		int RemapVolumeToSDL(const int volumeInPercentage) const;
		int RemapVolumeToIntegrian(const int volumeInSDL) const;

		std::map<SoundID, Mix_Chunk*> m_Sounds;
		std::map<MusicID, _Mix_Music*> m_Music;

		std::vector<Channel> m_Channels;

		_Mix_Music* m_pCurrentPlayingMusic;

		const int m_SDLMixerMaxVolume; /* SDL_Mixer */

		using SoundPair = std::pair<SoundID, Mix_Chunk*>;
		using MusicPair = std::pair<MusicID, _Mix_Music*>;
	};
}