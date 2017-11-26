extern char* help_text;
#ifndef MENU_H
#define MENU_H

#define MAX_MENUS 100 //Maximum number of submenu if COUNT_MENU used
#define MAX_PP 21 //Maximum number of options displayed in a sub-menu before scrolling

#define MENU_COUNT __COUNTER__
#define COUNT_MENU = __COUNTER__,

enum menus {
	Main_Menu,
	Players_List, Players_Options,
	Sub_1,
	Spawn_Vehicle,
	Super_Car, Sportive_Car, SportiveC_Car, Muscle_Car, Sedans_Car, Coupes_Car, Compact_Car, SUVTrucks_Car, OFFRoad_Car, Vans_Car, 
	Emergency_Car, Service_Car, Commercial_Car, Military_Car, Moto_Car, Helicopters_Car, Bicycles, Planes_Car, Boats_Car, Snow_Car,
	Settings_Menu, Settings_Shader, Settings_Background,
	Credits_Menu,
};

struct menu {
	char current_index[MAX_MENUS], parent_menu[MAX_MENUS], option_count;
	char previous_menu[MAX_MENUS];
	int submenuLevel;
	char c_option_index, current_menu, c_drawing_index, c_option_t, selected_player;
	bool call_function, is_open, changing_menu;
	char* title, *sub_title;
	void init();
	void change_menu(char index, bool s = false);
	void draw_menu();
	void run_menu();
};

extern struct menu Menu;

#endif // !MENU_H
