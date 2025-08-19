#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "handle_client.hpp"

#define MAX_BUFFER 512

typedef struct{
    char *method;
    char *uri;
    char *host;
    char *usr_agent;
} http_req_t;

typedef struct{
    char *version;
    char *status;
    char *reason;
} status_line;

int handle_client(int conn_sock){

    http_req_t client_req {};
    status_line status_line {};

    char buffer[MAX_BUFFER];
    int charsRead = 0;
    int totatlCharsRead = 0;

    while (true) {
        charsRead = read(conn_sock, buffer, MAX_BUFFER);
        
        // Either EOF or Error
        if(charsRead < 0)
            break;
        
        totatlCharsRead += charsRead;

        if(charsRead < MAX_BUFFER)
            break;
    }

    buffer[totatlCharsRead-1] = '\0';
    if(buffer[totatlCharsRead-1] == '\n'){
        buffer[totatlCharsRead-2] = '\0';
    }

    std::cout << buffer << std::endl;

    return close(conn_sock);
}
