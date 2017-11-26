#include "Utils.h"
#include "Buttons.h"
#include "Colors.h"
#include "Huds.h"
#include "Menu.h"
#include "Functions.h"
#include "Invoker.h"

struct menu Menu;

float x_axis = .84, y_axis = .06;
char drawing_index = 0;


#pragma region Declarations

char* help_text = NULL;

int FontShader = 1;
int FontBackground = 4;

#pragma endregion


bool can_draw_item() {
	return
		(drawing_index < MAX_PP) &&
		((Menu.current_index[Menu.current_menu] - Menu.c_option_index < (MAX_PP / 2) + 1) ||
		(Menu.c_option_index - drawing_index > abs(MAX_PP - Menu.option_count + 1)) ||
		((Menu.current_index[Menu.current_menu] - Menu.c_option_index < MAX_PP) && Menu.option_count <= MAX_PP));
}
bool is_current_option() {
	return Menu.current_index[Menu.current_menu] == Menu.c_option_index;
}
void draw_arrows() {
	float difference = ((Menu.c_option_index > MAX_PP ? MAX_PP : Menu.c_option_index) * (.035 * 10)) / 10;
	if (Menu.option_count > MAX_PP) {
		if (Menu.current_index[Menu.current_menu] > MAX_PP / 2)
			draw_sprite("helicopterhud", "hudarrow", x_axis, y_axis + difference + .059, .025, .010, 255, 255, 255, 255, 0, 1);
		if (Menu.option_count - Menu.current_index[Menu.current_menu] > MAX_PP / 2 + 1)
			draw_sprite("helicopterhud", "hudarrow", x_axis, y_axis + difference + .089, .025, .010, 255, 255, 255, 255, 180, 1);
	}
}

char* JoinStrings(char* One, char* Two, char* Three)
{
	char buffer[300];
	sprintf(buffer, "%s%s%s", One, Two, Three);
	return buffer;
}

void draw_option(char* text) {
	if (can_draw_item()) {
		bool selected = Menu.current_index[Menu.current_menu] - Menu.c_option_index == 0;
		if (selected) Menu.c_drawing_index = drawing_index;
		draw_text(text, FontBackground, .565, x_axis - .1 + (selected ? .012 : 0), y_axis + .052 + (.035 * drawing_index), options_color.r, options_color.g, options_color.b, options_color.a);
		drawing_index++;
	}
	Menu.c_option_index++;
}
void draw_float(float value, float adjustX, float adjustY) {
	if (can_draw_item()) {
		drawText(JoinStrings("< ", FtoS(value), " >"), 0, x_axis + adjustX, y_axis + adjustY + (.035 * drawing_index), .465, .465, 255, 255, 255, 255, center);
	}
}
void draw_int(char* value) {
	if (can_draw_item()) {
		char buf[35];
		sprintf(buf, "< %s >", value);
		draw_text(buf, 0, .465, x_axis + .074, y_axis + .052 + (.035 * drawing_index), 255, 255, 255, 255, center);
	}
}

void draw_valChar(int value, int max, float adjust) {
	if (can_draw_item()) {
		char buf[35];
		sprintf(buf, "(%i/%i)", value, max);
		draw_text(buf, 0, .45, x_axis + adjust, y_axis + .054 + (.035 * drawing_index), 255, 255, 255, 255, center);
	}
}

void draw_bool(bool value) {
	if (can_draw_item()) {
		char* n = (char*)(value ? "shop_box_tick" : "shop_box_cross");
		draw_sprite("commonmenu", n, x_axis + 0.088, y_axis + 0.073 + (.035 * drawing_index), 0.032, 0.056, (value ? 0 : 255), (value ? 255 : 0), (value ? 0 : 0), 150, 0, 1);
	}
}

#define add_option_c(text, function, ...) \
	if(is_current_option()) { Menu.c_option_t = 0; help_text = NULL; pointer_color = color_s(0, 255, 255, 255); if(Menu.call_function) { function(__VA_ARGS__); Menu.call_function = false; } } \
	draw_option(text);

#define add_option_bc_arg(text, show, x, v, ...) \
	if(is_current_option()) { \
		Menu.c_option_t = show; \
		help_text = NULL; \
		pointer_color = color_s(0, 255, 255, 255); \
		if(Menu.call_function) { \
			*(x(0, __VA_ARGS__)) = v; \
			(x(-1, __VA_ARGS__)); \
			Menu.call_function = false; \
		} \
	} \
	if(show) draw_bool(*(x(0, __VA_ARGS__))); \
	draw_option(text);


void add_option_bc(char* text, bool show, bool*(*x)(int), bool v, char* help = NULL) {
	if (is_current_option()) { 
		Menu.c_option_t = show;
		help_text = help;
		pointer_color = color_s(0, 255, 255, 255);
		if (Menu.call_function) { 
			*(x(0)) = v; 
			(x(-1));
			Menu.call_function = false;
		} 
	}
	if (show) draw_bool(*(x(0)));
	draw_option(text);
}

void add_option_n(char* text, char* help = NULL) {
	if (is_current_option())
	{
		pointer_color = color_s(0, 255, 255, 255);
		help_text = help;
		if (Menu.call_function) { Menu.call_function = false; }
	}
	draw_option(text);
}

void add_option_i(char* text, int* ptr, int min, int max, bool holdPress = false, bool keyBoard = false, char* help = NULL)
{
	if (is_current_option())
	{
		if (holdPress)
		{
			if (DPAD_LEFT_HELD) 
			{
				if (*ptr <= min)
					*ptr = max;
				else
					*ptr = *ptr - 1;
			}
			if (DPAD_RIGHT_HELD)
			{
				if (*ptr >= max)
					*ptr = min;
				else
					*ptr = *ptr + 1;
			}
			Menu.c_option_t = 2;
		}
		else
		{
			if (DPAD_LEFT)
			{
				if (*ptr <= min)
					*ptr = max;
				else
					*ptr = *ptr - 1;
			}
			if (DPAD_RIGHT)
			{
				if (*ptr >= max)
					*ptr = min;
				else
					*ptr = *ptr + 1;
			}
			Menu.c_option_t = 2;
		}
		if (keyBoard)
		{
			Menu.c_option_t = 3;
			if (BUTTON_SQUARE)
			{
				Keyboard(0, "", (strlen(ItoS(max)) + 1));
				keyboardVar = ptr;
			}
		}
		if (Menu.call_function) { Menu.call_function = false; }
		pointer_color = color_s(0, 255, 255, 255);
		help_text = help;
	}
	char buf[50];
	sprintf(buf, "%i", *ptr);
	draw_int(buf);
	draw_option(text);
}

