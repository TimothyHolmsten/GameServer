//
// Created by Timothy Friberg Holmsten on 30/05/16.
//

#include "headers/server.h"

int init_child_server(Server data) {

    data.running = 1;
    int n = 0;

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
        printf("Server %d\n", data.nr_of_clients);
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
    int best_server = -1;
    int points = 0;
    int temp_points = 0;
    int clients = 0;
    int running = 0;

    for(int i=0; i < len; i++)
    {
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

void *thread_read_server(void *s) {
    ThreadServerComm *reader = (ThreadServerComm*) s;

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

void *thread_write_server(void *s) {
    ThreadServerComm *writer = (ThreadServerComm*) s;

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
    if (server.nr_of_clients == MAX_CLIENTS)
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




















