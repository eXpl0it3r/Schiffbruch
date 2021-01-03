#include "Sound.hpp"

namespace Sound {
std::vector<sf::SoundBuffer> sound_buffers; // Sound::COUNT - Wavedateispeicher
std::vector<sf::Sound> sound_players; // Sound::COUNT

void Init()
{
    sound_buffers.resize(25);
    sound_players.resize(25);
}

void LoadSound(short Sound)
{
    if (s_SoundState == -1) { // Wenn keine Soundkarte vorhanden raus...
        return;
    }

    if (!sound_buffers[Sound].loadFromFile(Wav[Sound].Filename)) {
        sf::err() << "Couldn't load sound file '" << Wav[Sound].Filename << "'!" << std::endl;
    }

    sound_players[Sound].setBuffer(sound_buffers[Sound]);
    sound_players[Sound].setVolume(static_cast<float>(Wav[Sound].Volume));
    sound_players[Sound].setLoop(Wav[Sound].Loop);
}

void PlaySound(short Sound, short Volume)
{
    if ((Sound == 0) || (s_SoundState <= 0) || (sound_players[Sound].getStatus() == sf::Sound::Playing)) {
        return;
    }

    sound_players[Sound].play();
}

void StopSound(short Sound)
{
    if ((Sound == 0) || (s_SoundState <= 0)) {
        return;
    }

    sound_players[Sound].stop();
}
} // namespace Sound
