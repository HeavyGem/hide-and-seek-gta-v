#include "Invoker.h"

#pragma once


// bool

bool keyboardActive = false;
bool ShowGlare = true;

/*----------------------------------------------------------------------------------------------------------------------*/

// int

int VAR_INSTRUCTIONAL_COUNT, VAR_INSTRUCTIONAL_CONTAINER;
int fps;
int frameCache_new;
int frameCache_old;
int frameCache_time;
int keyboardAction;
int *keyboardVar = 0;
int NeonType;
int MinimapType;
int sprintf_opd[2] = { 0x14FDCEC, TOC }; //1.27
int(*__sprintf)(char * s, size_t n, const char * format, ...) = (int(*)(char*, size_t, const char*, ...))&sprintf_opd;

/*----------------------------------------------------------------------------------------------------------------------*/

// char

char * KeyboardRes;
char* KeyboardPlate = NULL;
char print_buf[0x50];

/*----------------------------------------------------------------------------------------------------------------------*/

// Hash & Others

RGB NeonCol = { 255, 0, 0 };

/*----------------------------------------------------------------------------------------------------------------------*/

// Lists

char* NeonMenu[] = { "~r~Red", "~g~Green", "~b~Blue", "~y~~bold~Yellow", "~o~Orange", "~p~~bold~Purple", "~f~Turquoise", "~p~Pink", "~w~White", "R~b~an~y~do~w~m" };
RGB NeonRGB[] = { { 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }, { 255, 255, 0 }, { 237, 127, 16 }, { 102, 0, 153 }, { 0, 255, 255 }, { 255, 0, 255 }, { 255, 255, 255 }, { 1, 2, 3 } };


unsigned char MapNormal = 0x00, MapBlack = 0x40, MapRed = 0x14, MapYellow = 0x16, MapGreen = 0x15, MapCyan = 0x19, MapBlue = 0x17, MapPink = 0x18;
unsigned char* MinimapColor[] = { &MapNormal, &MapBlack, &MapRed, &MapYellow, &MapGreen, &MapCyan, &MapBlue, &MapPink };
char* MinimapMenu[] = { "Normal", "Black", "Red", "Yellow", "Green", "Cyan", "Blue", "Pink" };