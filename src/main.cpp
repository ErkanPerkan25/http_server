#include <csignal>
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <fstream>

using namespace std;

#define SUCCESS     0
#define USAGE_ERROR 1
#define SOCK_ERROR  2
#define BIND_ERROR  3
#define LISTEN_ERROR 4

#define MAX_WAITING 25
#define MAX_BUFFER_SIZE 255


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
    stringstream req; 
    string method, path, version;

    char buffer[81];
    int charsRead;
    int totalCharsRead;

    while(true){
        charsRead = read(conn_sock, buffer, 80);
        
        // Either failed or had nothing more to read
        if(charsRead <= 0)
            break;
        
        req << buffer;

        totalCharsRead += charsRead;

        if(charsRead < 80)
            break;
    }

    cout << req.str().length() << endl;

    cout << "Total chars read: " << totalCharsRead << endl;

    
    req.str()[totalCharsRead] = '\0';

    if(req.str()[totalCharsRead-1] == '\n')
        req.str()[totalCharsRead-2] = '\0';
    
    totalCharsRead = 0;

    req >> method >> path;

    if(method == "GET"){
        cout << "Recived a 'GET' request! " << endl;
        // Removes the leading . and / of the path
        for(int i=0; i < path.size(); i++){
            if(path[i] == '.' || path[i] == '/'){
                path.erase(path.begin()+i);
                i--;
            }
            else{
                break;
            }
        }

        path = "../html/"+path;


        fstream fs;
        fs.open(path);
        if(fs.is_open()){
            string buffer;

            buffer += "HTTP/1.1 202 OK\n";
            buffer += "Content-Type: text/html\n\r\n";

            char ch;
            while (fs.get(ch)) {
                buffer += ch;
            }

            char* cbuff = (char *) buffer.c_str();
            int needed = buffer.size();

            while (needed) {
                int n = write(conn_sock, cbuff, needed);
                needed -= n;
                cbuff += n;
            }

            fs.close();
        }
        else{
            string buffer;

            buffer += "404 NOT FOUND \r\n\r\n";
            buffer += "<b>404 - Error resource not found on the server</b>";

            char* cbuff = (char *) buffer.c_str();

            int needed = buffer.length();

            while(needed){
                int n = write(conn_sock, cbuff, needed);
                needed -= n;
                cbuff += n;
            }

        }
    }
    else{
        cerr << "405 Method not allowed! Stopping the request!\n";
    }

    cout << "Connection from " << inet_ntoa(client_addr->sin_addr) << endl;

    close(conn_sock);
}



int main(int argc, char* argv[]){

    if(!argv[1]){
        cerr << "USAGE: " << argv[0] << " <port number>\n";
        return USAGE_ERROR;
    }

    return connectToServer(stoi(argv[1]));
}
