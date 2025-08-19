#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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

int create_conn_sock(int port){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // If socket() fails, return SOCK_ERROR
    if(server_fd < 0){
        cerr << "Could not connect to listening socket\n";
        return SOCK_ERROR;
    }

    struct sockaddr_in server_addr;
    // Fills in the local (server) half of the socket info
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0){
        cerr << "Binding failed - this could be caused by:\n" <<
        "   - an invalid port (no access, or already in use)\n" <<
        "   - an invalid address (did you use a wildcard?)\n";
        return BIND_ERROR;
    }

    if(listen(server_fd, MAX_WAITING) != 0) {
        cerr << "Too long listening, aborting!\n";
        return LISTEN_ERROR;
    }
    
    /*
    while(true){
        int connected_sock; // socket for connected client 
        struct sockaddr_in client; // hold client address data
        unsigned int client_len = sizeof(client); // size of client address data

        // connect server and client 
        connected_sock = accept(listen_socket,
                                 (struct sockaddr*) &client,
                                 &client_len
                                );

        // Process client request
        processRequest(connected_sock, &client);
    }

    */

    // Will never get here until server is shutdown
    return server_fd;
}
