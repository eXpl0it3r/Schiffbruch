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
