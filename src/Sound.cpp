#include "Sound.hpp"

#include "bin/sounds/abspann.wav.h"
#include "bin/sounds/angel.wav.h"
#include "bin/sounds/baumfaellt.wav.h"
#include "bin/sounds/brandung.wav.h"
#include "bin/sounds/crash.wav.h"
#include "bin/sounds/erfindung.wav.h"
#include "bin/sounds/faellen.wav.h"
#include "bin/sounds/feuer.wav.h"
#include "bin/sounds/fluss.wav.h"
#include "bin/sounds/hammer.wav.h"
#include "bin/sounds/klick2.wav.h"
#include "bin/sounds/klick.wav.h"
#include "bin/sounds/knistern.wav.h"
#include "bin/sounds/logo.wav.h"
#include "bin/sounds/platsch.wav.h"
#include "bin/sounds/schaufeln.wav.h"
#include "bin/sounds/schlagen.wav.h"
#include "bin/sounds/schleuder.wav.h"
#include "bin/sounds/schnarchen.wav.h"
#include "bin/sounds/schwimmen.wav.h"
#include "bin/sounds/sturm.wav.h"
#include "bin/sounds/trinken.wav.h"
#include "bin/sounds/wald.wav.h"
#include "bin/sounds/wolf.wav.h"
#include <string>
#include <unordered_map>

namespace Sound {
static std::vector<sf::SoundBuffer> sound_buffers; // Sound::COUNT - Wavedateispeicher
static std::vector<sf::Sound> sound_players; // Sound::COUNT

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

    // TODO: cleaner, just set directly in the Game.cpp
    std::unordered_map<std::string, const unsigned char*> datas = {
        { "sounds/abspann.wav",         resource_abspann_wav_data },
        { "sounds/angel.wav",   resource_angel_wav_data },
        { "sounds/baumfaellt.wav",      resource_baumfaellt_wav_data },
        { "sounds/brandung.wav",        resource_brandung_wav_data },
        { "sounds/crash.wav",   resource_crash_wav_data },
        { "sounds/erfindung.wav",       resource_erfindung_wav_data },
        { "sounds/faellen.wav",         resource_faellen_wav_data },
        { "sounds/feuer.wav",   resource_feuer_wav_data },
        { "sounds/fluss.wav",   resource_fluss_wav_data },
        { "sounds/hammer.wav",  resource_hammer_wav_data },
        { "sounds/klick2.wav",  resource_klick2_wav_data },
        { "sounds/klick.wav",   resource_klick_wav_data },
        { "sounds/knistern.wav",        resource_knistern_wav_data },
        { "sounds/logo.wav",    resource_logo_wav_data },
        { "sounds/platsch.wav",         resource_platsch_wav_data },
        { "sounds/schaufeln.wav",       resource_schaufeln_wav_data },
        { "sounds/schlagen.wav",        resource_schlagen_wav_data },
        { "sounds/schleuder.wav",       resource_schleuder_wav_data },
        { "sounds/schnarchen.wav",      resource_schnarchen_wav_data },
        { "sounds/schwimmen.wav",       resource_schwimmen_wav_data },
        { "sounds/sturm.wav",   resource_sturm_wav_data },
        { "sounds/trinken.wav",         resource_trinken_wav_data },
        { "sounds/wald.wav",    resource_wald_wav_data },
        { "sounds/wolf.wav",    resource_wolf_wav_data },
    };
    std::unordered_map<std::string, const unsigned long long> sizes = {
        { "sounds/abspann.wav",         resource_abspann_wav_size },
        { "sounds/angel.wav",   resource_angel_wav_size },
        { "sounds/baumfaellt.wav",      resource_baumfaellt_wav_size },
        { "sounds/brandung.wav",        resource_brandung_wav_size },
        { "sounds/crash.wav",   resource_crash_wav_size },
        { "sounds/erfindung.wav",       resource_erfindung_wav_size },
        { "sounds/faellen.wav",         resource_faellen_wav_size },
        { "sounds/feuer.wav",   resource_feuer_wav_size },
        { "sounds/fluss.wav",   resource_fluss_wav_size },
        { "sounds/hammer.wav",  resource_hammer_wav_size },
        { "sounds/klick2.wav",  resource_klick2_wav_size },
        { "sounds/klick.wav",   resource_klick_wav_size },
        { "sounds/knistern.wav",        resource_knistern_wav_size },
        { "sounds/logo.wav",    resource_logo_wav_size },
        { "sounds/platsch.wav",         resource_platsch_wav_size },
        { "sounds/schaufeln.wav",       resource_schaufeln_wav_size },
        { "sounds/schlagen.wav",        resource_schlagen_wav_size },
        { "sounds/schleuder.wav",       resource_schleuder_wav_size },
        { "sounds/schnarchen.wav",      resource_schnarchen_wav_size },
        { "sounds/schwimmen.wav",       resource_schwimmen_wav_size },
        { "sounds/sturm.wav",   resource_sturm_wav_size },
        { "sounds/trinken.wav",         resource_trinken_wav_size },
        { "sounds/wald.wav",    resource_wald_wav_size },
        { "sounds/wolf.wav",    resource_wolf_wav_size },
    };

    std::unordered_map<std::string, const unsigned char*>::const_iterator datait = datas.find(Wav[Sound].Filename);
    std::unordered_map<std::string, const unsigned long long>::const_iterator sizeit = sizes.find(Wav[Sound].Filename);
    if (sizeit == sizes.end() || datait == datas.end()) {
        sf::err() << "Couldn't find sound file '" << Wav[Sound].Filename << "'!" << std::endl;
        return;
    }

    if (!sound_buffers[Sound].loadFromMemory(datait->second, sizeit->second)) {
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
