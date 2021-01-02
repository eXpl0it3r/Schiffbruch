#include "Sound.hpp"

#include <vector>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Err.hpp>

#include "extern.hpp"

namespace Sound
{
    std::vector<sf::SoundBuffer> SoundBuffers; // Wavedateispeicher
    std::vector<sf::Sound> SoundPlayers;

    void Initialize()
    {
        SoundBuffers.resize(25);
        SoundPlayers.resize(25);
    }

    void LoadSound(const short sound)
    {
        if (Soundzustand == -1) // Wenn keine Soundkarte vorhanden raus...
        {
            return;
        }

        if (!SoundBuffers[sound].loadFromFile(Wav[sound].Dateiname))
        {
            sf::err() << "Couldn't load sound file '" << Wav[sound].Dateiname << "'!" << std::endl;
        }

        SoundPlayers[sound].setBuffer(SoundBuffers[sound]);
        SoundPlayers[sound].setVolume(static_cast<float>(Wav[sound].Volume));
        SoundPlayers[sound].setLoop(Wav[sound].Loop);
    }

    void PlaySound(const short sound, const short volume)
    {
        if (sound == 0
            || Soundzustand <= 0
            || SoundPlayers[sound].getStatus() == sf::Sound::Playing)
        {
            return;
        }

        SoundPlayers[sound].play();
        SoundPlayers[sound].setVolume(static_cast<float>(volume));
    }

    void StopSound(const short sound)
    {
        if (sound == 0 || Soundzustand <= 0)
        {
            return;
        }

        SoundPlayers[sound].stop();
    }
} // namespace Sound
