//
// Created by Timothy Friberg Holmsten on 30/05/16.
//

#include "headers/server.h"

int init_child_server(Server data) {

    int sockfd, clientfd;
    struct sockaddr_in self;
    struct sockaddr_in client_addr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
        printf("SO_REUSEADDR failed");

    memset(self.sin_zero, '\0', sizeof(self.sin_zero));
    self.sin_family = AF_INET;
    self.sin_port = htons(data.port);
    self.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY
    //self.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&self, sizeof(self));

    listen(sockfd, 4);

    data.running = 1;

    Client clients[MAX_CLIENTS];
    init_clients(clients, MAX_CLIENTS);

    pthread_t client_threads[MAX_CLIENTS];

    for (int i = 0; i < MAX_CLIENTS; i++)
        pthread_create(&client_threads[i], NULL, thread_client, &clients[i]);

    pthread_t r_thread, w_thread;
    Packet packet;
    packet = packet_nullify(packet);
    Packet *ptr_packet = &packet;

    Server *f = &data;

    ThreadServerComm thread_args;
    thread_args.server = f;
    thread_args.packet = ptr_packet;
    pthread_create(&r_thread, NULL, thread_read_server, &thread_args);
    pthread_create(&w_thread, NULL, thread_write_server, &thread_args);

    int n = 0;
    int r = 1;
    while (r)
    {
        addr_size = sizeof(client_addr);

        clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addr_size);

        if (clientfd != -1) {

            printf("Client Connected To Child Server %d! \n", data.server_id);

            clients[n++] = create_client(clientfd);

            data.nr_of_clients++;

            send_packet(11, data.server_id, data.nr_of_clients, data.fd_master[1]);

            printf("players: %d\n", data.nr_of_clients);


            //close(clientfd);
        }
    }

    //close(data.fd_master[0]);
    int running = 1;
    while (running)
    {
        char l[8];
        int b = 10*data.server_id;
        //write(data.fd_master[1], &b, sizeof(int));
        //printf("Server %d\n", data.nr_of_clients);
        //read(data.fd_child[0], &b, sizeof(int));
        recv(data.clients[1], &b, sizeof(int), NULL);
        printf("%d\n", b);
        usleep(100000);
        n++;
    }
    return 0;
}

void init_servers(Server *servers, int len, int port) {
    for (int i=0; i < len; i++)
    {
        pipe(servers[i].fd_master);
        pipe(servers[i].fd_child);
        servers[i].server_id = i;
        servers[i].nr_of_clients = 0;
        servers[i].running = 0;
        servers[i].port = port + (i+1);
    }
}

int calculate_best_server(Server *servers, int len) {
    int best_server = -1;
    int points = 0;
    int temp_points = 0;
    int clients = 0;
    int running = 0;

    for(int i=0; i < len; i++)
    {
        if (server_is_full(servers[i]))
            continue;

        clients = servers[i].nr_of_clients;
        running = servers[i].running;

        temp_points = points;
        //points = clients + running;
        points = clients;

        if (points == MAX_CLIENTS || running == 0)
            continue;

        if (points >= temp_points)
            best_server = servers[i].server_id;
    }

    return best_server;
}

void *thread_read_server(void *args) {
    ThreadServerComm *reader = (ThreadServerComm*) args;

    close(reader->server->fd_child[1]);

    int running = 1;
    while(running)
    {
        read(reader->server->fd_child[0], &reader->packet->data, sizeof(int)*PACKET_LENGTH);
        handle_packet(reader->packet, reader->server);

        usleep(16000);
    }

    return NULL;
}

void *thread_write_server(void *args) {
    ThreadServerComm *writer = (ThreadServerComm*) args;

    close(writer->server->fd_master[0]);

    int running = 1;
    while(running)
    {
        write(writer->server->fd_master[1], &writer->packet->data, sizeof(int)*PACKET_LENGTH);

        usleep(16000);
    }

    return NULL;
}

int server_is_full(Server server) {
    if (server.nr_of_clients >= MAX_CLIENTS)
        return 1;
    return 0;
}

int server_is_running(Server server) {
    if (server.running == 1)
        return 1;
    return 0;
}

int create_child_server(Server data) {
    switch (fork()) {
        case -1:
            printf("Could not create server\n");
            exit(-1);

        case 0:
            printf("Server Created\n");
            init_child_server(data);

            exit(0);
        default:
            usleep(16000);
    }
    return 0;
}

void *thread_client(void *args) {

    Client *c = (Client*)args;

    int running = 1;
    while(running)
    {
        usleep(16000);
    }

    return NULL;
}






















