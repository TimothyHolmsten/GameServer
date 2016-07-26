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

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)) < 0)
        printf("SO_REUSEADDR failed");

    memset(self.sin_zero, '\0', sizeof(self.sin_zero));
    self.sin_family = AF_INET;
    self.sin_port = htons(data.port);
    self.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY
    //self.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *) &self, sizeof(self));

    listen(sockfd, MAX_CLIENTS);

    data.running = 1;

    Client clients[MAX_CLIENTS];
    init_clients(clients, MAX_CLIENTS);
    // Creating client threads
    // TODO Make client threads better
    pthread_t client_threads[MAX_CLIENTS];

    ThreadClient tc;
    tc.counter = 0;
    tc.id = 0;
    tc.server_data = &data;
    pthread_mutex_init(&tc.mutex_lock, NULL);
    /*
    for (int i = 0; i < MAX_CLIENTS; i++) {
        tc.client = &clients[i];
        //pthread_create(&client_threads[i], NULL, thread_client, &clients[i]);
        pthread_create(&client_threads[i], NULL, thread_client, &tc);
        usleep(16000);
    }*/

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

    int r = 1;
    while (r) {
        addr_size = sizeof(client_addr);

        clientfd = accept(sockfd, (struct sockaddr *) &client_addr, &addr_size);

        if (clientfd != -1) {

            pthread_mutex_lock(&tc.mutex_lock);

            // Locks other connections out
            data.ready_for_new_client = 0;
            send_packet(41, data.server_id, data.ready_for_new_client, data.fd_master[1]);

            int free_spot = find_free_client(clients, MAX_CLIENTS);

            if (free_spot < 0) {
                printf("Cannot find a free spot for the client\n");
                data.ready_for_new_client = 1;
                send_packet(41, data.server_id, data.ready_for_new_client, data.fd_master[1]);
                pthread_mutex_unlock(&tc.mutex_lock);
                continue;
            }
            clients[free_spot] = create_client(free_spot, clientfd);
            data.clients[free_spot] = clients[free_spot].socket;

            tc.client = &clients[free_spot];
            // Creating client read thread
            pthread_create(&client_threads[free_spot], NULL, thread_client, &tc);

            data.nr_of_clients++;

            send_packet(11, data.server_id, data.nr_of_clients, data.fd_master[1]);
            //printf("Server %d players: %d\n", data.server_id, data.nr_of_clients);
            // Open up for other connections
            data.ready_for_new_client = 1;
            send_packet(41, data.server_id, data.ready_for_new_client, data.fd_master[1]);

            pthread_mutex_unlock(&tc.mutex_lock);

            if (data.nr_of_clients > MAX_CLIENTS) {
                printf("OVERFULL\n");
                exit(-1);
            }
        }
    }

    return 0;
}

void init_servers(Server *servers, int len, int port) {
    for (int i = 0; i < len; i++) {
        pipe(servers[i].fd_master);
        pipe(servers[i].fd_child);
        servers[i].server_id = i;
        servers[i].nr_of_clients = 0;
        servers[i].running = 0;
        servers[i].port = port + (i + 1);
        servers[i].ready_for_new_client = 1;
    }
}

int calculate_best_server(Server *servers, int len) {
    int best_server = -1;
    int points = 0;
    int temp_points = 0;
    int clients = 0;
    int running = 0;
    int ready = 0;

    for (int i = 0; i < len; i++) {
        send_packet(40, i, 0, servers[i].fd_child[1]);

        if (server_is_full(servers[i]))
            continue;

        clients = servers[i].nr_of_clients;
        running = servers[i].running;
        ready = servers[i].ready_for_new_client;

        temp_points = points;
        //points = clients + running;
        points = clients;

        if (points == MAX_CLIENTS || running == 0 || ready == 0)
            continue;

        if (points >= temp_points)
            best_server = servers[i].server_id;
    }

    return best_server;
}

void *thread_read_server(void *args) {
    ThreadServerComm *reader = (ThreadServerComm *) args;

    close(reader->server->fd_child[1]);

    int running = 1;
    while (running) {
        read(reader->server->fd_child[0], &reader->packet->data, sizeof(int) * COMMUNICATION_LENGTH);
        handle_communication(reader->packet, reader->server);

        //usleep(16000);
    }

    return NULL;
}

void *thread_write_server(void *args) {
    ThreadServerComm *writer = (ThreadServerComm *) args;

    close(writer->server->fd_master[0]);

    int running = 1;
    while (running) {
        if (writer->packet->data[0] != 0) // If there actually is a packet to send
        {
            write(writer->server->fd_master[1], &writer->packet->data, sizeof(int) * COMMUNICATION_LENGTH);
        }
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
            init_child_server(data);

            exit(0);
        default:
            usleep(16000);
    }
    return 0;
}

int find_free_client(Client *clients, int len) {

    for (int i = 0; i < len; i++)
        if (clients[i].connected == 0)
            return i;

    return -1;
}

void *thread_client(void *args) {

    //Client *c = (Client *) args;
    ThreadClient *tc = (ThreadClient *) args;

    pthread_mutex_lock(&tc->mutex_lock);
    int my_id = tc->id;
    Client *my_client = tc->client;
    tc->id++;
    pthread_mutex_unlock(&tc->mutex_lock);

    char text[1024];
    memset(text, 0, sizeof(text));

    // While the client is connected do this
    while (recv(my_client->socket, &text, 1024, 0)) {
        if (strlen(text) > 0) {
            for(int i = 0; i < MAX_CLIENTS; i++)
                send(tc->server_data->clients[i], text, strlen(text), 0);

            memset(text, 0, sizeof(text));
        }
    }

    pthread_mutex_lock(&tc->mutex_lock);
    // Client disconnected
    my_client->connected = 0;
    close(my_client->socket);
    my_client->socket = -1;
    tc->server_data->nr_of_clients--;
    // Update masterserver with how many clients there are on the server
    send_packet(11,
                tc->server_data->server_id,
                tc->server_data->nr_of_clients,
                tc->server_data->fd_master[1]
    );

    pthread_mutex_unlock(&tc->mutex_lock);

    pthread_exit(NULL);

    return NULL;
}






















