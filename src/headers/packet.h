//
// Created by Timothy Friberg Holmsten on 08/06/16.
//


#ifndef GAMESERVER_PACKET_H
#define GAMESERVER_PACKET_H

#include "structs.h"
#include <stdio.h>
#include <unistd.h>

Packet packet_nullify(Packet packet);

int handle_packet(Packet *packet, Server *server);

int packet_get_nr_of_clients(Packet *packet, Server *server);

int packet_set_nr_of_clients(Packet *packet, Server *server);

int packet_get_running(Packet *packet, Server *server);

int packet_set_running(Packet *packet, Server *server);

int packet_client_joined(Packet *packet, Server *server);

int packet_get_server_ready_for_new_client(Packet *packet, Server *server);

int packet_set_server_ready_for_new_client(Packet *packet, Server *server);

void send_packet(int packet, int server_id, int opt, int fd);

#endif //GAMESERVER_PACKET_H