void add_option_f(char* text, float *var, float min, float max, float ValueFloat = 0.1, bool holdPress = false, char* help = NULL)
{
	if (is_current_option())
	{
		if (holdPress)
		{
			if (DPAD_LEFT_HELD)
			{
				if (*var <= min)
					*var = max;
				else
					*var = *var - ValueFloat;
			}
			if (DPAD_RIGHT_HELD)
			{
				if (*var >= max)
					*var = min;
				else
					*var = *var + ValueFloat;
			}
		}
		else
		{
			if (DPAD_LEFT)
			{
				if (*var <= min)
					*var = max;
				else
					*var = *var - ValueFloat;
			}
			if (DPAD_RIGHT)
			{
				if (*var >= max)
					*var = min;
				else
					*var = *var + ValueFloat;
			}
		}
		if (Menu.call_function) { Menu.call_function = false; }
		pointer_color = color_s(0, 255, 255, 255);
		Menu.c_option_t = 2;
		help_text = help;
	}
	draw_float(*var, .074, .051);
	draw_option(text);
}

void SwapCharBool(char *text, bool*(*x)(int), bool v, char **From, int* var, int min, int max, float adjustText = .077, char* help = NULL)
{
	char buf[70];
	sprintf(buf, "%s: ~italic~%s", text, From[*var]);
	if (is_current_option())
	{
		if (DPAD_LEFT)
		{
			if (*var <= min)
				*var = max;
			else
				*var = *var - 1;
		}
		if (DPAD_RIGHT)
		{
			if (*var >= max)
				*var = min;
			else
				*var = *var + 1;
		}
		if (Menu.call_function)
		{
			*(x(0)) = v;
			(x(-1));
			Menu.call_function = false;
		}
		pointer_color = color_s(255, 0, 0, 255);
		Menu.c_option_t = 4;
		help_text = help;
	}
	draw_valChar(*var + 1, max + 1, adjustText); //Because a list ranges from 0 to max value generally, but the draw vars need + 1
	draw_bool(*(x(0)));
	draw_option(buf);
} // ok with a bool simply

void KeyboardOption(char *text, int action, char *defaultText, int maxLength, char* help = NULL)
{
	if (is_current_option()) {
		Menu.c_option_t = 0;
		pointer_color = color_s(75, 0, 130, 255);
		help_text = help;
		if (Menu.call_function)
		{
			Keyboard(action, defaultText, maxLength);
			Menu.call_function = false;
		}
	}
	draw_option(text);
} //for change name / send message / etc

void ApplyOffset(char *text, uint offset, char **From, int* var, int min, int max, unsigned char** ValsOffset)
{
	char buf[50];
	sprintf(buf, "%s: ~italic~%s", text, From[*var]);
	if (is_current_option())
	{
		if (DPAD_LEFT)
		{
			if (*var <= min)
				*var = max;
			else
				*var = *var - 1;
		}
		if (DPAD_RIGHT)
		{
			if (*var >= max)
				*var = min;
			else
				*var = *var + 1;
		}
		pointer_color = color_s(255, 0, 0, 255);
		Menu.c_option_t = 2;
		write_offset(offset, ValsOffset[*var], 4);
	}
	draw_valChar(*var + 1, max + 1, .077); //Because a list ranges from 0 to max value generally, but the draw vars need + 1
	draw_option(buf);
} //for use offset RTM/RPC (for example Big Guns / Minimap colors / etc)

void menu::change_menu(char index, bool s) {
	changing_menu = true;
	previous_menu[submenuLevel] = current_menu;
	if(!s) parent_menu[index] = current_menu;
	current_menu = index;
	drawing_index = 1;
	submenuLevel++;
	changing_menu = false;
}
void open_players_options(int player) {
	Menu.selected_player = player;
	Menu.change_menu(Players_Options);
}
void menu::draw_menu() {
	if (!is_open) return;
	if (current_index[current_menu] + 2 > option_count && current_menu == Players_List) current_index[current_menu] = option_count - 1;
	float difference = ((option_count > MAX_PP ? MAX_PP : option_count) * (.035 * 10)) / 10;
	draw_rect(x_axis, y_axis + .018, .205, .074, shader_color.r, shader_color.g, shader_color.b, shader_color.a);
	draw_rect(x_axis, y_axis + .055 + difference / 2, .205, difference, background_color.r, background_color.g, background_color.b, background_color.a);
	draw_rect(x_axis, y_axis + .074 + difference, .205, .038, shader_color.r, shader_color.g, shader_color.b, shader_color.a);
	draw_text(title, FontShader, 1, x_axis, y_axis - .024, title_color.r, title_color.g, title_color.b, title_color.a, center);
	draw_text(sub_title, 1, .55, x_axis, y_axis + .022, 191, 48, 48, 255, center);
	if (!IS_STRING_NULL_OR_EMPTY(help_text))
	{
		draw_text(help_text, 4, .455, x_axis - .02, y_axis + difference + .057, 0, 255, 255, 255, center);
	}
	draw_text((string)(current_index[current_menu] + 1) + "/" + (string)option_count, 0, .455, x_axis + .08, y_axis + difference + .058, 0, 255, 255, 255, center);
	draw_sprite("commonmenu", "arrowright", x_axis - .094, y_axis + .074 + (.035 * c_drawing_index), .022, .022, pointer_color.r, pointer_color.g, pointer_color.b, pointer_color.a);
	draw_instructions();
	draw_arrows();
	int GlareHandle = REQUEST_SCALEFORM_MOVIE("MP_MENU_GLARE");
	if (ShowGlare)
		DRAW_SCALEFORM_MOVIE(GlareHandle, x_axis + 0.3, 0.389f, 0.88, 0.77, 255, 255, 255, 255, 0);
}

bool Is_One_Friend(Player player)
{
	bool BplayerFriend = false;
	bool bplayerFriend = false;
	int handle[76];
	NETWORK_HANDLE_FROM_PLAYER_TEST(player, &handle[0], 13);
	if (NETWORK_IS_HANDLE_VALID(&handle[0], 13))
	{
		BplayerFriend = NETWORK_IS_FRIEND(&handle[0]);
	}
	if (BplayerFriend)
		bplayerFriend = true;
	else
		bplayerFriend = false;

	return bplayerFriend;
}


