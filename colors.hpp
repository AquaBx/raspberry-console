#pragma once

#include <cstdint>

enum class color : uint16_t {
    whitesmoke   = 0xF7BE, // gris
	dim_grey     = 0x6B4D, // gris foncé

	yellow       = 0xFFE0,
	yellow_green = 0x9E66,
	ivory        = 0xFFFE, // jaune pale

	alice_blue   = 0xF7DF, // bleu
	aqua         = 0xF7DF, // bleu clair (plus clair que alice_blue)
	azure        = 0xF7FF,
	dark_blue    = 0x0011, // bleu foncé en effet, sans doute trop foncé pour le voir correctement

	chartreuse   = 0x7FE0, // vert clair
	green_yellow = 0xAFE5, // vert clair pale
	forest_green = 0x2444, // vert foncé
	dark_green   = 0x03E0, // vert foncé, sans doute trop foncé pour le voir correctemement
	dark_turquoise = 0x067A,

	chocolate    = 0xD343, // marron plus clair que brown
	dark_salmon  = 0xECAF, // marron clair

	cornsilk     = 0xFFD8, // rouge pale
	dark_orange  = 0xFC60, // tire vers le rouge
	gold         = 0xFEA0, // orange

	crimson      = 0xD8A7, // violet foncé

	floral_white = 0xFFDE, // rose pale
	fuchsia      = 0xF81F, // un genre de magenta
	ghost_white  = 0xFFDF, // magenta pale

	black        = 0x0000,
	white        = 0xFFFF,

	green        = 0x07E0,
	blue         = 0x001F,

	cyan         = 0x07FF,
	magenta      = 0xF81F,

	violet       = 0x895C, // violet foncé
	brown        = 0xA145,
};
