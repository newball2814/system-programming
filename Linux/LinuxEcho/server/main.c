#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_BUFFER_SIZE 80 

void listen_to_client(int connfd);

int main(int argc, char *argv[]) {
    int sockfd, connfd, len, port;
    struct sockaddr_in servaddr, client;

    if (argc != 2) {
        printf("Usage: %s [PORT]\n", argv[0]);
         exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);
    
    // Init socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    printf("Socket created successfully.\n");
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Failed to bind socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Bind successfully.\n");

    // Listen to client
    if (listen(sockfd, 5) != 0) {
        perror("Failed to listen to client.");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d.\n", port);

    // Loop to receive request til EOF
    while (1337) {
        len = sizeof(client);

        if ((connfd = accept(sockfd, (struct sockaddr *)&client, &len)) < 0) {
            perror("Failed to accept client.\n");
            continue;
        }
        printf("Accepted client!!\n");        
        
        // Create a new thread for each client connection
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, listen_to_client, (void *)&connfd) != 0) {
            perror("Failed to create thread.");
            close(connfd);
            continue;
        }

        printf("Closing connection...\n");                
        break;      // Have to add `break` cuz server get stuck on closing connection without exiting the program -_-
    }
    
    return EXIT_SUCCESS;
}

void *listen_to_client(void *arg) {
    int connfd = *((int *)arg);
    char buf[MAX_BUFFER_SIZE];
    int n;
    while (1337) {
        memset(buf, 0, MAX_BUFFER_SIZE);
        if ((n = read(connfd, buf, sizeof(buf))) <= 0) {
            perror("Error reading from client.");
            break;
        }

        printf("Received from client: %s", buf);
        write(connfd, buf, strlen(buf) + 1);
        memset(buf, 0, MAX_BUFFER_SIZE);
    }

    close(connfd);
    pthread_exit(NULL);
}
