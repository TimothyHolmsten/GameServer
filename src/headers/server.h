//
// Created by Timothy Friberg Holmsten on 30/05/16.
//

#ifndef GAMESERVERTEST_SERVER_H
#define GAMESERVERTEST_SERVER_H

#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "packet.h"

#define MAX_CLIENTS 4

typedef struct Server{
    int fd[2];
    int sockfd, masterfd;
    int port;
    int clients[MAX_CLIENTS];
    int nr_of_clients;
    int server_id;
    int running;
}Server;

typedef struct ThreadServerComm {
    Server server;
    Packet packet;
} ThreadServerComm;

int create_server(Server data);
void init_servers(Server *servers, int len, int port);
int calculate_best_server(Server * servers, int len);
void create_comm_threads(Server server);
void *thread_read_server(void *s);

#endif //GAMESERVERTEST_SERVER_H
