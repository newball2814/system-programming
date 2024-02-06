#include <iostream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock." << endl;
        return 1;
    }

    // Create a socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        cerr << "Failed to create socket." << endl;

        WSACleanup();
        return 1;
    }

    // Set up the server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1337); 		 // Port
    serverAddr.sin_addr.s_addr = INADDR_ANY; // IP

    // Bind the socket to the server address
    if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Failed to bind socket." << endl;

        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Failed to listen for incoming connections." << endl;

        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Accept an incoming connection
    SOCKET clientSock = accept(sock, NULL, NULL);
    if (clientSock == INVALID_SOCKET) {
        cerr << "Failed to accept incoming connection." << endl;

        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Buffer to store incoming data
    char buffer[4096];

    // Infinite loop to handle incoming data
    while (true) {
        // Receive incoming data
        int bytesReceived = recv(clientSock, buffer, sizeof(buffer), 0);

        if (bytesReceived == SOCKET_ERROR) {
            cerr << "Failed to receive incoming data." << endl;

            closesocket(clientSock);
            closesocket(sock);
            WSACleanup();
            return 1;
        }

       if (send(clientSock, buffer, bytesReceived, 0) == SOCKET_ERROR) {
            cerr << "Failed to send data back to the client." << endl;
            closesocket(clientSock);
            closesocket(sock);
            WSACleanup();
            return 1;
        }
    }

    // Close the sockets
    closesocket(clientSock);
    closesocket(sock);

    // Clean up Winsock
    WSACleanup();

    return 0;
}
