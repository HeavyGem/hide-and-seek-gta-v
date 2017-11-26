#include "Functions.h"
#include "Utils.h"
#include "Invoker.h"
#include "Menu.h"
#include "Buttons.h"
#include "Colors.h"
#include "Lists.h"

#define INIT_ARRAY_16 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

void print0(char* text, int time) {
	BEGIN_TEXT_COMMAND_PRINT("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	END_TEXT_COMMAND_PRINT(time, 1);
}

bool isNumericChar(char x) { return (x >= '0' && x <= '9') ? true : false; }

int StoI(char *s)
{
	if (*s == NULL)
		return 0;

	int res = 0,
		sign = 1,
		i = 0;

	if (s[0] == '-')
	{
		sign = -1;
		i++;
	}
	for (; s[i] != '\0'; ++i)
	{
		if (isNumericChar(s[i]) == false)
			return 0;
		res = res * 10 + s[i] - '0';
	}
	return sign*res;
}

char* FtoS(float input)
{
	char returnvalue[64];
	int wholenumber = (int)input;
	input -= wholenumber;
	input *= 100;
	sprintf(returnvalue, "%d.%d", wholenumber, (int)input);
	return returnvalue;
}

char *ItoS(int number)
{
	char buf[30];
	sprintf(buf, "%i", number);
	return buf;
}

float PosP(float input) { return input / GET_SAFE_ZONE_SIZE(); }

bool* toggle_Glare(int _do) {
	static bool value = true; //when it's true, the option is enabled by default
	switch (_do) {
	case -1: {
		sprint("Animated globe %s", value ? "~g~On" : "~r~Off");
		break;
	}
	case 1: {
		ShowGlare = value ? true : false;
		break;
	}
	}
	return &value;
}

bool* FPS(int _do) {
	static bool value = true; //when it's true, the option is enabled by default
	switch (_do) {
	case -1: {
		sprint("FPS counter %s", value ? "~g~On" : "~r~Off");
		break;
	}
	case 1: {
		if (value)
		{
			char bufFPS[50];
			fps = frameCache_new - frameCache_old - 1;
			sprintf(bufFPS, "%i FPS", fps);
			if (fps <= 18) //if fps <= 18 draw the value in red
			{
				drawText(bufFPS, 0, PosP(0.21f), PosP(0.78f), 0.5, 0.5, 255, 0, 0, 255, true);
			}
			else
			{
				drawText(bufFPS, 0, PosP(0.21f), PosP(0.78f), 0.5, 0.5, 0, 255, 255, 255, true);
			}
			if (frameCache_time + 1000 < GET_GAME_TIMER()) //Timer of FPS counter for cadence
			{
				frameCache_time = GET_GAME_TIMER();
				frameCache_old = frameCache_new;
				frameCache_new = GET_FRAME_COUNT();
			}
		}
		break;
	}
	}
	return &value;
}

void drawText(char* text, int font, float x, float y, float scalex, float scaley, int r, int b, int g, int a, bool center) {
	SET_TEXT_FONT(font);
	SET_TEXT_SCALE(scalex, scaley);
	SET_TEXT_COLOUR(r, g, b, a);
	SET_TEXT_WRAP(0.0f, 1.0f);
	SET_TEXT_CENTRE(center);
	SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	SET_TEXT_OUTLINE();
	_SET_TEXT_ENTRY("STRING");
	_ADD_TEXT_COMPONENT_STRING2(text);
	_DRAW_TEXT(x, y);
}

int get_players_count() {
	int players = 0;
	for_player{
		if (strcmp(GET_PLAYER_NAME(player), "**Invalid**")) players++;
	}
	return players;
}

void teleport(Vector3 Vec) {
	if (IS_PED_IN_ANY_VEHICLE(PLAYER_PED_ID(), 0))
		SET_ENTITY_COORDS(GET_VEHICLE_PED_IS_IN(PLAYER_PED_ID(), 0), Vec.x, Vec.y, Vec.z, 0, 0, 0, 0);
	else
		SET_ENTITY_COORDS(PLAYER_PED_ID(), Vec.x, Vec.y, Vec.z, 0, 0, 0, 0);
}

bool* set_model(int _do, char* model) {
	static bool value;
	static char* _model;
	if (model != 0) _model = model;

	if (!value) return &value;
	switch (_do) {
	case -1: {
		//sprint("Model set to ~g~%s", _model);
		break;
	}
	case 1: {
		int hash = GET_HASH_KEY(_model);
		if (IS_MODEL_IN_CDIMAGE(hash) && IS_MODEL_VALID(hash)) {
			REQUEST_MODEL(hash);
			if (HAS_MODEL_LOADED(hash)) {
				SET_PLAYER_MODEL(PLAYER_ID(), hash);
				SET_MODEL_AS_NO_LONGER_NEEDED(hash);
				if (_model == "mp_m_freemode_01")
				{
					SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER_PED_ID());
				}
				if (_model == "mp_f_freemode_01")
				{
					SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER_PED_ID());
				}
				value = false;
			}
		}
		else value = false;
		break;
	}
	}
	return &value;
}

