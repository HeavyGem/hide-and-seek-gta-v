#include "Huds.h"
#include "Invoker.h"

void draw_text(char* text, int font, float size, float x, float y, int r, int g, int b, int a, ALIGN_V align, int layer) {
	switch (align) {
		case center: SET_TEXT_CENTRE(true); break;
		case right:
			SET_TEXT_RIGHT_JUSTIFY(true); 
			SET_TEXT_WRAP(0, x);
			x = 0;
			break;
		default:
			break;
	}
	SET_TEXT_FONT(font);
	SET_TEXT_SCALE(size, size);
	SET_TEXT_COLOUR(r, g, b, a);
	SET_TEXT_OUTLINE();
	_SET_TEXT_ENTRY("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	_SET_2D_LAYER(layer);
	_DRAW_TEXT(x, y);
}
void draw_rect(float x, float y, float width, float height, int r, int g, int b, int a, int layer) {
	_SET_2D_LAYER(layer);
	DRAW_RECT(x, y, width, height, r, g, b, a);
}
void draw_sprite(char* textureDict, char* textureName, float x, float y, float width, float height, int r, int g, int b, int a, float rotation, int layer)
{
	//_SET_2D_LAYER(layer); //not sure
	REQUEST_STREAMED_TEXTURE_DICT(textureDict, false);
	if (HAS_STREAMED_TEXTURE_DICT_LOADED(textureDict))
	{
		_SET_2D_LAYER(layer);
		DRAW_SPRITE(textureDict, textureName, x, y, width, height, rotation, r, g, b, a);
	}
}
