//
// Created by Timothy Friberg Holmsten on 30/05/16.
//

#include "headers/server.h"


int create_server(Server data) {

    int sockfd, clientfd;
    struct sockaddr_in self;
    struct sockaddr_in client_addr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(self.sin_zero, NULL, sizeof(self.sin_zero));
    self.sin_family = AF_INET;
    self.sin_port = htons(data.port);
    self.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY

    bind(sockfd, (struct sockaddr*)&self, sizeof(self));

    listen(sockfd, MAX_CLIENTS);



    return 0;
}

void init_servers(Server *servers, int len) {
    for (int i=0; i < len; i++)
    {
        servers[i].server_id = i;
        servers[i].nr_of_clients = 0;
        servers[i].running = 0;
    }
}

int calculate_best_server(Server *servers, int len) {
    int best_server = 0;
    int points = 0;
    int temp_points = 0;
    int players = 0;
    int running = 0;

    for(int i=0; i < len; i++)
    {
        players = servers[i].nr_of_clients;
        running = servers[i].running;

        temp_points = points;
        points = players + running;

        if (players == MAX_CLIENTS || points == MAX_CLIENTS + 1)
            continue;

        if (points > temp_points)
            best_server = servers[i].server_id;
    }

    return best_server;
}





