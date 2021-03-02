#define ZED_NET_IMPLEMENTATION
#include <zed_net.h>
#include "main.h"

struct Player *players = NULL;
size_t numPlayers = 0;

zed_net_socket_t Socket;

void handleRequest(char *data, zed_net_address_t addr) {
	char *command = strtok(data, ":");
	if (strcmp(command, "DC") == 0) { // Disconnect
		size_t index;
		for (size_t x = 0; x < numPlayers; x++) {
			if (players[x].ip = addr.host) { // Find the current player's index
				index = x;
				break;
			}
		}
		for (size_t x = index; x < numPlayers; x++) {
			players[x] = players[x + 1]; // Remove it and move everything else
		}
		numPlayers -= 1;
		return;
	} else if (strcmp(command, "JN") == 0) { // Join
		players = realloc(players, ++numPlayers * sizeof(*players));
		struct Player tmpPlayer;
		tmpPlayer.x = 0;
		tmpPlayer.y = 0;
		itoa(numPlayers - 1, tmpPlayer.ip, 10);
		tmpPlayer.ip = addr.host;
		players[numPlayers - 1] = tmpPlayer;
		return;
	} else if (strcmp(command, "REQ") == 0) {
		char *tmp;
		zed_net_udp_socket_send(&Socket, addr, itoa(numPlayers, tmp, 10), strlen(itoa(numPlayers, tmp, 10)));
		for (size_t x = 0; x < numPlayers; x++) {
			if (players[x].ip != addr.host) {
				char *toSend;
				strcat(toSend, players[x].ID);
				strcat(toSend, ",");
				strcat(toSend, itoa(players[x].x, tmp, 10));
				strcat(toSend, ",");
				strcat(toSend, itoa(players[x].y, tmp, 10));
				zed_net_udp_socket_send(&Socket, addr, toSend, strlen(toSend));
			}
		}
	} else if (strcmp(command, "MV") == 0) {
		char *xpos = strtok(NULL, ":");
		char *ypos = strtok(NULL, ":");
		for (size_t x = 0; x < numPlayers; x++) {
			if (players[x].ip = addr.host) { // Find the current player's index
				players[x].x = atoi(xpos);
				players[x].y = atoi(ypos);
				break;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	if (zed_net_init() != 0) {
		printf("Failed to initialise zed_net: %s\n", zed_net_get_error());
		return 1;
	}
	int rc = zed_net_udp_socket_open(&Socket, 12345, 0);
	if (rc == -1) {
		printf("Error creating socket: %s\n", zed_net_get_error());
		return 1;
	}
	printf("Bound to port 12345 and listening.\n");
	while (1) {
		zed_net_address_t addr;
		char data[256];
		if (zed_net_udp_socket_receive(&Socket, &addr, data, 2048) != -1) {
			printf("Recieved from `%s`\n", zed_net_host_to_str(addr.host));
			handleRequest(data, addr);
		} else {
			printf("An error occursed recieving data: %s\n", zed_net_get_error());
		}
	}
	zed_net_shutdown();
	return 0;
}