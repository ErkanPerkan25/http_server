#include <iostream>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define SUCCESS     0
#define USAGE_ERROR 1
#define SOCK_ERROR  2
#define BIND_ERROR  3
#define LISTEN_ERROR 4

#define MAX_WAITING 25
#define MAX_BUFFER_SIZE 1025


int connectToServer(int port);
void processRequest(int conn_sock, struct sockaddr_in *);

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

    // Will never get here until server is shutdown
    return SUCCESS;
}

void processRequest(int conn_sock, struct sockaddr_in* client_addr){
    char* buffer[MAX_BUFFER_SIZE];
    int charsRead = 0;
    int totalCHarsRead = 0;

    while(true){
        charsRead = read(conn_sock, buffer, 1024);

        // breaks if errors (-1) or EOF (0)
        if(charsRead <= 0)
            break;

        totalCHarsRead += charsRead;

    }
    
    cout << buffer << endl;

    close(conn_sock);
}



int main(int argc, char* argv[]){

    if(!argv[1]){
        cerr << "USAGE: " << argv[0] << " <port number>\n";
        return USAGE_ERROR;
    }

    return connectToServer(stoi(argv[1]));
}
