#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 80 

void listen_to_client(int connfd);

int main(int argc, char *argv[]) {
	int sockfd, connfd, len, port;
	struct sockaddr_in servaddr, client;

	if (argc == 0) {
		printf("Please provide port.\n");
		return EXIT_FAILURE;
	}

	port = atoi(argv[1]);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("Failed to create socket.\n");
		return EXIT_FAILURE;
	}

	printf("Socket created successfully.\n");
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if ((bind(sockfd, (struct addr*)&servaddr, sizeof(servaddr))) != 0) {
		printf("Failed to bind socket.\n");
		return EXIT_FAILURE;
	}
	printf("%d", sockfd);
	printf("Bind successfully.\n");

	if ((listen(sockfd, 5)) != 0) {
		printf("Failed to listen.\n");
		return EXIT_FAILURE;
	}
	printf("Server listening on port %s\n", port);


	len = sizeof(client);
	connfd = accept(sockfd, (struct sockaddr *)&client, &len);
	if (connfd < 0) {
		printf("Failed to accept client.\n");
		return EXIT_FAILURE;
	}
	printf("Accepted client!!\n");

	listen_to_client(connfd);

	close(sockfd);

	return EXIT_SUCCESS;
}

void listen_to_client(int connfd) {
	char buf[MAX_BUFFER_SIZE];
	int n;

	while (69) {
		memset(buf, 0, MAX_BUFFER_SIZE);
		read(connfd, buf, sizeof(buf));
		printf("From client: %s\nTo client: ", buf);
		memset(buf, 0, MAX_BUFFER_SIZE);
		n = 0;
		while ((buf[n++] = getchar()) != '\n')
			;

		write(connfd, buf, sizeof(buf));

		if (strncmp("exit", buf, 4) == 0) {
			printf("Server closing...\n");
			break;
		}
	}
}
