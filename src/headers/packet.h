//
// Created by Timothy Friberg Holmsten on 08/06/16.
//


#ifndef GAMESERVER_PACKET_H
#define GAMESERVER_PACKET_H

#include "structs.h"
#include <stdio.h>

Packet packet_nullify(Packet packet);
int handle_packet(Packet *packet, Server *server);

int packet_get_nr_of_clients(Packet *packet, Server *server);
int packet_set_nr_of_clients(Packet *packet, Server *server);

int packet_get_running(Packet *packet, Server *server);
int packet_set_running(Packet *packet, Server *server);

#endif //GAMESERVER_PACKET_H
