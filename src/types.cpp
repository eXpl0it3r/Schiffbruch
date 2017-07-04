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
