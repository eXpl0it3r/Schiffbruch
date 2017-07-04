#include "types.hpp"

void to_json(json& j, const RECT& rect)
{
	j = json{
		{"left", rect.left},
		{"top", rect.top},
		{"right", rect.right},
		{"bottom", rect.bottom}
	};
}

void from_json(const json& j, RECT& rect)
{
	rect.left = j.at("left").get<LONG>();
	rect.top = j.at("top").get<LONG>();
	rect.right = j.at("right").get<LONG>();
	rect.bottom = j.at("bottom").get<LONG>();
}

void to_json(json & j, const ZWEID & zweid)
{
	j = json{
		{"x", zweid.x},
		{"y", zweid.y}
	};
}

void from_json(const json & j, ZWEID & zweid)
{
	zweid.x = j.at("x").get<short>();
	zweid.y = j.at("y").get<short>();
}

void to_json(json & j, const GUY & guy)
{
	j = json{
		{"Aktiv", guy.Aktiv},
		{"Aktion", guy.Aktion},
		{"Pos", guy.Pos},
		{"PosAlt", guy.PosAlt},
		{"PosScreen", guy.PosScreen},
		{"Zustand", guy.Zustand},
		{"AkNummer", guy.AkNummer}
	};

	j["Resource"] = {};
	for (size_t i = 0; i < 3; i++)
	{
		j["Resource"][i] = guy.Resource[i];
	}

	j["Inventar"] = {};
	for (size_t i = 0; i < BILDANZ; i++)
	{
		j["Inventar"][i] = guy.Inventar[i];
	}
}

void from_json(const json & j, GUY & guy)
{
	guy.Aktiv = j.at("Aktiv").get<bool>();
	guy.Aktion = j.at("Aktion").get<short>();
	guy.Pos = j.at("Pos").get<ZWEID>();
	guy.PosAlt = j.at("PosAlt").get<ZWEID>();
	guy.PosScreen = j.at("PosScreen").get<ZWEID>();
	guy.Zustand = j.at("Zustand").get<short>();
	guy.AkNummer = j.at("AkNummer").get<short>();
	json resources = j.at("Resource");
	for (size_t i = 0; i < 3; i++)
	{
		guy.Resource[i] = resources[i].get<float>();
	}
	json inventory = j.at("Inventar");
	for (size_t i = 0; i < BILDANZ; i++)
	{
		guy.Inventar[i] = inventory[i].get<short>();
	}
}

void to_json(json & j, const BMP & bmp)
{
	//NOTE: this ignores the "Surface" property!
	j = json{
		{"Animation", bmp.Animation},
		{"Anzahl", bmp.Anzahl},
		{"Phase", bmp.Phase},
		{"rcSrc", bmp.rcSrc},
		{"rcDes", bmp.rcDes},
		{"Breite", bmp.Breite},
		{"Hoehe", bmp.Hoehe},
		{"Geschwindigkeit", bmp.Geschwindigkeit},
		{"Sound", bmp.Sound},
		{"AkAnzahl", bmp.AkAnzahl},
		{"First", bmp.First}
	};

	j["Rohstoff"] = {};
	for (size_t i = 0; i < BILDANZ; i++)
	{
		j["Rohstoff"][i] = bmp.Rohstoff[i];
	}
}

void from_json(const json & j, BMP & bmp)
{
	//NOTE: this ignores the "Surface" property!
	bmp.Animation = j.at("Animation").get<bool>();
	bmp.Anzahl = j.at("Anzahl").get<short>();
	bmp.Phase = j.at("Phase").get<short>();
	bmp.rcSrc = j.at("rcSrc").get<RECT>();
	bmp.rcDes = j.at("rcDes").get<RECT>();
	bmp.Breite = j.at("Breite").get<short>();
	bmp.Hoehe = j.at("Hoehe").get<short>();
	bmp.Geschwindigkeit = j.at("Geschwindigkeit").get<short>();
	bmp.Sound = j.at("Sound").get<short>();
	json resources = j.at("Rohstoff");
	for (size_t i = 0; i < BILDANZ; i++)
	{
		bmp.Rohstoff[i] = resources[i].get<short>();
	}
	bmp.AkAnzahl = j.at("AkAnzahl").get<short>();
	bmp.First = j.at("First").get<bool>();
}

void to_json(json & j, const WAV & wav)
{
	j = json{
		{"Dateiname", std::string(wav.Dateiname)},
		{"Loop", wav.Loop},
		{"Volume", wav.Volume}
	};
}

void from_json(const json & j, WAV & wav)
{
	//get the filename
	std::string filename = j.at("Dateiname").get<std::string>();
	//FIXME: it's kind of impossible to prevent a memory leak here at some point
	//because I can't know if the reference is to a statically or dynamically allocated string
	wav.Dateiname = new char[filename.length() + sizeof("")];
	std::string::iterator iterator;
	size_t index;
	for (index = 0, iterator = filename.begin(); iterator < filename.end(); iterator++, index++)
	{
		wav.Dateiname[index] = *iterator;
	}

	wav.Loop = j.at("Loop").get<bool>();
	wav.Volume = j.at("Volume").get<bool>();
}
