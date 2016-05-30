//
// Created by Timothy Friberg Holmsten on 29/05/16.
//
#include "headers/masterserver.h"


void master_server() {

    Server server_list[NR_OF_SERVERS];

    init_servers(server_list, NR_OF_SERVERS);
    init_child_handler();

    int sockfd, clientfd;
    struct sockaddr_in self;
    struct sockaddr_in client_addr;
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(self.sin_zero, NULL, sizeof(self.sin_zero));
    self.sin_family = AF_INET;
    self.sin_port = htons(PORT);
    self.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY

    bind(sockfd, (struct sockaddr*)&self, sizeof(self));

    listen(sockfd, 5);

    int running = 1;

    while (running)
    {
        addr_size = sizeof(client_addr);

        clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addr_size);

        if (clientfd != -1)
        {
            close(clientfd);
            continue;
        }
        printf("Client Connected!\n");

        /*
        switch (fork())
        {
            case -1:
                printf("Could not create server\n");
                exit(-1);

            case 0:
                printf("Server Created %d!\n", server_list[0].server_id);

        }*/

        sleep(1);
        uint32_t info;
        char buf[8];
        memset(buf,0, sizeof(buf));
        //printf("%d\n", ntohl(info));
        recv(clientfd, &buf, 8, 0);
        //recv(clientfd, &info, sizeof(uint32_t), 0);

        //printf("%s\n", buf);
        printf("%d\n", buf[0]);

        close(clientfd);
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



