#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

#include "handle_client.hpp"

#define MAX_BUFFER 512
#define CRLF "\r\n"

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
} status_line_t;

typedef struct{
    status_line_t status_line;
    char *content_type;
    uint32_t cotent_length;
} http_res_t;

int parse_http_req(char * msg, http_req_t *req){
    // parse each line by using CRLF
    char* saveptr; // stores the rest of the msg here
    char* line = strtok_r(msg, CRLF, &saveptr);

    // parse the words in the line
    char *first_token_ptr;
    char *method_var = strtok_r(line, " ", &first_token_ptr);
    size_t method_size_len = strlen(method_var);
    // Dynamic allocation of memory, set it to zero first
    req->method = (char *)calloc(method_size_len+1, sizeof(char));
    if(req->method == NULL) return -1;
    // set the 'req->method' to method_var
    strncpy(req->method, method_var, method_size_len+1);

    // parse the url (path)
    char *second_token_ptr;
    char *uri = strtok_r(first_token_ptr, " ", &second_token_ptr);
    size_t uri_size_len = strlen(uri);
    req->uri = (char *)calloc(uri_size_len, sizeof(char));
    if(req->uri == NULL){
        return -1;
    }
    strcpy(req->uri, uri);

    while(line != NULL){
    
    }

    // parse the host
    /*
    line = strtok_r(saveptr, CRLF, &saveptr);

    char* host_var;
    char* host_h = strtok_r(line, " ", &host_var);
    size_t  host_size_len = strlen(host_h);
    req->host = (char*)calloc(host_size_len+1, sizeof(char));
    if(req->host == NULL) return -1; 
    strcpy(req->host, host_h);

    line = strtok_r(saveptr, CRLF, &saveptr);
    */

    
    return 0;
}

int handle_client(int conn_sock){

    http_req_t client_req {};
    http_res_t client_res {};

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

    parse_http_req(buffer, &client_req);
    //std::cout << buffer << std::endl;

    return close(conn_sock);
}
