#include "Invoker.h"
#include "Buttons.h"

bool button_pressed(int button) {
	return IS_DISABLED_CONTROL_JUST_PRESSED(0, button);
}
bool button_held(int button) {
	return IS_DISABLED_CONTROL_PRESSED(0, button);
}
void disable_control(int button) {
	SET_INPUT_EXCLUSIVE(0, button);
	DISABLE_CONTROL_ACTION(0, button, true);
}

bool BUTTON_TRIANGLE, BUTTON_X, BUTTON_SQUARE, BUTTON_O, BUTTON_L1, BUTTON_L2, BUTTON_L3, BUTTON_R1, BUTTON_R2, BUTTON_R3;
bool DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT, BUTTON_L1_R1, DPAD_UP_DOWN, DPAD_LEFT_RIGHT, DPAD_LEFT_RIGHT_HELD;
bool BUTTON_L1_HELD, BUTTON_L2_HELD, BUTTON_L3_HELD, BUTTON_R1_HELD, BUTTON_R2_HELD, BUTTON_R3_HELD;
bool DPAD_LEFT_HELD, DPAD_RIGHT_HELD, DPAD_UP_HELD, DPAD_DOWN_HELD, BUTTON_TRIANGLE_HELD, BUTTON_X_HELD, BUTTON_SQUARE_HELD, BUTTON_O_HELD;

void update_buttons() {
	BUTTON_TRIANGLE = button_pressed(0xC0);
	BUTTON_X = button_pressed(0xC1); // X = A
	BUTTON_SQUARE = button_pressed(0xC2); // Square = X
	BUTTON_O = button_pressed(0xC3);
	BUTTON_L1 = button_pressed(0xC4);
	BUTTON_L2 = button_pressed(0xC6);
	BUTTON_L3 = button_pressed(0xC8);
	BUTTON_R1 = button_pressed(0xC5);
	BUTTON_R2 = button_pressed(0xC7);
	BUTTON_R3 = button_pressed(0xC9);
	BUTTON_L1_R1 = BUTTON_L1 | BUTTON_R1;
	DPAD_UP = button_pressed(0xCA);
	DPAD_DOWN = button_pressed(0xCB);
	DPAD_LEFT = button_pressed(0xCC);
	DPAD_RIGHT = button_pressed(0xCD);
	DPAD_LEFT_RIGHT = DPAD_LEFT | DPAD_RIGHT;
	DPAD_UP_DOWN = DPAD_UP | DPAD_DOWN;

	//Buttons held:

	BUTTON_L1_HELD = button_held(0xC4);
	BUTTON_L2_HELD = button_held(0xC6);
	BUTTON_L3_HELD = button_held(0xC8);
	BUTTON_R1_HELD = button_held(0xC5);
	BUTTON_R2_HELD = button_held(0xC7);
	BUTTON_R3_HELD = button_held(0xC9);
	DPAD_LEFT_HELD = button_held(0xCC);
	DPAD_RIGHT_HELD = button_held(0xCD);
	DPAD_UP_HELD = button_held(0xCA);
	DPAD_DOWN_HELD = button_held(0xCB);
	BUTTON_TRIANGLE_HELD = button_held(0xC0);
	BUTTON_X_HELD = button_held(0xC1);
	BUTTON_SQUARE_HELD = button_held(0xC2);
	BUTTON_O_HELD = button_held(0xC3);

	DPAD_LEFT_RIGHT_HELD = DPAD_LEFT_HELD | DPAD_RIGHT_HELD;
}
void toggle_buttons(bool state) {
	if (!state)
	{
		HIDE_HELP_TEXT_THIS_FRAME();
		SET_CINEMATIC_BUTTON_ACTIVE(0);
		HIDE_HUD_COMPONENT_THIS_FRAME(10);
		HIDE_HUD_COMPONENT_THIS_FRAME(6);
		HIDE_HUD_COMPONENT_THIS_FRAME(7);
		HIDE_HUD_COMPONENT_THIS_FRAME(9);
		HIDE_HUD_COMPONENT_THIS_FRAME(8);
		SET_INPUT_EXCLUSIVE(2, 166);
		SET_INPUT_EXCLUSIVE(2, 167);
		SET_INPUT_EXCLUSIVE(2, 177);
		SET_INPUT_EXCLUSIVE(2, 178);
		SET_INPUT_EXCLUSIVE(2, 193);
		SET_INPUT_EXCLUSIVE(2, 194);
		SET_INPUT_EXCLUSIVE(2, 195);
		SET_INPUT_EXCLUSIVE(2, 202);
		SET_INPUT_EXCLUSIVE(2, 203);
		SET_INPUT_EXCLUSIVE(2, 204);
		SET_INPUT_EXCLUSIVE(2, 205);
		disable_control(0xCA);//UP
		disable_control(0xCB);//DOWN
		disable_control(0xCC);//LEFT
		disable_control(0xCD);//RIGHT
		disable_control(0xC3);//O
	}
}