#include "include/main.h"
#include <zed_net.h>

zed_net_address_t server;
zed_net_socket_t Sock;
char *lastSent = "A";

int setupClient() {
	if (zed_net_init() != 0) {
		printf("Error initialising zed_net: %s\n", zed_net_get_error());
		return -1;
	}

	zed_net_socket_t intSock;

	int rc = zed_net_udp_socket_open(&intSock, 0, 0); // Open the socket

	if (rc == -1) { // Check the socket opened
		printf("Error opening socket: %s\n", zed_net_get_error());
		zed_net_socket_close(&intSock);
		zed_net_shutdown();
		return -1;
	}

	Sock = intSock;

	int hostIdx = atoi(strtok(NULL, ","));
	int portIdx = atoi(strtok(NULL, ","));
	char *host = menu[hostIdx].text;
	int port = atoi(menu[portIdx].text);

	if (zed_net_get_address(&server, host, port) != 0) { // Get the server's address
		printf("Error getting the server address: %s\n", zed_net_get_error());

		zed_net_socket_close(&Sock);
		zed_net_socket_close(&intSock);
		zed_net_shutdown();

		return -1;
	}

	printf("Client setup.\n");

	return 0;
}

int sendMsg(char *msg) {
	if (zed_net_udp_socket_send(&Sock, server, msg, strlen(msg)) != 0) { // Check it sent correctly
		printf("Send failed: %s\n", zed_net_get_error());
		return -1;
	}
	return 0;
}

void sendPos() {
	char toSend[256];
	sprintf(toSend, "MV:%lld:%d", MainPlayer.rect.x + worldScrollX, MainPlayer.rect.y);
	if (strcmp(toSend, lastSent) == 0) { // Don't send if it hasnt changed.
		return;
	}
	sendMsg(toSend);
	lastSent = strdup(toSend);
}

void connectToServer() {
	char JoinMsg[32];
	if (strlen(Settings.name) > 29) {
		printf("Username is too long. Max 29 chars.");
		return;
	}
	sprintf(JoinMsg, "JN:%s", Settings.name);
	if (sendMsg(JoinMsg) != 0) // Join the server and check it joined correctly
		printf("Connection failed.\n");
	char playerdata[256];
	int bytes = zed_net_udp_socket_receive(&Sock, &server, playerdata, sizeof(playerdata)); // Get starting position
	if (bytes == -1) {
		printf("Failed to get other player info: %s\n", zed_net_get_error());
		return;
	}
	if (strlen(playerdata) == 0) {
		printf("Recieved 0 bytes.\nData: %s\n", playerdata);
		return;
	}
	if (bytes == 0) {
		printf("Recieved 0 bytes.\nData: %s\n", playerdata);
		return;
	}
	int xpos = atoi(strtok(playerdata, ":"));
	int ypos = atoi(strtok(NULL, ":"));
	MainPlayer.worldRect.x = xpos;
	MainPlayer.worldRect.y = ypos; // Set the position
	if (xpos > width / 2) {
		worldScrollX = xpos - width / 2; // Change world scroll
	} else if (xpos < width / 2) {
		worldScrollX = 0;
	}
	MainPlayer.rect.x = xpos - worldScrollX;
	MainPlayer.rect.y = ypos;
	for (size_t x = 0; x < mapSize; x++) { // Update positions of all tiles
		map[x].rect.x = map[x].worldRect.x - worldScrollX;
	}
	for (size_t x = 0; x < numPlayers; x++) { // Update positions of all players
		players[x].rect.x = players[x].worldRect.x - worldScrollX;
	}
	return;
}

