#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "Includes.h"
#include "Utils.h"

extern int *keyboardVar;
extern bool ShowGlare;
extern int fps;
extern char* NeonMenu[];
extern int NeonType;
extern unsigned char* MinimapColor[];
extern char* MinimapMenu[];
extern int MinimapType;

void drawText(char * text, int font, float x, float y, float scalex, float scaley, int r, int b, int g, int a, bool center);
char* ItoS(int num);
char* FtoS(float input);
bool* toggle_Glare(int _do = 0);
bool* FPS(int _do = 0);
void Keyboard(int action = 0, char* defaultText = 0, int maxLength = 0);
bool* CheckKeyboard(int _do = 0);


bool* set_model(int _do = 0, char* model = 0);
bool* toggle_godmode(int _do = 0);
bool* toggle_invisibility(int _do = 0);
void explode_player(int player);
void teleport(Vector3 Vec);
bool* spawn_vehicle(int _do = 0, char* name = 0);
bool* toggle_neons(int _do = 0);

int get_players_count();

void draw_instructions();
void print0(char* text, int time);

extern int sprintf_opd[2];
extern int(*__sprintf)(char * s, size_t n, const char * format, ...);

extern char print_buf[0x50];
#define sprint(fmt, ...) \
	__sprintf(print_buf, sizeof(print_buf), fmt, __VA_ARGS__); \
	print0(print_buf, 750);

#define for_player for(int player = 0; player < 16; player++)

#endif