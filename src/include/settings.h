typedef struct configuration {
	const char* name;
	int playerTextSize;
	int width;
	int height;
	int menuSize;
	int titleSize;
	int notifSize;
	int mainMenuSize;
} configuration;

extern configuration Settings;

int loadSettings();