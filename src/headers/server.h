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
#include "communication.h"
#include "structs.h"
#include "client.h"

int init_child_server(Server data);

int create_child_server(Server data);

void init_servers(Server *servers, int len, int port);

int calculate_best_server(Server *servers, int len);

int server_is_full(Server server);

int server_is_running(Server server);

int find_free_client(Client *clients, int len);

void *thread_read_server(void *args);

void *thread_write_server(void *args);

void *thread_client(void *args);

#endif //GAMESERVERTEST_SERVER_H