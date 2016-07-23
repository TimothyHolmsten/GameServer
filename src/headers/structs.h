//
// Created by Timothy Friberg Holmsten on 09/06/16.
//

#ifndef GAMESERVER_STRUCTS_H
#define GAMESERVER_STRUCTS_H

#include "defines.h"
#include <pthread.h>

typedef struct Packet {
    int data[COMMUNICATION_LENGTH];
} Packet;

typedef struct Server {
    int fd_master[2];
    int fd_child[2];
    int sockfd, masterfd;
    int port;
    int clients[MAX_CLIENTS];
    int nr_of_clients;
    int server_id;
    int running;
    int ready_for_new_client;
} Server;

typedef struct Client {
    int id;
    int connected;
    int socket;
} Client;

typedef struct ThreadClient {
    Client *client;
    int counter;
    pthread_mutex_t mutex_lock;
    int id;
    char broadcast[1024];
    Server *server_data;
} ThreadClient;

typedef struct ThreadServerComm {
    Server *server;
    Packet *packet;
} ThreadServerComm;

typedef struct ThreadComm {
    Server *server_list;
    int id;
} ThreadComm;

#endif //GAMESERVER_STRUCTS_H
