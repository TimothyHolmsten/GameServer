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
    data.running = 1;
    /*write(data.fd[1], "Hej!", 4);
    sleep(4);
    write(data.fd[1], "exit", 4);*/
    int n = 0;
    //create_comm_threads(&data);

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

    //close(data.fd_master[0]);

    int running = 1;
    while (running)
    {
        char l[8];
        int b = 10*data.server_id;
        //write(data.fd_master[1], &b, sizeof(int));
        printf("Server %d\n", data.server_id);
        //read(data.fd_child[0], &b, sizeof(int));
        sleep(2);
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

void create_comm_threads(Server *server) {
    pthread_t r_thread;

    Packet *packet;
    //packet_nullify(packet);

    ThreadServerComm reader;
    reader.server = server;
    //reader.packet = packet;
    pthread_create(&r_thread, NULL, thread_read_server, &reader);
}

void *thread_read_server(void *s) {
    ThreadServerComm *reader = (ThreadServerComm*) s;

    close(reader->server->fd_child[1]);

    int running = 1;
    while(running)
    {
        read(reader->server->fd_child[0], &reader->packet->data, sizeof(int)*PACKET_LENGTH);
        handle_packet(reader->packet, reader->server);

        //sleep(1);
        usleep(16000);
    }

    return NULL;
}

void *thread_write_server(void *s) {
    ThreadServerComm *writer = (ThreadServerComm*) s;

    close(writer->server->fd_master[0]);

    int running = 1;
    while(running)
    {
        /*
        writer->packet->data[0] = 0;
        writer->packet->data[1] = 100;
        */
        write(writer->server->fd_master[1], &writer->packet->data, sizeof(int)*PACKET_LENGTH);

        //sleep(1);
        usleep(16000);
    }

    return NULL;
}

int handle_packet(Packet *packet, Server *server) {
    int type = packet->data[0];
    int server_id = packet->data[1];

    if(type == 0)
        return 0;

    if(type == 10)
    {
        packet->data[0] = 11;
        return 0;
    }
    if(type == 11)
    {
        packet->data[0] = 0;
        packet->data[1] = 1337;
        server->nr_of_clients = 10;
        return 0;
    }

    return -1;
}













