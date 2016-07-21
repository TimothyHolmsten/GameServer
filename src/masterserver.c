//
// Created by Timothy Friberg Holmsten on 29/05/16.
//
#include "headers/masterserver.h"

void master_server() {

    Server server_list[NR_OF_SERVERS];

    init_servers(server_list, NR_OF_SERVERS, PORT);
    init_child_handler();

    int sockfd, clientfd;
    struct sockaddr_in self;
    struct sockaddr_in client_addr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)) < 0)
        printf("SO_REUSEADDR failed");

    memset(self.sin_zero, '\0', sizeof(self.sin_zero));
    self.sin_family = AF_INET;
    self.sin_port = htons(PORT);
    self.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY
    //self.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *) &self, sizeof(self));

    listen(sockfd, NR_OF_SERVERS * MAX_CLIENTS);

    pthread_mutex_t lock;

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init failed\n");
        return;
    }

    pthread_t reading_threads[NR_OF_SERVERS];
    ThreadComm readers[NR_OF_SERVERS];
    for (int i = 0; i < NR_OF_SERVERS; i++) {
        readers[i].id = i;
        Server *list = server_list;
        readers[i].server_list = list;

        pthread_create(&reading_threads[i], NULL, thread_read_servers, &readers[i]);
    }

    int running = 1;
    while (running) {
        usleep(10000);

        addr_size = sizeof(client_addr);

        clientfd = accept(sockfd, (struct sockaddr *) &client_addr, &addr_size);

        if (clientfd != -1) {

            //printf("Client Connected!\n");

            redirect_new_client(clientfd, server_list);
            close(clientfd);
        }
    }
}

int redirect_new_client(int clientfd, Server *server_list) {
    int server = calculate_best_server(server_list, NR_OF_SERVERS);
    int start_server = -1;

    if (server != -1) {
        send(clientfd, &server_list[server].port, sizeof(int), 0);

        return 0;
    }
    else // Start a server
    {
        for (int i = 0; i < NR_OF_SERVERS; i++) {
            if (server_is_running(server_list[i]))
                continue;

            start_server = i;
            server_list[i].running = 1;
            server_list[i].clients[0] = clientfd;
            server_list[i].nr_of_clients = 0;
            create_child_server(server_list[i]);
            send(clientfd, &server_list[i].port, sizeof(int), 0);
            break;
        }
    }
    return start_server;
}

void sigchld_handler(int s) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void init_child_handler() {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

void update_server_list(Server *server_list, int len) {
    for (int i = 0; i < len; i++) {
        printf("Server %d, running = %d\n", server_list[i].server_id, server_list[i].running);
        Server s;

        close(server_list[i].fd_master[1]);
        read(server_list[i].fd_master[0], &s, sizeof(s));

        server_list[s.server_id] = s;

        printf("Updated server %d, running = %d\n", server_list[i].server_id, server_list[i].running);
    }
}

void *thread_read_servers(void *s) {
    ThreadComm *reader = (ThreadComm *) s;
    Packet packet;

    int running = 1;
    while (running) {
        packet = packet_nullify(packet);
        if (reader->server_list[reader->id].running == 0) {
            usleep(16000);
            continue;
        }

        read(reader->server_list[reader->id].fd_master[0], &packet.data, sizeof(int) * PACKET_LENGTH);
        handle_packet(&packet, &reader->server_list[reader->id]);

        //usleep(16000);
        //printf("%d\n", packet.data[0]);
    }
    return NULL;
}