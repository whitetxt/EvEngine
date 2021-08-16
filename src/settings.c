#include "include/main.h"
#include <ini.h>

configuration Settings;

static int handler(void* user, const char* section, const char* name, const char* value) { // Stolen straight from an example lol
	configuration* pconfig = (configuration*)user;

	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("multiplayer", "name")) {
		pconfig->name = strdup(value);
	} else if (MATCH("multiplayer", "playertextsize")) {
		pconfig->playerTextSize = atoi(value);
	} else if (MATCH("video", "width")) {
		pconfig->width = atoi(value);
	} else if (MATCH("video", "height")) {
		pconfig->height = atoi(value);
	} else if (MATCH("menu", "menusize")) {
		pconfig->menuSize = atoi(value);
	} else if (MATCH("menu", "titlesize")) {
		pconfig->titleSize = atoi(value);
	} else if (MATCH("general", "notifsize")) {
		pconfig->notifSize = atoi(value);
	} else if (MATCH("general", "mainmenusize")) {
		pconfig->mainMenuSize = atoi(value);
	} else {
		return 0;  /* unknown section/name, error */
	}
	return 1;
}

// Loads settings from "settings.ini"
// @return 0 on success, 1 on failure
int loadSettings() {
	if (ini_parse("settings.ini", handler, &Settings) < 0) {
		printf("Can't load 'settings.ini'\n");
		return 1;
	}
	return 0;
}