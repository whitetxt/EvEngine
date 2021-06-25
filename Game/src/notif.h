struct Notif {
	struct Text text;
	bool fadeIn;
	bool fadeOut;
	float timeLeft;
};

void initNotifs();
void createNotif(char *text, float life);
void renderNotifs();