#include "Colors.h"

color_s black, white, red, green, blue, cyan, yellow, pink, title_color, options_color, background_color, shader_color, pointer_color;
void init_colors() {
	black = color_s(0, 0, 0);
	white = color_s(255, 255, 255);
	red = color_s(255, 0, 0);
	green = color_s(0, 255, 0);
	blue = color_s(0, 0, 255);
	cyan = color_s(0, 255, 255);
	yellow = color_s(255, 255, 0);
	pink = color_s(255, 0, 255);
	title_color = color_s(0, 255, 255, 255);
	options_color = color_s(255, 255, 255, 255);
	background_color = color_s(0, 0, 0, 140);
	shader_color = color_s(0, 0, 0, 200);
	pointer_color = color_s(0, 255, 255, 255);
}