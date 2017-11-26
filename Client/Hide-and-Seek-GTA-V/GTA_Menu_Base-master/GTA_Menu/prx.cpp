#include "Includes.h"
#include "Utils.h"
#include "Invoker.h"
#include "Huds.h"
#include "Buttons.h"
#include "Menu.h"
#include "Colors.h"
#include "Functions.h"
#include "tErrors.h"
#include "Dialog.h"

int IS_PLAYER_ONLINE() {
	char* script = GET_THIS_SCRIPT_NAME();
	if (!strcmp(script, "ingamehud")) {
		/* Hud stuff goes here */
		Menu.draw_menu();
		Menu.run_menu();
		update_buttons();
	}
	if(!strcmp(script, "freemode") || !NETWORK_IS_IN_SESSION()) {
		/* Menu calls go in here, unless they draw huds then they go above */
		toggle_Glare(true);
		FPS(true);
		CheckKeyboard(true);
		set_model(true);
		toggle_godmode(true);
		toggle_invisibility(true);
		spawn_vehicle(true);
		toggle_neons(true);

		if (Menu.is_open) { toggle_buttons(false); } else { toggle_buttons(true); }
	}

	return 1;
}

void thread_a(uint64_t) {
	init_colors();
	Menu.init();
	

	char sleep_time = 50;
	while (true) {
		if (!Menu.is_open) {
			DPAD_LEFT_RIGHT = false, DPAD_UP_DOWN = false, BUTTON_L1_R1 = false;
			Menu.is_open = BUTTON_R1_HELD && DPAD_RIGHT_HELD; //Opens buttons
			Menu.submenuLevel = 0;
			BUTTON_O = false;
		}
		else {
			if (BUTTON_X) {
				Menu.call_function = true;
				sleep(sleep_time);
			}
			if (DPAD_UP_DOWN) {
				Menu.call_function = false;
				Menu.current_index[Menu.current_menu] += DPAD_UP ? -1 : 1;
				wrap_num(&Menu.current_index[Menu.current_menu], (char)0, (char)(Menu.option_count - 1));
				if (fps <= 18) {
					sleep(125); //Timer Up/Down when fps <= 18
				}
				else {
					sleep(sleep_time); //Timer Up/Down when fps => 19
				}
			}
			if (BUTTON_O) {
				if (Menu.current_menu == 0) Menu.is_open = false;
				else {
					//Menu.change_menu(Menu.parent_menu[Menu.current_menu], true); //Old return system (Bug when there are few submenus)
					Menu.current_menu = Menu.previous_menu[Menu.submenuLevel - 1]; //New return system (Do not bug)
					Menu.submenuLevel--;
					help_text = NULL;
					sleep(sleep_time);
				}
			}
		}
		sleep(10);
	}
}

RUN_MODULE(Menu Base, main);
int main() {
	new_thread(thread_a);
	PatchInJump(0x3E3A20, (int)IS_PLAYER_ONLINE);
	/*Simple message to launch that closes by pressing X*/
	//Dialog::msgdialog_mode = Dialog::MODE_STRING_OK;
	//Dialog::Show("Menu Base John v3\n Remaked by Dylan57richem\n  To open the menu: R1 + >");

	/*Progress bar static to launch that closes after x seconds (x = 13 actually)*/
	Dialog::msgdialog_mode = Dialog::MODE_RUNNING;
	Dialog::ProgressBar("Extra games menu by BFoGvDo & Mathiis95 : R1 + > to open", "Hide & seek is our favorite !", 13);
	Dialog::End();
	return 0;
}