bool* toggle_godmode(int _do) {
	static bool value;
	switch (_do) {
	case -1: {
		sprint("Godmode %s", value ? "~g~On" : "~r~Off");
		break;
	}
	case 1: {
		SET_PLAYER_INVINCIBLE(PLAYER_ID(), value);
		break;
	}
	}
	return &value;
}

bool* toggle_invisibility(int _do) {
	static bool value;

	switch (_do) {
	case -1: {
		sprint("Invisible %s", value ? "~g~On" : "~r~Off");
		break;
	}
	case 1:{
		SET_ENTITY_VISIBLE(PLAYER_PED_ID(), !value, 0);
		break;
	}
	}
	return &value;
}

void explode_player(int player) {
	Vector3 Pos = GET_ENTITY_COORDS(GET_PLAYER_PED(player), false);
	ADD_EXPLOSION(Pos.x, Pos.y, Pos.z, 29, 0.5f, true, false, 5.0f);
}

bool* spawn_vehicle(int _do, char* name) {
	static bool value;
	static char* _name;
	if (name != 0) _name = name;

	if (!value) return &value;
	switch (_do) {
	case -1: {
		//sprint("Spawned ~g~%s", _name);
		break;
	}
	case 1: {
		int hash = GET_HASH_KEY(_name);
		Vector3 origin;
		if (IS_MODEL_IN_CDIMAGE(hash) && IS_MODEL_VALID(hash)) {
			REQUEST_MODEL(hash);
			if (HAS_MODEL_LOADED(hash)) {
				int ped_id = PLAYER_PED_ID();
				int old_vehicle = GET_VEHICLE_PED_IS_USING(ped_id);
				float speed = GET_ENTITY_SPEED(old_vehicle);
				if (old_vehicle) DELETE_VEHICLE(&old_vehicle);
					origin = GET_ENTITY_COORDS(ped_id, true);
				int new_vehicle = CREATE_VEHICLE(hash, origin.x, origin.y, origin.z, 0, 1, 0);
				if (new_vehicle) {
					SET_ENTITY_HEADING(new_vehicle, GET_ENTITY_HEADING(ped_id));
					SET_PED_INTO_VEHICLE(ped_id, new_vehicle, -1);
					SET_VEHICLE_ENGINE_ON(new_vehicle, 1, 0);
					SET_VEHICLE_FORWARD_SPEED(new_vehicle, speed);
					value = 0;
				}
			}
		}
		else value = 0;
		break;
	}
	}
	return &value;
}

void _DRAW_LIGHT_NEON(Entity entity, RGB Colour) //Neon light
{
	NeonCol.R = Colour.R;
	NeonCol.G = Colour.G;
	NeonCol.B = Colour.B;

	Vector3 Relative = GET_ENTITY_COORDS(entity, false);
	float ZGround = 0;
	if (GET_GROUND_Z_FOR_3D_COORD(Relative.x, Relative.y, Relative.z, &ZGround))
		Relative.z = ZGround;
	if (NeonType != 9)//Random
		DRAW_LIGHT_WITH_RANGE(Relative.x, Relative.y, Relative.z + 0.05f, NeonCol.R, NeonCol.G, NeonCol.B, 2.0f, 10.0f);
	else
		DRAW_LIGHT_WITH_RANGE(Relative.x, Relative.y, Relative.z + 0.05f, GET_RANDOM_INT_IN_RANGE(0, 255), GET_RANDOM_INT_IN_RANGE(0, 255), GET_RANDOM_INT_IN_RANGE(0, 255), 2.0f, 10.0f);
}

