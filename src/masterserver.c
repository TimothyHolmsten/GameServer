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

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
        printf("SO_REUSEADDR failed");

    memset(self.sin_zero, '\0', sizeof(self.sin_zero));
    self.sin_family = AF_INET;
    self.sin_port = htons(PORT);
    self.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY
    //self.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&self, sizeof(self));

    listen(sockfd, 5);

    int running = 1;
    int n = 0;

    while (running)
    {
        addr_size = sizeof(client_addr);

        clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addr_size);

        if (clientfd != -1) {

            printf("Client Connected!\n");


            switch (fork()) {
                case -1:
                    printf("Could not create server\n");
                    exit(-1);

                case 0:
                    printf("Server Created\n");
                    //close(server_list[n].fd_master[0]);
                    create_server(server_list[n]);

                    exit(0);

                default:
                    //close(server_list[n].fd_master[1]);
                    server_list[n].running = 1;
                    create_read_thread(server_list, NR_OF_SERVERS);
                    n++;
                    //wait(0);
                    //sleep(5);
                    //update_server_list(server_list, NR_OF_SERVERS);
                    //read(server_list[0].fd[0], &k, 5);
                    //printf("Server said: %s\n", k);
            }
            close(clientfd);
        }
    }
}

void sigchld_handler(int s)
{
    while(waitpid(-1,NULL,WNOHANG) > 0);
}

void init_child_handler() {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
}

void update_server_list(Server *server_list, int len) {
    for (int i = 0; i < len; i++)
    {
        printf("Server %d, running = %d\n", server_list[i].server_id, server_list[i].running);
        Server s;

        close(server_list[i].fd_master[1]);
        read(server_list[i].fd_master[0], &s, sizeof(s));

        server_list[s.server_id] = s;

        printf("Updated server %d, running = %d\n", server_list[i].server_id, server_list[i].running);
    }
}

void *thread_read_servers(void *s)
{
    ThreadComm *reader = (ThreadComm*) s;
    Packet packet;

    int running = 1;
    while(running) {
        for (int i = 0; i < reader->length; i++) {
            packet = packet_nullify(packet);

            if (reader->server_list[i].running == 0)
                continue;

            read(reader->server_list[i].fd_master[0], &packet.data, sizeof(int)*PACKET_LENGTH);
            handle_packet(&packet, &reader->server_list[i]);

            usleep(16000);
        }
    }
    return NULL;
}

void create_read_thread(Server *server_list, int len)
{
    pthread_t r_thread;

    ThreadComm reader;
    reader.id = 1;
    reader.length = len;

    for (int i = 0; i < len; i++)
        reader.server_list[i] = server_list[i];

    pthread_create(&r_thread, NULL, thread_read_servers, &reader);

}