#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 8080
#define MAX_CLIENT 10

using namespace std;

int main(){
    int sock_listen;
    int sock_connection;
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    sock_listen = socket(AF_INET,SOCK_STREAM,0);
    if(bind(sock_listen, (sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("bind error:");
        exit(EXIT_FAILURE);
    }
    if(listen(sock_listen, MAX_CLIENT) < 0){
        perror("Listen fail:");
        exit(EXIT_FAILURE);
    }
    // do somethings to handle multiClient connecting 

    // end proccess
    
    //shut down the server
    close(sock_connection);
    shutdown(sock_listen,SHUT_RDWR);
    return 1;
}