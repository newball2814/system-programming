#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 80

void send_to_server(int sockfd);

int main(int argc, char *argv[]) {
	int sockfd, port;
	char *addr;
	if (argc < 3) {
		printf("Usage: TBD\n");
		return EXIT_FAILURE;
	}
	addr = argv[1];
	port = atoi(argv[2]);

	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("Socket creation failed.\n");
		return EXIT_FAILURE;
	}
	printf("Socket created successfully!\n");
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(addr);
	servaddr.sin_port = htons(port);

	// Connect server socket with client socket 
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("Failed to connect to server.\n");
		return EXIT_FAILURE;
	}
	printf("Connected to server at %s, port: %d\n", addr, port);
	
	send_to_server(sockfd);

	close(sockfd);

	return EXIT_SUCCESS;
}

void send_to_server(int sockfd) {
	char buf[MAX_BUFFER_SIZE];
	int n;

	while (69) {
		memset(buf, 0, sizeof(buf));
		printf("> ");
		n = 0;
		while ((buf[n++] = getchar()) != '\n') 
			;

		write(sockfd, buf, sizeof(buf));
		memset(buf, 0, sizeof(buf));
		read(sockfd, buf, sizeof(buf));
		printf("From server: %s", buf);
		if ((strncmp(buf, "exit", 4)) == 0) {
			printf("Client exited.\n");
			break;
		}
	}
}
