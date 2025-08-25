#include <cerrno>
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
    const char *version;
    const char *status;
    const char *reason;
} status_line_t;

typedef struct{
    status_line_t status_line;
    const char *content_type;
    uint32_t cotent_length;
} http_res_t;

void cleanup(char **ptr){
    free(*ptr);
    *ptr = NULL;
}

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
    req->uri = (char *)calloc(uri_size_len+1, sizeof(char));
    if(req->uri == NULL){
        goto free_up_memory;
        return -1;
    }
    strncpy(req->uri, uri, uri_size_len);
    
    char *field;
    while(line != NULL){
        line = strtok_r(NULL, CRLF, &saveptr);

        char *token_ptr;
        char *token = strtok_r(line, " ", &token_ptr);

        while(token != NULL){
            size_t token_size = strlen(token);
            if(strncmp("Host:", token, token_size) == 0){
                field = req->host;
            }
            else if(strncmp("User-agent:", token, token_size) == 0){
                field = req->usr_agent;
            }
            else{
                if(field == req->host){
                    req->host = (char *)calloc(token_size+1, sizeof(char));
                    if(req->host == NULL){
                        goto free_up_memory;
                    }
                    strncpy(req->host, token, token_size+1);
                }
                else if(field == req->usr_agent){
                    req->usr_agent = (char *)calloc(token_size+1, sizeof(char));
                    if(req->usr_agent == NULL){
                        goto free_up_memory;
                    }
                    strncpy(req->usr_agent, token, token_size+1);
                }
            }
            token = strtok_r(NULL, " ", &token_ptr);
        }
    }

    return 0;

    free_up_memory:
        cleanup(&req->method);
        cleanup(&req->uri);
        cleanup(&req->host);
        cleanup(&req->usr_agent);
    return -1;
}

char* create_http_res(char *content, http_res_t *res){
    res->status_line.status = "200";
    res->status_line.reason = "OK";
    res->status_line.version = "HTTP/1.1";

    if(content == NULL){
        res->status_line.status = "500";
        res->status_line.reason = "Server Error";
    }

    res->content_type = "text/html";
    res->cotent_length = sizeof(content);


    return 0;
}

long get_fsize(FILE *fptr){
    if(fseek(fptr, 0, SEEK_END) != 0){
        fclose(fptr);
        printf("Error seeking file: %d\n", errno);
        return -1;
    }
    long fsize = ftell(fptr);
    if(ftell(fptr) == -1){
        fclose(fptr);
        printf("Error telling file: %d\n", errno);
        return -1;
    }
    rewind(fptr);
    return fsize;
}

int handle_client(int conn_sock){

    char buffer[MAX_BUFFER];
    int charsRead = 0;
    int totatlCharsRead = 0;

    while (true) {
        charsRead = recv(conn_sock, buffer, MAX_BUFFER,0);
        
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

    http_req_t req {0};

    if(parse_http_req(buffer, &req) == 1){
        return -1;
    }

    // remove leading '.' and '/'
    size_t url_size = strlen(req.uri);  
    size_t filename_size;
    // gets the size of filename
    for(int i,j=0; i < url_size; i++){
        if(req.uri[i] != '.' || req.uri[i] != '/'){
            j++;
        }
        else{
            filename_size = j;
            break;
        }
    }

    // allocates the memory and sets it
    char *filename = (char *)calloc(filename_size+1, sizeof(char));
    for(int j,i=0; i < url_size; i++){
        if(req.uri[i] != '.' || req.uri[i] != '/'){
            filename[j++] = req.uri[i];
        }
        else{
            filename[j] = '\0';
            break;
        }
    }

    if(filename == NULL){
        return -1;
    }
    //set path to file
    char *path = (char*)"../www";

    size_t path_size = strlen(path);
    size_t file_var_size = strlen(filename);

    char *full_path = (char*)calloc((path_size+file_var_size)+1, sizeof(char));
    if(full_path == NULL){
        cleanup(&path);
        cleanup(&filename);
        cleanup(&req.method);
        cleanup(&req.uri);
        cleanup(&req.host);
        cleanup(&req.usr_agent);
        return -1;
    }

    strcpy(full_path, path);
    strcat(full_path, filename);

    printf("%s\n", filename);
    printf("%s\n", full_path);


    char *fbuffer; 
    FILE* fptr = fopen(full_path, "r");

    if(fptr == NULL){
        printf("Error: Could access file on server.");
        cleanup(&req.method);
        cleanup(&req.uri);
        cleanup(&req.host);
        cleanup(&req.usr_agent);
        return -1;
    }
    else{
        long fsize = get_fsize(fptr);

        fbuffer = (char*)calloc(fsize+1, sizeof(char));

        if(fbuffer == NULL){
            cleanup(&req.method);
            cleanup(&req.uri);
            cleanup(&req.host);
            cleanup(&req.usr_agent);
            return -1;
        }

        fread(fbuffer, sizeof(char), fsize, fptr);

        if(fbuffer == NULL){
            fclose(fptr);
            cleanup(&req.method);
            cleanup(&req.uri);
            cleanup(&req.host);
            cleanup(&req.usr_agent);
            return -1;
        }

        fbuffer[fsize] = '\0';

        fclose(fptr);
    }

    printf("%s\n", fbuffer);

    http_res_t res{0};
    char *res_str = create_http_res(buffer, &res);

    return close(conn_sock);
}

