#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "network.hpp"
#include "handle_client.hpp"

using namespace std;

#define USAGE_ERROR 1

// Server (socket) file descriptor
int servers_fd;

int main(int argc, char* argv[]){

    if(!argv[1]){
        cerr << "USAGE: " << argv[0] << " <port number>\n";
        return USAGE_ERROR;
    }
    
    // If can't create a connection socket, return error
    if((servers_fd = create_conn_sock(stoi(argv[1]))) < 0){
        cerr << "Error: Could not create server socket." << endl;
        return 1;
    }

    // Client address information
    struct sockaddr_in client_addr {};

    while(true){
        socklen_t sock_size = sizeof(client_addr);
        int client_sock = accept(servers_fd, (struct sockaddr *) &client_addr, &sock_size);
        
        if(client_sock == -1){
            return 1;
        }
        else{
            cout << "Connection from " << inet_ntoa(client_addr.sin_addr) << endl;
            handle_client(client_sock);
        }
    }

    return 0;
}
