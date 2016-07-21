//
// Created by Timothy Friberg Holmsten on 26/06/16.
//

#ifndef GAMESERVER_CLIENT_H
#define GAMESERVER_CLIENT_H

#include "structs.h"

Client create_client(int id, int socket);

void init_clients(Client *clients, int n);

#endif //GAMESERVER_CLIENT_H
