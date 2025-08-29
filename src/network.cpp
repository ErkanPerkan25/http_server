#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "network.hpp"

using namespace std;

#define USAGE_ERROR 1
#define SOCK_ERROR  2
#define BIND_ERROR  3
#define LISTEN_ERROR 4

#define MAX_WAITING 25
#define PORT 8080

int create_conn_sock(){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // If socket() fails, return SOCK_ERROR
    if(server_fd < 0){
        cerr << "Could not connect to listening socket\n";
        return SOCK_ERROR;
    }

    struct sockaddr_in server_addr;
    // Fills in the local (server) half of the socket info
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0){
        cerr << "Binding failed - this could be caused by:\n" <<
        "- an invalid port (no access, or already in use)\n" <<
        "- an invalid address (did you use a wildcard?)\n";
        return BIND_ERROR;
    }

    if(listen(server_fd, MAX_WAITING) != 0) {
        cerr << "Too long listening, aborting!\n";
        return LISTEN_ERROR;
    }

    cout << "listening from " << inet_ntoa(server_addr.sin_addr) << ":" << PORT << endl;
    
    // Will never get here until server is shutdown
    return server_fd;
}