int getUpdates() {
	while (1) {
		char playerdata[256];
		int bytes = zed_net_udp_socket_receive(&Sock, &server, playerdata, sizeof(playerdata));
		if (bytes == -1) {
			printf("Failed to get other player info: %s\n", zed_net_get_error());
			return -1;
		}
		if (strlen(playerdata) == 0) {
			printf("Recieved 0 bytes.\nData: %s\n", playerdata);
			return -1;
		}
		if (bytes == 0) {
			printf("Recieved 0 bytes.\nData: %s\n", playerdata);
			return -1;
		}
		playerdata[bytes] = '\0';
		char *command = strtok(playerdata, ":");
		if (strcmp(command, "JN") == 0) { // If a new player joined
			char *ID = strtok(NULL, ":");
			char *name = strtok(NULL, ":");
			int xpos = atoi(strtok(NULL, ":"));
			int ypos = atoi(strtok(NULL, ":"));
			printf("Player `%s` joined the game.", name);
			players = realloc(players, ++numPlayers * sizeof(*players));
			struct OtherPlayer tmpPlayer;
			tmpPlayer.worldRect.x = xpos;
			tmpPlayer.worldRect.y = ypos;
			SDL_QueryTexture(playerTexture, NULL, NULL, &tmpPlayer.rect.w, &tmpPlayer.rect.h);
			tmpPlayer.rect.x = tmpPlayer.worldRect.x - worldScrollX;
			tmpPlayer.rect.y = tmpPlayer.worldRect.y;
			tmpPlayer.ID = strdup(ID);
			tmpPlayer.name = strdup(name);
			players[numPlayers - 1] = tmpPlayer;

		} else if (strcmp(command, "MV") == 0) { // If a player moved
			char *ID = strtok(NULL, ":");
			for (size_t x = 0; x < numPlayers; x++) {
				if (strcmp(players[x].ID, ID) == 0) {
					int xpos = atoi(strtok(NULL, ":"));
					int ypos = atoi(strtok(NULL, ":"));
					players[x].worldRect.x = xpos;
					players[x].worldRect.y = ypos;
					players[x].rect.x = xpos - worldScrollX;
					players[x].rect.y = ypos;
				}
			}

		} else if (strcmp(command, "DC") == 0) { // If a player disconnected
			char *ID = strtok(NULL, ":");
			for (size_t x = 0; x < numPlayers; x++) {
				if (strcmp(players[x].ID, ID) == 0) {
					for (size_t y = x; y < numPlayers; y++) {
						players[y] = players[y + 1];
					}
					numPlayers--;
					break;
				}
			}
		} else if (strcmp(command, "SCR") == 0) { // If a player started crouching
			char *ID = strtok(NULL, ":");
			for (size_t x = 0; x < numPlayers; x++) {
				if (strcmp(ID, players[x].ID) == 0) {
					players[x].crouching = true;
				}
			}
		} else if (strcmp(command, "ECR") == 0) { // If a player stopped crouching
			char *ID = strtok(NULL, ":");
			for (size_t x = 0; x < numPlayers; x++) {
				if (strcmp(ID, players[x].ID) == 0) {
					players[x].crouching = false;
				}
			}
		} else if (strcmp(command, "QUIT") == 0) { // If we have disconnected (From sending a "DC" request)
			zed_net_socket_close(&Sock);
			return 0;
		} else { // Command not recognised
			printf("Unknown command %s recieved.\n", command);
		}
	}
}

void updateMultiplayerNames() {
	// Updates the positions and names of other players
	textArrSize = numPlayers;
	textArr = realloc(textArr, textArrSize * sizeof(*textArr));
	for (size_t x = 0; x < textArrSize; x++) {
		textArr[x] = createText(players[x].name, 0, 0, playerFont); // Create the text
		textArr[x].rect.x = (players[x].rect.x + (players[x].rect.w / 2)) - (textArr[x].rect.w / 2); // Move it on the X
		textArr[x].rect.y = players[x].rect.y - textArr[x].rect.h - 5; // Move it on the Y
	}
}

int stayAlive() {
	while (1) {
		SDL_Delay(10000); // Wait 10 seconds
		if (sendMsg("PING") != 0) { // If the ping fails (presumably the socket is closed)
			return 0;
		}
	}
}