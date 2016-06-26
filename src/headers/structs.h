//
// Created by Timothy Friberg Holmsten on 09/06/16.
//

#ifndef GAMESERVER_STRUCTS_H
#define GAMESERVER_STRUCTS_H

#include "defines.h"

typedef struct Packet{
    int data[PACKET_LENGTH];
}Packet;

typedef struct Server{
    int fd_master[2];
    int fd_child[2];
    int sockfd, masterfd;
    int port;
    int clients[MAX_CLIENTS];
    int nr_of_clients;
    int server_id;
    int running;
}Server;

typedef struct Client{
    int id;
    int connected;
}Client;

typedef struct ThreadServerComm {
    Server *server;
    Packet *packet;
} ThreadServerComm;

typedef struct ThreadComm{
    Server *server_list;
    int id;
}ThreadComm;

#endif //GAMESERVER_STRUCTS_H
