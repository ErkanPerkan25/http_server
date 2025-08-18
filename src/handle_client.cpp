#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "handle_client.hpp"

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

/*
int handle_client(int conn_sock, struct sockaddr_in* client_addr){
    //stringstream req; 
    //string method, path, version;

    char buffer[81];
    int charsRead;
    int totalCharsRead = 0;

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
    
    //totalCharsRead = 0;

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

        path = "../www/"+path;


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


            fs.close();

            char* cbuff = (char *) buffer.c_str();
            int needed = buffer.size();

            while (needed) {
                int n = write(conn_sock, cbuff, needed);
                needed -= n;
                cbuff += n;
            }
        }
        else{
            string buffer;

            buffer += "404 NOT FOUND\r\n\r\n";
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
*/
