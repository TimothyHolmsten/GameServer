//
// Created by Timothy Friberg Holmsten on 29/05/16.
//

#ifndef GAMESERVER_MASTERSERVER_H
#define GAMESERVER_MASTERSERVER_H

#include "server.h"
#include <sys/socket.h>
#include <memory.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

#define NR_OF_SERVERS 10
#define PORT 5000

void sigchld_handler(int s);
void init_child_handler();

void master_server();

#endif //GAMESERVER_MASTERSERVER_H
