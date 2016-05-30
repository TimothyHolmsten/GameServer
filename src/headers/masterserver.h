//
// Created by Timothy Friberg Holmsten on 29/05/16.
//

#ifndef GAMESERVER_MASTERSERVER_H
#define GAMESERVER_MASTERSERVER_H

#define NR_OF_SERVERS 10
#define PORT 5000

void sigchld_handler(int s);
void init_child_handler();
void master_server();

#endif //GAMESERVER_MASTERSERVER_H
