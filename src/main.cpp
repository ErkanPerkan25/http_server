#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

#define SUCCESS     0
#define USAGE_ERROR 1
#define SOCK_ERROR  2
#define BIND_ERROR  3
#define LISTEN_ERROR 4

#define MAX_WAITING 25

int connectToServer(int port){
    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);

    // If socket() fails, return SOCK_ERROR
    if(listen_socket < 0){
        cerr << "Could not connect to listening socket\n";
        return SOCK_ERROR;
    }

    struct sockaddr_in local_addr;
    // Fills in the local (server) half of the socket info
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listen_socket, (struct sockaddr *) &local_addr, sizeof(local_addr)) != 0){
        cerr << "Binding failed - this could be caused by:\n" <<
        "   - an invalid port (no access, or already in use)\n" <<
        "   - an invalid address (did you use a wildcard?)\n";
        return BIND_ERROR;
    }

    if(listen(listen_socket, MAX_WAITING) != 0) {
        cerr << "Too long listening, aborting!\n";
        return LISTEN_ERROR;
    }

    while(true){
    }

    // Will never get here until server is shutdown
    return SUCCESS;
}



int main(int argc, char* argv[]){

    if(!argv[1]){
        cerr << "USAGE: " << argv[0] << " <port number>\n";
        return USAGE_ERROR;
    }

    return connectToServer(stoi(argv[1]));
}
