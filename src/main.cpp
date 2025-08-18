#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "network.hpp"

using namespace std;

#define USAGE_ERROR 1

int main(int argc, char* argv[]){

    if(!argv[1]){
        cerr << "USAGE: " << argv[0] << " <port number>\n";
        return USAGE_ERROR;
    }

    if(create_conn_sock(stoi(argv[1])) == 0){
    }

    return 0;
}
