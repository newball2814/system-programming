#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

void *handleClient(int *arg) {
    int clientSocket = *arg;
    char buffer[256];
    int n;

    // Communication with client 
    while ((n = read(clientSocket, buffer, sizeof(buffer))) > 0) {
        write(clientSocket, buffer, n);
    }

    if (n < 0) {
        cerr << "Error reading from socket." << endl;
    }

    close(clientSocket);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    int clientSocket, serverSocket, port;
    struct sockaddr_in serverAddr, clientAddr;
    
    if (argc != 1) {
        cout << "Usage: ./serv [PORT]" << endl;
        return EXIT_FAILURE;
    }
    
    // Create server socket 
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Error opening socket." << endl;
        return EXIT_FAILURE;
    }

    port = atoi(argv[2]);
    
    
    return EXIT_SUCCESS;
}
