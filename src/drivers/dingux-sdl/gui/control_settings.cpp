// Externals
extern Config *g_config;

/* MENU COMMANDS */

static void show_mouse_update(unsigned long key) {
	int val;

	if (key == DINGOO_RIGHT)
		val = 1;
	if (key == DINGOO_LEFT)
		val = 0;

	g_config->setOption("SDL.ShowMouseCursor", val);
}

static void mouse_update(unsigned long key) {
	int val;
	g_config->getOption("SDL.MouseSpeed", &val);

	if (key == DINGOO_RIGHT)
		val = val < 8 ? val + 1 : 8;
	if (key == DINGOO_LEFT)
		val = val > 0 ? val - 1 : 0;

	g_config->setOption("SDL.MouseSpeed", val);
}

int keyCheck(unsigned long key)
{
	if (key == DINGOO_B) return DefaultGamePad[0][1];
	if (key == DINGOO_A) return DefaultGamePad[0][0];
	if (key == DINGOO_Y) return DefaultGamePad[0][9];
	if (key == DINGOO_X) return DefaultGamePad[0][8];

	return -1;	
}

//!!! BUG LOCATED IN SDL LIBRARY GUI.CPP 
//!!! << configGamePadButton prefix creation SDL.Input.GamePad.%d missing "." at end >> 
//!!! FOLLOWING STRINGS MAY BE DEPRECATED IF FIXED IN FUTURE RELEASE
static void setA(unsigned long key)
{   	
	g_config->setOption("SDL.Input.GamePad.0A", keyCheck(key));
	UpdateInput(g_config);
}

static void setB(unsigned long key)
{   	
	g_config->setOption("SDL.Input.GamePad.0B", keyCheck(key));
	UpdateInput(g_config);
}

static void setTurboB(unsigned long key)
{   	
	g_config->setOption("SDL.Input.GamePad.0TurboB", keyCheck(key));
	UpdateInput(g_config);
}

static void setTurboA(unsigned long key)
{   	
	g_config->setOption("SDL.Input.GamePad.0TurboA", keyCheck(key));
	UpdateInput(g_config);
}

static void MergeControls(unsigned long key)
{
	int val;

	if (key == DINGOO_RIGHT)
		val = 1;
	if (key == DINGOO_LEFT)
		val = 0;

	g_config->setOption("SDL.MergeControls", val);
}

static void setAnalogStick(unsigned long key)
{
	int val;

	if (key == DINGOO_RIGHT)
		val = 1;
	if (key == DINGOO_LEFT)
		val = 0;

	g_config->setOption("SDL.AnalogStick", val);
}

static void setAutoFirePattern(unsigned long key)
{
	int val;
	g_config->getOption("SDL.AutoFirePattern", &val);

	if (key == DINGOO_RIGHT) val = val < 2 ? val + 1 : 2;
	if (key == DINGOO_LEFT) val = val > 0 ? val - 1 : 0;

	g_config->setOption("SDL.AutoFirePattern", val);
}

static void InputMenu(unsigned long key)
{
	int val;
	g_config->getOption("SDL.InputMenu", &val);

	if (key == DINGOO_RIGHT) val = val < 3 ? val + 1 : 3;
	if (key == DINGOO_LEFT) val = val > 0 ? val - 1 : 0;
   
	g_config->setOption("SDL.InputMenu", val);
	UpdateInput(g_config);
}

static void resetMappings(unsigned long key)
{
	g_config->setOption("SDL.Input.GamePad.0A", DefaultGamePad[0][1]);
	g_config->setOption("SDL.Input.GamePad.0B", DefaultGamePad[0][9]);
	g_config->setOption("SDL.Input.GamePad.0TurboA", DefaultGamePad[0][0]);
	g_config->setOption("SDL.Input.GamePad.0TurboB", DefaultGamePad[0][8]);
	g_config->setOption("SDL.MergeControls", 0);
	g_config->setOption("SDL.AnalogStick", 0);
	g_config->setOption("SDL.AutoFirePattern", 0);
	g_config->setOption("SDL.InputMenu", 0);
	g_config->setOption("SDL.MouseSpeed", 3);
	g_config->setOption("SDL.ShowMouseCursor", 0);
	UpdateInput(g_config);
}
/* CONTROL SETTING MENU */

