//
// Created by Timothy Friberg Holmsten on 2016-10-25.
//
#include "headers/liarsgame.h"

void play(ThreadClient *tc, Client *client) {
    char text[1024];
    memset(text, 0, sizeof(text));
    while (recv(client->socket, &text, 1024, 0)) {
        if (strlen(text) > 0) {
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (i != client->id)
                    send(tc->server_data->clients[i], text, strlen(text), 0);
            }
            memset(text, 0, sizeof(text));
        }
    }
}
