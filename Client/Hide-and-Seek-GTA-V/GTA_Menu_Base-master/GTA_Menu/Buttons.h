#ifndef BUTTONS_H
#define BUTTONS_H

extern bool BUTTON_TRIANGLE, BUTTON_X, BUTTON_SQUARE, BUTTON_O, BUTTON_L1, BUTTON_L2, BUTTON_L3, BUTTON_R1, BUTTON_R2, BUTTON_R3;
extern bool DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT, BUTTON_L1_R1, DPAD_UP_DOWN, DPAD_LEFT_RIGHT, DPAD_LEFT_RIGHT_HELD;
extern bool BUTTON_L1_HELD, BUTTON_L2_HELD, BUTTON_L3_HELD, BUTTON_R1_HELD, BUTTON_R2_HELD, BUTTON_R3_HELD;
extern bool DPAD_LEFT_HELD, DPAD_RIGHT_HELD, DPAD_UP_HELD, DPAD_DOWN_HELD, BUTTON_TRIANGLE_HELD, BUTTON_X_HELD, BUTTON_SQUARE_HELD, BUTTON_O_HELD;
void update_buttons();
bool button_held(int button);
void toggle_buttons(bool state);
void disable_control(int button);

#endif