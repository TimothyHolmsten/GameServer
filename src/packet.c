//
// Created by Timothy Friberg Holmsten on 08/06/16.
//

#include "headers/packet.h"

Packet packet_nullify(Packet packet) {
    for (int i = 0; i < PACKET_LENGTH; i++)
    {
        packet.data[i] = 0;
    }
    return packet;
}

int handle_packet(Packet *packet, Server *server) {
    int type = packet->data[0];

    // No message
    if(type == 0)
        return 0;

    // Get/Set nr_of_clients
    if(type == 10)
        return packet_get_nr_of_clients(packet, server);
    if(type == 11)
        return packet_set_nr_of_clients(packet, server);

    // Get/Set server running
    if(type == 20)
        return packet_get_running(packet, server);
    if(type == 21)
        return packet_set_running(packet, server);

    // Player joined
    if(type == 30)
        return packet_client_joined(packet, server);

    return -1;
}

int packet_client_joined(Packet *packet, Server *server)
{
    packet->data[0] = 0;
    server->clients[server->nr_of_clients] = packet->data[2];
    server->nr_of_clients++;
    return 0;
}

int packet_get_nr_of_clients(Packet *packet, Server *server) {
    packet->data[0] = 11;
    packet->data[1] = server->server_id;
    packet->data[2] = server->nr_of_clients;
    return 0;
}

int packet_set_nr_of_clients(Packet *packet, Server *server) {
    server->nr_of_clients = packet->data[2];
    packet->data[0] = 0;
    return 0;
}

int packet_get_running(Packet *packet, Server *server) {
    packet->data[0] = 21;
    packet->data[1] = server->server_id;
    packet->data[2] = server->running;
    return 0;
}

int packet_set_running(Packet *packet, Server *server) {
    server->running = packet->data[2];
    packet->data[0] = 0;
    return 0;
}