void reset_vars() {
	drawing_index = 0;
	Menu.c_option_index = 0;
}
void menu::run_menu() {
	if (!is_open) return;
	reset_vars();

	static int a_int = 0; //value example (to delete)
	static float a_float = 0; //value example (to delete)

	switch (current_menu) {
		case Main_Menu: {
			option_count = 5;
			sub_title = "Main Menu";
			add_option_c("Players list", change_menu, Players_List);
			add_option_c("Sub 1", change_menu, Sub_1);
			add_option_c("Spawn Vehicle", change_menu, Spawn_Vehicle);
			add_option_c("Settings", change_menu, Settings_Menu);
			add_option_c("Credits", change_menu, Credits_Menu);
			break;
		}
		case Players_List: {
			sub_title = "Players List";
			option_count = get_players_count();
			for_player
			{
				char * Name = GET_PLAYER_NAME(player);
				char NameBuffer[100];
				if (Is_One_Friend(player) == true) // if the player is one friend
				{
					sprintf(NameBuffer, "~d~[FRIEND] %s", Name);
					if (!strcmp(Name, "**Invalid**")) continue;
					Name = NameBuffer;
					add_option_c(Name, open_players_options, player);
				}
				else if (player == NETWORK_GET_HOST_OF_SCRIPT("freemode", -1, 0)) // if the player is the host
				{
					sprintf(NameBuffer, "~r~[HOST] %s", Name);
					if (!strcmp(Name, "**Invalid**")) continue;
					Name = NameBuffer;
					add_option_c(Name, open_players_options, player);
				}
				else if (GET_PLAYER_PED(player) == PLAYER_PED_ID()) // if the player is you
				{
					sprintf(NameBuffer, "~f~[YOU] %s", Name);
					if (!strcmp(Name, "**Invalid**")) continue;
					Name = NameBuffer;
					add_option_c(Name, open_players_options, player);
				}
				else // and all others players
				{
					sprintf(NameBuffer, "%s", Name);
					if (!strcmp(Name, "**Invalid**")) continue;
					Name = NameBuffer;
					add_option_c(Name, open_players_options, player);
				}
			}
			break;
		}
		case Players_Options: {
			sub_title = GET_PLAYER_NAME(selected_player);
			option_count = 2;
			add_option_c("Teleport to", teleport, GET_ENTITY_COORDS(GET_PLAYER_PED(selected_player), true));
			add_option_c("Explode", explode_player, selected_player);
			break;
		}
		case Sub_1: {
			sub_title = "Sub 1";
			option_count = 8;
			add_option_i("Edit Int", &a_int, 0, 100, true, true);
			add_option_f("Edit Float", &a_float, 0, 100.0f, 1.0f, true);
			add_option_bc("Godmode", true, toggle_godmode, !*toggle_godmode());
			add_option_bc("Invisible", true, toggle_invisibility, !*toggle_invisibility());
			add_option_bc_arg("Change Model to Jesus", false, set_model, true, "u_m_m_jesus_01");
			add_option_c("Explode Self", explode_player, PLAYER_ID());
			SwapCharBool("Neons vehicle", toggle_neons, !*toggle_neons(), NeonMenu, &NeonType, 0, 9, 0.061);
			ApplyOffset("Minimap colors", 0x01D6B357, MinimapMenu, &MinimapType, 0, 7, MinimapColor);
			break;
		}
		case Spawn_Vehicle: {
			sub_title = "Spawn Vehicle";
			option_count = 20;
			add_option_c("Super sport", change_menu, Super_Car);
			add_option_c("Sport", change_menu, Sportive_Car);
			add_option_c("Classics", change_menu, SportiveC_Car);
			add_option_c("Muscle car", change_menu, Muscle_Car);
			add_option_c("Coupes", change_menu, Coupes_Car);
			add_option_c("Compact", change_menu, Compact_Car);
			add_option_c("Sedans", change_menu, Sedans_Car);
			add_option_c("SUV", change_menu, SUVTrucks_Car);
			add_option_c("Off Road", change_menu, OFFRoad_Car);
			add_option_c("Vans", change_menu, Vans_Car);
			add_option_c("Emergency", change_menu, Emergency_Car);
			add_option_c("Service", change_menu, Service_Car);
			add_option_c("Commercial", change_menu, Commercial_Car);
			add_option_c("Military", change_menu, Military_Car);
			add_option_c("Motorcycles", change_menu, Moto_Car);
			add_option_c("Bicycles", change_menu, Bicycles);
			add_option_c("Helicopters", change_menu, Helicopters_Car);
			add_option_c("Planes", change_menu, Planes_Car);
			add_option_c("Boats", change_menu, Boats_Car);
			add_option_c("Snowy vehicles", change_menu, Snow_Car);
			break;
		}
		case Super_Car: {
			sub_title = "Super sport";
			option_count = 11;
			add_option_bc_arg("Adder", false, spawn_vehicle, true, "adder");
			add_option_bc_arg("Entity XF", false, spawn_vehicle, true, "entityxf");
			add_option_bc_arg("Cheetah", false, spawn_vehicle, true, "cheetah");
			add_option_bc_arg("Infernus", false, spawn_vehicle, true, "infernus");
			add_option_bc_arg("Vacca", false, spawn_vehicle, true, "vacca");
			add_option_bc_arg("Bullet", false, spawn_vehicle, true, "bullet");
			add_option_bc_arg("Voltic", false, spawn_vehicle, true, "voltic");
			add_option_bc_arg("Zentorno", false, spawn_vehicle, true, "zentorno");
			add_option_bc_arg("Turismo R", false, spawn_vehicle, true, "turismor");
			add_option_bc_arg("Osiris", false, spawn_vehicle, true, "osiris");
			add_option_bc_arg("T20", false, spawn_vehicle, true, "t20");
			break;
		}
		case Sportive_Car: {
			sub_title = "Sport";
			option_count = 29;
			add_option_bc_arg("Elegy RH8", false, spawn_vehicle, true, "elegy2");
			add_option_bc_arg("Khamelion", false, spawn_vehicle, true, "khamelion");
			add_option_bc_arg("Carbonizzare", false, spawn_vehicle, true, "carbonizzare");
			add_option_bc_arg("Feltzer", false, spawn_vehicle, true, "feltzer2");
			add_option_bc_arg("Stirling GT", false, spawn_vehicle, true, "feltzer3");
			add_option_bc_arg("Rapid GT", false, spawn_vehicle, true, "rapidgt");
			add_option_bc_arg("Rapid GT cabrio", false, spawn_vehicle, true, "rapidgt2");
			add_option_bc_arg("Coquette", false, spawn_vehicle, true, "coquette");
			add_option_bc_arg("9F", false, spawn_vehicle, true, "ninef");
			add_option_bc_arg("9F cabrio", false, spawn_vehicle, true, "ninef2");
			add_option_bc_arg("Surano", false, spawn_vehicle, true, "surano");
			add_option_bc_arg("Banshee", false, spawn_vehicle, true, "banshee");
			add_option_bc_arg("Comet", false, spawn_vehicle, true, "comet2");
			add_option_bc_arg("Schwartzer", false, spawn_vehicle, true, "schwarzer");
			add_option_bc_arg("Fusilade", false, spawn_vehicle, true, "fusilade");
			add_option_bc_arg("Buffalo", false, spawn_vehicle, true, "buffalo");
			add_option_bc_arg("Buffalo S", false, spawn_vehicle, true, "buffalo2");
			add_option_bc_arg("Penumbra", false, spawn_vehicle, true, "penumbra");
			add_option_bc_arg("Sultan", false, spawn_vehicle, true, "sultan");
			add_option_bc_arg("Futo", false, spawn_vehicle, true, "futo");
			add_option_bc_arg("Furore GT", false, spawn_vehicle, true, "furoregt");
			add_option_bc_arg("Massacro", false, spawn_vehicle, true, "massacro");
			add_option_bc_arg("Massacro race", false, spawn_vehicle, true, "massacro2");
			add_option_bc_arg("Jester", false, spawn_vehicle, true, "jester");
			add_option_bc_arg("Jester race", false, spawn_vehicle, true, "jester2");
			add_option_bc_arg("Windsor", false, spawn_vehicle, true, "windsor");
			add_option_bc_arg("Alpha", false, spawn_vehicle, true, "alpha");
			add_option_bc_arg("Kuruma", false, spawn_vehicle, true, "kuruma");
			add_option_bc_arg("Kuruma armored", false, spawn_vehicle, true, "kuruma2");
			break;
		}
		case SportiveC_Car: {
			sub_title = "Classics";
			option_count = 20;
			add_option_bc_arg("Coquette classic", false, spawn_vehicle, true, "coquette2");
			add_option_bc_arg("Coquette blackfin", false, spawn_vehicle, true, "coquette3");
			add_option_bc_arg("Z-Type", false, spawn_vehicle, true, "ztype");
			add_option_bc_arg("Stinger", false, spawn_vehicle, true, "stinger");
			add_option_bc_arg("Stinger GT", false, spawn_vehicle, true, "stingergt");
			add_option_bc_arg("Monroe", false, spawn_vehicle, true, "monroe");
			add_option_bc_arg("JB 700", false, spawn_vehicle, true, "jb700");
			add_option_bc_arg("Tornado", false, spawn_vehicle, true, "tornado");
			add_option_bc_arg("Tornado cabrio", false, spawn_vehicle, true, "tornado2");
			add_option_bc_arg("Tornado rusty", false, spawn_vehicle, true, "tornado3");
			add_option_bc_arg("Tornado mexican", false, spawn_vehicle, true, "tornado4");
			add_option_bc_arg("Peyote", false, spawn_vehicle, true, "peyote");
			add_option_bc_arg("Manana", false, spawn_vehicle, true, "manana");
			add_option_bc_arg("Virgo", false, spawn_vehicle, true, "virgo");
			add_option_bc_arg("Roosevelt", false, spawn_vehicle, true, "btype");
			add_option_bc_arg("Blade", false, spawn_vehicle, true, "blade");
			add_option_bc_arg("Glendale", false, spawn_vehicle, true, "glendale");
			add_option_bc_arg("Pigalle", false, spawn_vehicle, true, "pigalle");
			add_option_bc_arg("Casco", false, spawn_vehicle, true, "casco");
			add_option_bc_arg("Chino", false, spawn_vehicle, true, "chino");
			break;
		}
		case Muscle_Car: {
			sub_title = "Muscle car";
			option_count = 12;
			add_option_bc_arg("Hotknife", false, spawn_vehicle, true, "hotknife");
			add_option_bc_arg("Gauntlet", false, spawn_vehicle, true, "gauntlet");
			add_option_bc_arg("Vigero", false, spawn_vehicle, true, "vigero");
			add_option_bc_arg("Dominator", false, spawn_vehicle, true, "dominator");
			add_option_bc_arg("Buccaneer", false, spawn_vehicle, true, "buccaneer");
			add_option_bc_arg("Phoenix", false, spawn_vehicle, true, "phoenix");
			add_option_bc_arg("Sabre Turbo", false, spawn_vehicle, true, "sabregt");
			add_option_bc_arg("Ruiner", false, spawn_vehicle, true, "ruiner");
			add_option_bc_arg("Voodoo", false, spawn_vehicle, true, "voodoo2");
			add_option_bc_arg("Picador", false, spawn_vehicle, true, "picador");
			add_option_bc_arg("Ratloader", false, spawn_vehicle, true, "ratloader");
			add_option_bc_arg("Rat-Truck", false, spawn_vehicle, true, "ratloader2");
			break;
		}
		case Coupes_Car: {
			sub_title = "Coupes";
			option_count = 10;
			add_option_bc_arg("Exemplar", false, spawn_vehicle, true, "exemplar");
			add_option_bc_arg("Cabrio", false, spawn_vehicle, true, "cogcabrio");
			add_option_bc_arg("Felon", false, spawn_vehicle, true, "felon");
			add_option_bc_arg("Felon GT", false, spawn_vehicle, true, "felon2");
			add_option_bc_arg("Zion", false, spawn_vehicle, true, "zion");
			add_option_bc_arg("Zion Cabrio", false, spawn_vehicle, true, "zion2");
			add_option_bc_arg("Sentinel", false, spawn_vehicle, true, "sentinel2");
			add_option_bc_arg("Sentinel XS", false, spawn_vehicle, true, "sentinel");
			add_option_bc_arg("Ocelot Jackal", false, spawn_vehicle, true, "jackal");
			add_option_bc_arg("F620", false, spawn_vehicle, true, "f620");
			break;
		}
		case Compact_Car: {
			sub_title = "Compact";
			option_count = 8;
			add_option_bc_arg("Dilettante", false, spawn_vehicle, true, "dilettante");
			add_option_bc_arg("Dilettante patrol", false, spawn_vehicle, true, "dilettante2");
			add_option_bc_arg("Issi", false, spawn_vehicle, true, "issi2");
			add_option_bc_arg("Prairie", false, spawn_vehicle, true, "prairie");
			add_option_bc_arg("Blista", false, spawn_vehicle, true, "blista");
			add_option_bc_arg("Rhapsody", false, spawn_vehicle, true, "rhapsody");
			add_option_bc_arg("Warrener", false, spawn_vehicle, true, "warrener");
			add_option_bc_arg("Panto", false, spawn_vehicle, true, "panto");
			break;
		}
		case Sedans_Car: {
			sub_title = "Sedans";
			option_count = 19;
			add_option_bc_arg("Asea", false, spawn_vehicle, true, "asea");
			add_option_bc_arg("Asterope", false, spawn_vehicle, true, "asterope");
			add_option_bc_arg("Emperor", false, spawn_vehicle, true, "emperor");
			add_option_bc_arg("Emperor rusty", false, spawn_vehicle, true, "emperor2");
			add_option_bc_arg("Limousine", false, spawn_vehicle, true, "stretch");
			add_option_bc_arg("Fugitive", false, spawn_vehicle, true, "fugitive");
			add_option_bc_arg("Intruder", false, spawn_vehicle, true, "intruder");
			add_option_bc_arg("Oracle", false, spawn_vehicle, true, "oracle2");
			add_option_bc_arg("Oracle XS", false, spawn_vehicle, true, "oracle");
			add_option_bc_arg("Premier", false, spawn_vehicle, true, "premier");
			add_option_bc_arg("Primo", false, spawn_vehicle, true, "primo");
			add_option_bc_arg("Regina", false, spawn_vehicle, true, "regina");
			add_option_bc_arg("Romero", false, spawn_vehicle, true, "romero");
			add_option_bc_arg("Tailgater", false, spawn_vehicle, true, "tailgater");
			add_option_bc_arg("Stanier", false, spawn_vehicle, true, "stanier");
			add_option_bc_arg("Stratum", false, spawn_vehicle, true, "stratum");
			add_option_bc_arg("Surge", false, spawn_vehicle, true, "surge");
			add_option_bc_arg("Schafter", false, spawn_vehicle, true, "schafter2");
			add_option_bc_arg("Washington", false, spawn_vehicle, true, "washington");
			break;
		}
		case SUVTrucks_Car: {
			sub_title = "SUV";
			option_count = 24;
			add_option_bc_arg("Baller", false, spawn_vehicle, true, "baller");
			add_option_bc_arg("Baller 2", false, spawn_vehicle, true, "baller2");
			add_option_bc_arg("Rocoto", false, spawn_vehicle, true, "rocoto");
			add_option_bc_arg("Dubsta", false, spawn_vehicle, true, "dubsta");
			add_option_bc_arg("Dubsta gang", false, spawn_vehicle, true, "dubsta2");
			add_option_bc_arg("Dubsta 6x6", false, spawn_vehicle, true, "dubsta3");
			add_option_bc_arg("Serrano", false, spawn_vehicle, true, "serrano");
			add_option_bc_arg("Landstalker", false, spawn_vehicle, true, "landstalker");
			add_option_bc_arg("FQ 2", false, spawn_vehicle, true, "fq2");
			add_option_bc_arg("Patriot", false, spawn_vehicle, true, "patriot");
			add_option_bc_arg("Habanero", false, spawn_vehicle, true, "habanero");
			add_option_bc_arg("Radius", false, spawn_vehicle, true, "radi");
			add_option_bc_arg("Granger", false, spawn_vehicle, true, "granger");
			add_option_bc_arg("Mesa", false, spawn_vehicle, true, "mesa");
			add_option_bc_arg("Mesa merryweather", false, spawn_vehicle, true, "mesa3");
			add_option_bc_arg("Seminole", false, spawn_vehicle, true, "seminole");
			add_option_bc_arg("Kalahari", false, spawn_vehicle, true, "kalahari");
			add_option_bc_arg("Gresley", false, spawn_vehicle, true, "gresley");
			add_option_bc_arg("BeeJay XL", false, spawn_vehicle, true, "bjxl");
			add_option_bc_arg("Huntley S", false, spawn_vehicle, true, "huntley");
			add_option_bc_arg("Sadler", false, spawn_vehicle, true, "sadler");
			add_option_bc_arg("Guardian", false, spawn_vehicle, true, "guardian");
			add_option_bc_arg("Cavalcade", false, spawn_vehicle, true, "cavalcade");
			add_option_bc_arg("Cavalcade 2", false, spawn_vehicle, true, "cavalcade2");
			break;
		}
		case OFFRoad_Car: {
			sub_title = "Off Road";
			option_count = 19;
			add_option_bc_arg("Liberator", false, spawn_vehicle, true, "monster");
			add_option_bc_arg("Sandking XL", false, spawn_vehicle, true, "sandking");
			add_option_bc_arg("Sandking SWB", false, spawn_vehicle, true, "sandking2");
			add_option_bc_arg("Dune Buggy", false, spawn_vehicle, true, "dune");
			add_option_bc_arg("Space Docker", false, spawn_vehicle, true, "dune2");
			add_option_bc_arg("BF Injection", false, spawn_vehicle, true, "bfinjection");
			add_option_bc_arg("Bifta", false, spawn_vehicle, true, "bifta");
			add_option_bc_arg("Blazer", false, spawn_vehicle, true, "blazer");
			add_option_bc_arg("Blazer lifeguard", false, spawn_vehicle, true, "blazer2");
			add_option_bc_arg("Blazer Trevor", false, spawn_vehicle, true, "blazer3");
			add_option_bc_arg("DuneLoader", false, spawn_vehicle, true, "dloader");
			add_option_bc_arg("Bodhi", false, spawn_vehicle, true, "bodhi");
			add_option_bc_arg("Rancher XL", false, spawn_vehicle, true, "rancherxl");
			add_option_bc_arg("Rebel", false, spawn_vehicle, true, "rebel2");
			add_option_bc_arg("Rebel rusty", false, spawn_vehicle, true, "rebel");
			add_option_bc_arg("Brawler", false, spawn_vehicle, true, "brawler");
			add_option_bc_arg("Insurgent", false, spawn_vehicle, true, "insurgent2");
			add_option_bc_arg("Insurgent armed", false, spawn_vehicle, true, "insurgent");
			add_option_bc_arg("Technical", false, spawn_vehicle, true, "technical");
			break;
		}
		case Vans_Car: {
			sub_title = "Vans";
			option_count = 27;
			add_option_bc_arg("Bobcat XL", false, spawn_vehicle, true, "bobcatxl");
			add_option_bc_arg("Boxville", false, spawn_vehicle, true, "boxville");
			add_option_bc_arg("Boxville 2", false, spawn_vehicle, true, "boxville2");
			add_option_bc_arg("Boxville 3", false, spawn_vehicle, true, "boxville3");
			add_option_bc_arg("Boxville 4", false, spawn_vehicle, true, "boxville4");
			add_option_bc_arg("Speedo", false, spawn_vehicle, true, "speedo");
			add_option_bc_arg("Speedo Clown", false, spawn_vehicle, true, "speedo2");
			add_option_bc_arg("Bison", false, spawn_vehicle, true, "bison");
			add_option_bc_arg("Bison 2", false, spawn_vehicle, true, "bison2");
			add_option_bc_arg("Bison 3", false, spawn_vehicle, true, "bison3");
			add_option_bc_arg("Burrito", false, spawn_vehicle, true, "burrito");
			add_option_bc_arg("Burrito 2", false, spawn_vehicle, true, "burrito2");
			add_option_bc_arg("Burrito 3", false, spawn_vehicle, true, "burrito3");
			add_option_bc_arg("Burrito 4", false, spawn_vehicle, true, "burrito4");
			add_option_bc_arg("Journey", false, spawn_vehicle, true, "journey");
			add_option_bc_arg("Minivan", false, spawn_vehicle, true, "minivan");
			add_option_bc_arg("Paradise", false, spawn_vehicle, true, "paradise");
			add_option_bc_arg("Pony", false, spawn_vehicle, true, "pony");
			add_option_bc_arg("Pony Weed", false, spawn_vehicle, true, "pony2");
			add_option_bc_arg("Rumpo", false, spawn_vehicle, true, "rumpo");
			add_option_bc_arg("Rumpo 2", false, spawn_vehicle, true, "rumpo2");
			add_option_bc_arg("Slamvan", false, spawn_vehicle, true, "slamvan");
			add_option_bc_arg("Slamvan 2", false, spawn_vehicle, true, "slamvan2");
			add_option_bc_arg("Surfer", false, spawn_vehicle, true, "surfer");
			add_option_bc_arg("Surfer rusty", false, spawn_vehicle, true, "surfer2");
			add_option_bc_arg("Taco Van", false, spawn_vehicle, true, "taco");
			add_option_bc_arg("Youga", false, spawn_vehicle, true, "youga");
			break;
		}
		case Emergency_Car: {
			sub_title = "Emergency";
			option_count = 16;
			add_option_bc_arg("FIB buffalo", false, spawn_vehicle, true, "fbi");
			add_option_bc_arg("FIB granger", false, spawn_vehicle, true, "fbi2");
			add_option_bc_arg("Firetruck", false, spawn_vehicle, true, "firetruk");
			add_option_bc_arg("Ambulance", false, spawn_vehicle, true, "ambulance");
			add_option_bc_arg("Police stanier", false, spawn_vehicle, true, "police");
			add_option_bc_arg("Police buffalo", false, spawn_vehicle, true, "police2");
			add_option_bc_arg("Police interceptor", false, spawn_vehicle, true, "police3");
			add_option_bc_arg("Unmarked car", false, spawn_vehicle, true, "police4");
			add_option_bc_arg("Police bike", false, spawn_vehicle, true, "policeb");
			add_option_bc_arg("Police Van", false, spawn_vehicle, true, "policet");
			add_option_bc_arg("Police RIOT", false, spawn_vehicle, true, "riot");
			add_option_bc_arg("Sheriff stanier", false, spawn_vehicle, true, "sheriff");
			add_option_bc_arg("Sheriff granger", false, spawn_vehicle, true, "sheriff2");
			add_option_bc_arg("Prison Bus", false, spawn_vehicle, true, "pbus");
			add_option_bc_arg("Park Ranger", false, spawn_vehicle, true, "pranger");
			add_option_bc_arg("SUV lifeguard", false, spawn_vehicle, true, "lguard");
			break;
		}
		case Service_Car: {
			sub_title = "Service";
			option_count = 8;
			add_option_bc_arg("Bus airport", false, spawn_vehicle, true, "airbus");
			add_option_bc_arg("Bus", false, spawn_vehicle, true, "bus");
			add_option_bc_arg("Bus dashhound", false, spawn_vehicle, true, "coach");
			add_option_bc_arg("Bus of rent", false, spawn_vehicle, true, "rentalbus");
			add_option_bc_arg("Bus tourist", false, spawn_vehicle, true, "tourbus");
			add_option_bc_arg("Taxi", false, spawn_vehicle, true, "taxi");
			add_option_bc_arg("Large towtruck", false, spawn_vehicle, true, "towtruck");
			add_option_bc_arg("Small towtruck", false, spawn_vehicle, true, "towtruck2");
			break;
		}
		case Commercial_Car: {
			sub_title = "Commercial";
			option_count = 28;
			add_option_bc_arg("Mule", false, spawn_vehicle, true, "mule2");
			add_option_bc_arg("Phantom", false, spawn_vehicle, true, "phantom");
			add_option_bc_arg("Benson", false, spawn_vehicle, true, "benson");
			add_option_bc_arg("Packer", false, spawn_vehicle, true, "packer");
			add_option_bc_arg("Pounder", false, spawn_vehicle, true, "pounder");
			add_option_bc_arg("Hauler", false, spawn_vehicle, true, "hauler"); 
			add_option_bc_arg("Stockade", false, spawn_vehicle, true, "stockade");
			add_option_bc_arg("Biff", false, spawn_vehicle, true, "biff");
			add_option_bc_arg("Large dumptruck", false, spawn_vehicle, true, "dump");
			add_option_bc_arg("Bulldozer", false, spawn_vehicle, true, "bulldozer");
			add_option_bc_arg("Forklift", false, spawn_vehicle, true, "forklift");
			add_option_bc_arg("Container carrier", false, spawn_vehicle, true, "handler");
			add_option_bc_arg("Cutter", false, spawn_vehicle, true, "cutter");
			add_option_bc_arg("Dump truck", false, spawn_vehicle, true, "trash");
			add_option_bc_arg("Dump truck rusty", false, spawn_vehicle, true, "trash2");
			add_option_bc_arg("Tipper", false, spawn_vehicle, true, "tiptruck2");
			add_option_bc_arg("Brute tipper", false, spawn_vehicle, true, "tiptruck");
			add_option_bc_arg("Mixer", false, spawn_vehicle, true, "mixer2");
			add_option_bc_arg("Car carrier", false, spawn_vehicle, true, "flatbed");
			add_option_bc_arg("Ripley", false, spawn_vehicle, true, "ripley");
			add_option_bc_arg("Scrap metal truck", false, spawn_vehicle, true, "scrap");
			add_option_bc_arg("Lawn mower", false, spawn_vehicle, true, "mower");
			add_option_bc_arg("Docktug", false, spawn_vehicle, true, "docktug");
			add_option_bc_arg("Airtug", false, spawn_vehicle, true, "airtug");
			add_option_bc_arg("Tractor old", false, spawn_vehicle, true, "tractor");
			add_option_bc_arg("Tractor farm", false, spawn_vehicle, true, "tractor2");
			add_option_bc_arg("Caddy", false, spawn_vehicle, true, "caddy");
			add_option_bc_arg("Caddy old", false, spawn_vehicle, true, "caddy2");
			break;
		}
		case Military_Car: {
			sub_title = "Military";
			option_count = 5;
			add_option_bc_arg("4x4 Crusader", false, spawn_vehicle, true, "crusader");
			add_option_bc_arg("Barracks", false, spawn_vehicle, true, "barracks");
			add_option_bc_arg("Barracks semi", false, spawn_vehicle, true, "barracks2");
			add_option_bc_arg("Barracks movie", false, spawn_vehicle, true, "barracks3");
			add_option_bc_arg("Rhino Tank", false, spawn_vehicle, true, "rhino");
			break;
		}
		case Moto_Car: {
			sub_title = "Motorcycles";
			option_count = 22;
			add_option_bc_arg("Carbon RS", false, spawn_vehicle, true, "carbonrs");
			add_option_bc_arg("Bati 801", false, spawn_vehicle, true, "bati");
			add_option_bc_arg("Bati 801RR", false, spawn_vehicle, true, "bati2");
			add_option_bc_arg("Hexer", false, spawn_vehicle, true, "hexer");
			add_option_bc_arg("Innovation", false, spawn_vehicle, true, "innovation");
			add_option_bc_arg("Double-T", false, spawn_vehicle, true, "double");
			add_option_bc_arg("Thrust", false, spawn_vehicle, true, "thrust");
			add_option_bc_arg("Vindicator", false, spawn_vehicle, true, "vindicator");
			add_option_bc_arg("Ruffian", false, spawn_vehicle, true, "ruffian");
			add_option_bc_arg("Vader", false, spawn_vehicle, true, "vader");
			add_option_bc_arg("PCJ 600", false, spawn_vehicle, true, "pcj");
			add_option_bc_arg("Hakuchou", false, spawn_vehicle, true, "hakuchou");
			add_option_bc_arg("Akuma", false, spawn_vehicle, true, "akuma");
			add_option_bc_arg("Sanchez (colors)", false, spawn_vehicle, true, "sanchez2");
			add_option_bc_arg("Sanchez", false, spawn_vehicle, true, "sanchez");
			add_option_bc_arg("Faggio", false, spawn_vehicle, true, "faggio2");
			add_option_bc_arg("Daemon", false, spawn_vehicle, true, "daemon");
			add_option_bc_arg("Bagger", false, spawn_vehicle, true, "bagger");
			add_option_bc_arg("Nemesis", false, spawn_vehicle, true, "nemesis");
			add_option_bc_arg("Sovereign", false, spawn_vehicle, true, "sovereign");
			add_option_bc_arg("Enduro", false, spawn_vehicle, true, "enduro");
			add_option_bc_arg("Lectro", false, spawn_vehicle, true, "lectro")
			break;
		}
		case Bicycles: {
			sub_title = "Bicycles";
			option_count = 7;
			add_option_bc_arg("BMX", false, spawn_vehicle, true, "bmx");
			add_option_bc_arg("Cruiser", false, spawn_vehicle, true, "cruiser");
			add_option_bc_arg("Endurex", false, spawn_vehicle, true, "tribike2");
			add_option_bc_arg("Fixter", false, spawn_vehicle, true, "fixter");
			add_option_bc_arg("Scorcher", false, spawn_vehicle, true, "scorcher");
			add_option_bc_arg("Tri-cycles", false, spawn_vehicle, true, "tribike3");
			add_option_bc_arg("Whippet", false, spawn_vehicle, true, "tribike");
			break;
		}
		case Helicopters_Car: {
			sub_title = "Helicopters";
			option_count = 14;
			add_option_bc_arg("Annihilator", false, spawn_vehicle, true, "annihilator");
			add_option_bc_arg("Buzzard", false, spawn_vehicle, true, "buzzard2");
			add_option_bc_arg("Buzzard attack", false, spawn_vehicle, true, "buzzard");
			add_option_bc_arg("Frogger", false, spawn_vehicle, true, "frogger");
			add_option_bc_arg("Frogger Trevor", false, spawn_vehicle, true, "frogger2");
			add_option_bc_arg("Maverick", false, spawn_vehicle, true, "maverick");
			add_option_bc_arg("Cargobob", false, spawn_vehicle, true, "cargobob");
			add_option_bc_arg("Cargobob rescue", false, spawn_vehicle, true, "cargobob2");
			add_option_bc_arg("Maverick police", false, spawn_vehicle, true, "polmav");
			add_option_bc_arg("Swift", false, spawn_vehicle, true, "swift");
			add_option_bc_arg("Swift deluxe", false, spawn_vehicle, true, "swift2");
			add_option_bc_arg("Valkyrie", false, spawn_vehicle, true, "valkyrie");
			add_option_bc_arg("Savage", false, spawn_vehicle, true, "savage");
			add_option_bc_arg("Skylift", false, spawn_vehicle, true, "skylift");
			break;
		}
		case Planes_Car: {
			sub_title = "Planes";
			option_count = 18;
			add_option_bc_arg("Titan", false, spawn_vehicle, true, "titan");
			add_option_bc_arg("Luxor", false, spawn_vehicle, true, "luxor");
			add_option_bc_arg("Luxor deluxe", false, spawn_vehicle, true, "luxor2");
			add_option_bc_arg("Shamal", false, spawn_vehicle, true, "shamal");
			add_option_bc_arg("Vestra", false, spawn_vehicle, true, "vestra");
			add_option_bc_arg("Miljet", false, spawn_vehicle, true, "miljet");
			add_option_bc_arg("Velum", false, spawn_vehicle, true, "velum");
			add_option_bc_arg("Velum 5 seats", false, spawn_vehicle, true, "velum2");
			add_option_bc_arg("Mammatus", false, spawn_vehicle, true, "mammatus");
			add_option_bc_arg("Duster", false, spawn_vehicle, true, "duster");
			add_option_bc_arg("Mallard", false, spawn_vehicle, true, "stunt");
			add_option_bc_arg("Cuban 800", false, spawn_vehicle, true, "cuban800");
			add_option_bc_arg("Cargo Plane", false, spawn_vehicle, true, "cargoplane");
			add_option_bc_arg("Blimp", false, spawn_vehicle, true, "blimp");
			add_option_bc_arg("P-996 Lazer", false, spawn_vehicle, true, "lazer");
			add_option_bc_arg("Jet", false, spawn_vehicle, true, "jet");
			add_option_bc_arg("Besra", false, spawn_vehicle, true, "besra");
			add_option_bc_arg("Hydra", false, spawn_vehicle, true, "hydra");
			break;
		}
		case Boats_Car: {
			sub_title = "Boats";
			option_count = 13;
			add_option_bc_arg("Marquis", false, spawn_vehicle, true, "marquis");
			add_option_bc_arg("Jetmax", false, spawn_vehicle, true, "jetmax");
			add_option_bc_arg("Squalo", false, spawn_vehicle, true, "squalo");
			add_option_bc_arg("Suntrap", false, spawn_vehicle, true, "suntrap");
			add_option_bc_arg("Tropic", false, spawn_vehicle, true, "tropic");
			add_option_bc_arg("Seashark", false, spawn_vehicle, true, "seashark");
			add_option_bc_arg("Seashark lifeguard", false, spawn_vehicle, true, "seashark2");
			add_option_bc_arg("Bateau police", false, spawn_vehicle, true, "predator");
			add_option_bc_arg("Submersible", false, spawn_vehicle, true, "submersible");
			add_option_bc_arg("Speeder", false, spawn_vehicle, true, "speeder");
			add_option_bc_arg("Dinghy 2 seats", false, spawn_vehicle, true, "dinghy2");
			add_option_bc_arg("Dinghy 4 seats", false, spawn_vehicle, true, "dinghy");
			add_option_bc_arg("Toro", false, spawn_vehicle, true, "toro");
			break;
		}
		case Snow_Car: {
			sub_title = "Snowy vehicles";
			option_count = 10;
			add_option_bc_arg("Asea", false, spawn_vehicle, true, "asea2");
			add_option_bc_arg("Burrito", false, spawn_vehicle, true, "burrito5");
			add_option_bc_arg("Emperor", false, spawn_vehicle, true, "emperor3");
			add_option_bc_arg("Rancher police", false, spawn_vehicle, true, "policeold1");
			add_option_bc_arg("Esperanto police", false, spawn_vehicle, true, "policeold2");
			add_option_bc_arg("Mesa", false, spawn_vehicle, true, "mesa2");
			add_option_bc_arg("Rancher XL", false, spawn_vehicle, true, "rancherxl2");
			add_option_bc_arg("Sadler", false, spawn_vehicle, true, "sadler2");
			add_option_bc_arg("Stockade", false, spawn_vehicle, true, "stockade3");
			add_option_bc_arg("Tractor", false, spawn_vehicle, true, "tractor3");
			break;
		}
		case Settings_Menu: {
			sub_title = "Settings";
			option_count = 5;
			add_option_c("Shader >", change_menu, Settings_Shader);
			add_option_c("Background >", change_menu, Settings_Background);
			add_option_f("Edit X Axis", &x_axis, 0.0, 1.0, 0.005, true);
			//add_option_f("Edit Y Axis", &y_axis, 0, 1, 0.005, true);
			add_option_bc("FPS counter", true, FPS, !*FPS());
			add_option_bc("Animated globe", true, toggle_Glare, !*toggle_Glare());
			break;
		}
		case Settings_Shader: {
			sub_title = "Shader";
			option_count = 9;
			add_option_i("Text Red", &title_color.r, 0, 255, true, true);
			add_option_i("Text Green", &title_color.g, 0, 255, true, true);
			add_option_i("Text Blue", &title_color.b, 0, 255, true, true);
			add_option_i("Opacity text", &title_color.a, 0, 255, true, true);
			add_option_i("Font text", &FontShader, 0, 7);
			add_option_i("Body Red", &shader_color.r, 0, 255, true, true);
			add_option_i("Body Green", &shader_color.g, 0, 255, true, true);
			add_option_i("Body Blue", &shader_color.b, 0, 255, true, true);
			add_option_i("Body Opacity", &shader_color.a, 0, 255, true, true);
			break;
		}
		case Settings_Background: {
			sub_title = "Background";
			option_count = 9;
			add_option_i("Text Red", &options_color.r, 0, 255, true, true);
			add_option_i("Text Green", &options_color.g, 0, 255, true, true);
			add_option_i("Text Blue", &options_color.b, 0, 255, true, true);
			add_option_i("Opacity text", &options_color.a, 0, 255, true, true);
			add_option_i("Font text", &FontBackground, 0, 7);
			add_option_i("Red body", &background_color.r, 0, 255, true, true);
			add_option_i("Green body", &background_color.g, 0, 255, true, true);
			add_option_i("Blue body", &background_color.b, 0, 255, true, true);
			add_option_i("Opacity body", &background_color.a, 0, 255, true, true);
			break;
		}
		case Credits_Menu: {
			sub_title = "Credits";
			option_count = 3;
			add_option_n("John", "Creator of his base");
			add_option_n("Dylan57richem", "Improvements & additions");
			add_option_n("gopro_2027", "Codes sprx for CEX Cobra");
			break;
		}
}
}

void menu::init() {
	title = "Menu Base Remake";
}

