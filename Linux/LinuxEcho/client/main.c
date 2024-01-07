#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 80

void send_to_server(int sockfd);

int main(int argc, char *argv[]) {
	int sockfd, port;
	char *addr;
	struct sockaddr_in servaddr;

	if (argc < 3) {
		printf("Usage: %s [ADDR] [PORT]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	addr = argv[1];
	port = atoi(argv[2]);

    // Init socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("Socket creation failed.\n");
		exit(EXIT_FAILURE);
	}

	printf("Socket created successfully!\n");
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(addr);
	servaddr.sin_port = htons(port);

	// Connect server socket with client socket 
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("Failed to connect to server.\n");
		exit(EXIT_FAILURE);
	}
	printf("Connected to server at %s, port: %d\n", addr, port);
	
	send_to_server(sockfd);

	close(sockfd);
	return EXIT_SUCCESS;
}

void send_to_server(int sockfd) {
	char buf[MAX_BUFFER_SIZE];
	int n;

	while (1337) {
		memset(buf, 0, sizeof(buf));
		printf("> ");
		n = 0;
        // Read stdin
		while ((buf[n++] = getchar()) != '\n') 
			;

		write(sockfd, buf, sizeof(buf));
		memset(buf, 0, sizeof(buf));

        // Acknowledge server's message
		read(sockfd, buf, sizeof(buf));
		printf("Message server received: %s", buf);

		if (strncmp(buf, "EXIT", 4) == 0) {
			printf("Client exited.\n");
			break;
		}
	}
}