bool* toggle_neons(int _do) {
	static bool value;
	switch (_do) {
	case -1: {
		sprint("Neons vehicle %s", value ? "~g~On" : "~r~Off");
		break;
	}
	case 1: {
		if (value)
		{
			if (IS_PED_IN_ANY_VEHICLE(PLAYER_PED_ID(), false))
			{
				int Vehicle = GET_VEHICLE_PED_IS_IN(PLAYER_PED_ID(), false);
				Hash veh = GET_ENTITY_MODEL(Vehicle);
				if (!IS_THIS_MODEL_A_HELI(veh) && !IS_THIS_MODEL_A_PLANE(veh) && !IS_THIS_MODEL_A_TRAIN(veh) && !IS_THIS_MODEL_A_BOAT(veh))
				{
					float ZGround = 0;
					Vector3 vehCoords = GET_ENTITY_COORDS(Vehicle, false);
					if (GET_GROUND_Z_FOR_3D_COORD(vehCoords.x, vehCoords.y, vehCoords.z, &ZGround))
						_DRAW_LIGHT_NEON(Vehicle, NeonRGB[NeonType]);
				}
			}
		}
		break;
	}
	}
	return &value;
}//ok

void Keyboard(int action, char* defaultText, int maxLength)
{
	DISPLAY_ONSCREEN_KEYBOARD(0, "FMMC_KEY_TIP8", "", defaultText, "", "", "", maxLength);
	keyboardAction = action;
	keyboardActive = true;
}

bool* CheckKeyboard(int _do) {
	static bool value = true;
	switch (_do) {
	case 1: {
		KeyboardRes = GET_ONSCREEN_KEYBOARD_RESULT();
		if (keyboardActive)
		{
			if (UPDATE_ONSCREEN_KEYBOARD() == 1)
			{
				if (keyboardAction == 0) //option_i
				{
					*keyboardVar = StoI(GET_ONSCREEN_KEYBOARD_RESULT());
					keyboardActive = false;
				}
			}
		}
		break;
	}
	}
	return &value;
}

void InstructionsInit() {
	VAR_INSTRUCTIONAL_COUNT = 0;
	VAR_INSTRUCTIONAL_CONTAINER = REQUEST_SCALEFORM_MOVIE("instructional_buttons");
	DRAW_SCALEFORM_MOVIE_FULLSCREEN(VAR_INSTRUCTIONAL_CONTAINER, 255, 255, 255, 0, 0);
	_PUSH_SCALEFORM_MOVIE_FUNCTION(VAR_INSTRUCTIONAL_CONTAINER, "CLEAR_ALL");
	_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
	_PUSH_SCALEFORM_MOVIE_FUNCTION(VAR_INSTRUCTIONAL_CONTAINER, "SET_CLEAR_SPACE");
	_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(200);
	_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
}
void InstructionsAdd(char *text, int button) {
	_PUSH_SCALEFORM_MOVIE_FUNCTION(VAR_INSTRUCTIONAL_CONTAINER, "SET_DATA_SLOT");
	_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(VAR_INSTRUCTIONAL_COUNT);
	_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(button);
	_BEGIN_TEXT_COMPONENT("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	_END_TEXT_COMPONENT();
	_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
	VAR_INSTRUCTIONAL_COUNT++;
}
void InstructionsEnd() {
	_PUSH_SCALEFORM_MOVIE_FUNCTION(VAR_INSTRUCTIONAL_CONTAINER, "DRAW_INSTRUCTIONAL_BUTTONS");
	_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
	_PUSH_SCALEFORM_MOVIE_FUNCTION(VAR_INSTRUCTIONAL_CONTAINER, "SET_BACKGROUND_COLOUR");
	_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
	_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
	_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
	_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(80);
	_POP_SCALEFORM_MOVIE_FUNCTION_VOID();
}
void draw_instructions() {
	InstructionsInit();
	InstructionsAdd((char*)(Menu.current_menu ? "Back" : "Close"), 31); //BUTTON_O
	InstructionsAdd("Scroll", 10); //BUTTON_DPAD_UP_DOWN
	switch (Menu.c_option_t) //Instructions buttons
	{
		default: case 0: InstructionsAdd("Select", 30); break;//BUTTON_A (options normal)
		case 1: InstructionsAdd("Toggle", 30); break;//BUTTON_A (options bool)
		case 2: InstructionsAdd("Change Value", 11); break;//BUTTON_DPAD_LEFT_RIGHT (float, int or CharSwap)
		case 3: InstructionsAdd("Keyboard", 32); break;//BUTTON_SQUARE (keyboard)
		case 4: InstructionsAdd("Change Value", 11), InstructionsAdd("Toggle", 30); break;//BUTTON_DPAD_LEFT_RIGHT + X (SwapCharBool)
	}
	InstructionsEnd();
}