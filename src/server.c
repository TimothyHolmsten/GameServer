//
// Created by Timothy Friberg Holmsten on 30/05/16.
//

#include "headers/server.h"

int create_server(Server data) {
    /*
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
    */
    //printf("Hello from server %d\n", data.server_id);
    data.running = 2;
    /*write(data.fd[1], "Hej!", 4);
    sleep(4);
    write(data.fd[1], "exit", 4);*/
    int n = 0;
    while (1)
    {
        char l[8];
        int b = 10*data.server_id;
        sprintf(l, "%d\0", getpid());
        write(data.fd[1], &b, sizeof(int));
        sleep(2);
        n++;
    }

    return 0;
}

void init_servers(Server *servers, int len, int port) {
    for (int i=0; i < len; i++)
    {
        pipe(servers[i].fd);
        servers[i].server_id = i;
        servers[i].nr_of_clients = 0;
        servers[i].running = 0;
        servers[i].port = port + (i+1);
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

        if (points >= temp_points)
            best_server = servers[i].server_id;
    }

    return best_server;
}

void create_comm_threads(Server server) {
    pthread_t r_thread;

    ThreadServerComm reader;
    reader.server = server;

    pthread_create(&r_thread, NULL, thread_read_server, &reader);

}

void *thread_read_server(void *s) {


    return NULL;
}