static SettingEntry cm_menu[] = 
{
	{ "Button A", "Map input for A", "SDL.Input.GamePad.0A", setA },
	{ "Button B", "Map input for B", "SDL.Input.GamePad.0B", setB },
	{ "Turbo A", "Map input for Turbo A", "SDL.Input.GamePad.0TurboA", setTurboA },
	{ "Turbo B", "Map input for Turbo B", "SDL.Input.GamePad.0TurboB", setTurboB },
	{ "Merge P1/P2", "Control both players at once", "SDL.MergeControls", MergeControls },
	{ "Analog Stick", "Analog Stick for Directions", "SDL.AnalogStick", setAnalogStick },
	{ "Auto-fire Pattern", "Set auto-fire pattern", "SDL.AutoFirePattern", setAutoFirePattern },	
	{ "Mouse cursor", "Show/hide mouse cursor", "SDL.ShowMouseCursor", show_mouse_update },
	{ "Mouse speed", "Mouse cursor speed", "SDL.MouseSpeed", mouse_update },
	{ "Menu", "Input to open the menu", "SDL.InputMenu", InputMenu },
	{ "Reset defaults", "Reset default control mappings", "", resetMappings },
};

int RunControlSettings()
{
	static int index = 0;
	static int spy = 72;
	int done = 0, y, i;
	int err = 1;
	int editMode = 0;

	static const int menu_size = sizeof(cm_menu) / sizeof(cm_menu[0]);
	static const int max_entries = 8;
	static int offset_start = 0;
	static int offset_end = menu_size > max_entries ? max_entries : menu_size;
	g_dirty = 1;
	while (!done) {
		// Parse input
		readkey();
		// if (parsekey(DINGOO_SELECT)) {
		// if (parsekey(DINGOO_A)) {
		// }

		if ( editMode ) {
			if (parsekey(DINGOO_A, 0) || parsekey(DINGOO_B, 0) || parsekey(DINGOO_X, 0) || parsekey(DINGOO_Y, 0)) {
				cm_menu[index].update(g_key);
				g_dirty = 1;
				editMode = 0;
			}
		} else {
			if (parsekey(DINGOO_A)) {
				if(index < 4) // Allow edit mode only for button mapping menu items
				{
					editMode = 1;
					DrawText(gui_screen, ">>", 185, spy);
					g_dirty = 0;
				} else { // if (index > 3) {
					cm_menu[index].update(g_key);
				}
			}
			if (parsekey(DINGOO_B)) {
				//ERROR CHECKING
				int iBtn1 = -1;
				int iBtn2 = -1;
				err = 1;
				for ( int i = 0; i < 4; i++ ) {
					for ( int y = 0; y < 4; y++ ) {
						g_config->getOption(cm_menu[i].option, &iBtn1);
						if (i != y) {
							g_config->getOption(cm_menu[y].option, &iBtn2);
							if (iBtn1 == iBtn2) {
								err = 0;
								g_dirty = 1;
							}
							
						}
					}
				}

				done= err;
			}
			if (parsekey(DINGOO_UP, 1)) {
				if (index > 0) {
					index--;
					if (index >= offset_start)
						spy -= 15;
					if ((offset_start > 0) && (index < offset_start)) {
						offset_start--;
						offset_end--;
					}
				} else {
					index = menu_size-1;
					offset_end = menu_size;
					offset_start = menu_size <= max_entries ? 0 : offset_end - max_entries;
					spy = 72 + 15*(index - offset_start);
				}
			}

			if (parsekey(DINGOO_DOWN, 1)) {
				if (index < (menu_size - 1)) {
					index++;
					if (index < offset_end)
						spy += 15;
					if ((offset_end < menu_size) && (index >= offset_end)) {
						offset_end++;
						offset_start++;
					}
				} else {
					index = 0;
					offset_start = 0;
					offset_end = menu_size <= max_entries ? menu_size : max_entries;
					spy = 72;
				}
			}
	   		if ( (index > 3 && index != menu_size - 1) &&
	   			(parsekey(DINGOO_LEFT, 1) || parsekey(DINGOO_RIGHT, 1))
   			) {
				cm_menu[index].update(g_key);
			}
		}
  
		// Draw stuff
		if( g_dirty ) 
		{
			draw_bg(g_bg);
			
			//Draw Top and Bottom Bars
			DrawChar(gui_screen, SP_SELECTOR, 0, 37);
			DrawChar(gui_screen, SP_SELECTOR, 81, 37);
			DrawChar(gui_screen, SP_SELECTOR, 0, 225);
			DrawChar(gui_screen, SP_SELECTOR, 81, 225);
			DrawText(gui_screen, "B - Go Back", 235, 225);
			DrawChar(gui_screen, SP_LOGO, 12, 9);
			
			// Draw selector
			DrawChar(gui_screen, SP_SELECTOR, 56, spy);
			DrawChar(gui_screen, SP_SELECTOR, 77, spy);
			if (err == 0) {
				DrawText(gui_screen, "!!!Error - Duplicate Key Mapping!!! ", 8, 37);
			} else {
				DrawText(gui_screen, "Control Settings", 8, 37);
			}

			// Draw menu
			for (i = offset_start, y = 72; i < offset_end; i++, y += 15) {
				int iBtnVal = -1;
				char cBtn[32];
				int mergeValue;

				DrawText(gui_screen, cm_menu[i].name, 60, y);
				
				g_config->getOption(cm_menu[i].option, &iBtnVal);
				
				if (i < 4) {
					if (iBtnVal == DefaultGamePad[0][0]) sprintf(cBtn, "%s", "A");
					else if (iBtnVal == DefaultGamePad[0][1]) sprintf(cBtn, "%s", "B");
					else if (iBtnVal == DefaultGamePad[0][8]) sprintf(cBtn, "%s", "X");
					else if (iBtnVal == DefaultGamePad[0][9]) sprintf(cBtn, "%s", "Y");
					else sprintf(cBtn, "%s", "<empty>");
				}

				else if (!strcmp(cm_menu[i].name, "Reset defaults"))
					sprintf(cBtn, "%s", "");
				else if (
					!strcmp(cm_menu[i].name, "Merge P1/P2")
					|| !strcmp(cm_menu[i].name, "Mouse cursor")
					|| !strcmp(cm_menu[i].name,  "Analog Stick")
				) {
					sprintf(cBtn, "%s", iBtnVal ? "on" : "off");
				}
				else if(!strcmp(cm_menu[i].name, "Auto-fire Pattern"))
                {
					const char *autofire_text[] = {"1 on/1 off", "2 on/2 off", "1 on/3 off"};
					g_config->getOption("SDL.AutoFirePattern", &iBtnVal);
					sprintf(cBtn, "%s", autofire_text[iBtnVal]);
                }				
				else if(!strcmp(cm_menu[i].name, "Menu"))
                {
					const char *menu_button[] = {"Pwr/Sel+Strt/L2",	"Power","Select+Start","L2"};
					g_config->getOption("SDL.InputMenu", &iBtnVal);
					sprintf(cBtn, "%s", menu_button[iBtnVal]);
                }
				else sprintf(cBtn, "%d", iBtnVal);

				DrawText(gui_screen, cBtn, 210, y);
				
			}

			// Draw info
			DrawText(gui_screen, cm_menu[index].info, 8, 225);

			// Draw offset marks
			if (offset_start > 0)
				DrawChar(gui_screen, SP_UPARROW, 157, 62);
			if (offset_end < menu_size)
				DrawChar(gui_screen, SP_DOWNARROW, 157, 203);

			g_dirty = 0;
		}

		SDL_Delay(16);
		
		// Update real screen
		FCEUGUI_Flip();
	}	

	// Clear screen
	dingoo_clear_video();

	g_dirty = 1;
	return 0;
}
