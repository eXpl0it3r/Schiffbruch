#include "Sound.hpp"



#include "Direct.hpp"

namespace Sound
{
	void LoadSound(short Sound)
	{
		HMMIO 			DateiHandle;	// Handle der geöffneten Datei
		MMCKINFO		parent,			// DateiInfos
						child;			// DateiInfos
		WAVEFORMATEX    wfmtx;			// wave format structure

		UCHAR	*snd_buffer,			// SoundZwischenspeicher
				*audio_ptr_1 = nullptr,	// Pointer1
				*audio_ptr_2 = nullptr;	// Pointer2

		DWORD	audio_length_1 = 0,	// Länge des 1.Buffer
				audio_length_2 = 0;	// Länge des 2.Buffer

		if (Soundzustand == -1) return; // Wenn keine Soundkarte vorhanden raus...	

		// Dateiinfo initialisieren
		parent.ckid = (FOURCC)0;
		parent.cksize = 0;
		parent.fccType = (FOURCC)0;
		parent.dwDataOffset = 0;
		parent.dwFlags = 0;
		child = parent;

		// Datei öffnen
		DateiHandle = mmioOpen(Wav[Sound].Dateiname, nullptr, MMIO_READ | MMIO_ALLOCBUF);

		// Dateiinfos auslesen 
		parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(DateiHandle, &parent, nullptr, MMIO_FINDRIFF);
		child.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(DateiHandle, &child, &parent, 0);
		mmioRead(DateiHandle, (char *)&wfmtx, sizeof(wfmtx));
		mmioAscend(DateiHandle, &child, 0);
		child.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(DateiHandle, &child, &parent, MMIO_FINDCHUNK);

		// Speicher für die Zwischenspeicherung besorgen
		snd_buffer = (UCHAR *)malloc(child.cksize);

		// Die Wav-Daten rüberkopieren
		mmioRead(DateiHandle, (char *)snd_buffer, child.cksize);

		// Die Datei schließen
		mmioClose(DateiHandle, 0);

		// Den Buffer machen
		dsbdesc.dwSize = sizeof(DSBUFFERDESC);
		dsbdesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLVOLUME; // Wenn das mixen nicht funktioniert,dass hier reaktivieren
		dsbdesc.dwBufferBytes = child.cksize;
		dsbdesc.lpwfxFormat = &wfmtx;
		lpds->CreateSoundBuffer(&dsbdesc, &lpdsbWav[Sound], nullptr);

		// Buffer locken und Daten reinkopieren und unlocken
		lpdsbWav[Sound]->Lock(0, child.cksize,
			(void **)&audio_ptr_1,
			&audio_length_1,
			(void **)&audio_ptr_2,
			&audio_length_2,
			DSBLOCK_FROMWRITECURSOR);

		memcpy(audio_ptr_1, snd_buffer, audio_length_1);
		memcpy(audio_ptr_2, (snd_buffer + audio_length_1), audio_length_2);
		lpdsbWav[Sound]->Unlock(audio_ptr_1, audio_length_1, audio_ptr_2, audio_length_2);

		// Den ZwischenSpeicherfreigeben
		free(snd_buffer);

		// Die Standardlautstärke festlegen
		lpdsbWav[Sound]->SetVolume((long)(-10000 + 100 * Wav[Sound].Volume));
	}

	void PlaySound(short Sound, short Volume)
	{
		short z;

		if ((Sound == 0) || (Soundzustand <= 0)) return;

		z = -10000 + 100 * Wav[Sound].Volume;
		lpdsbWav[Sound]->SetVolume((long)(-10000 + (10000 + z)*Volume / 100));
		if (Wav[Sound].Loop) lpdsbWav[Sound]->Play(NULL, NULL, DSBPLAY_LOOPING);
		else lpdsbWav[Sound]->Play(NULL, NULL, NULL);
	}

	void StopSound(short Sound)
	{
		if ((Sound == 0) || (Soundzustand <= 0)) return;
		lpdsbWav[Sound]->Stop();
	}

} // namespace Sound