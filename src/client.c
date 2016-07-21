//
// Created by Timothy Friberg Holmsten on 26/06/16.
//

#include "headers/client.h"

Client create_client(int id, int socket) {

    Client c;
    c.id = id;
    c.socket = socket;
    c.connected = 1;

    return c;
}

void init_clients(Client *clients, int n) {
    for (int i = 0; i < n; i++) {
        clients[i].id = 0;
        clients[i].connected = 0;
    }
}





