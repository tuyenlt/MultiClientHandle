#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>


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
    fd_set readfds;
    int max_sd;
    int client_socket[MAX_CLIENT];
    int activity;
    int sd;

    while(true){
        FD_ZERO(&readfds);
        FD_SET(sock_listen, &readfds);
        max_sd = sock_listen;

        for( int i=0; i < MAX_CLIENT; i++)
        {
            sd = client_socket[i];
            if(sd > 0){
                FD_SET(sd, &readfds);
            }

            if(sd > max_sd){
                max_sd = sd;
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if((activity < 0) && (errno != EINTR)){
            perror("select eror");
        }

        // incomming client connect handle
        if(FD_ISSET(sock_listen, &readfds)){
            if(sock_connection = accept(sock_listen,(sockaddr*)&server_addr, 
                                        (socklen_t*)sizeof(server_addr)) < 0)
            {
                perror("acept error:");
                exit(EXIT_FAILURE);
            }else{
                send(sock_connection, "wellcome to the server",22,0);
                // add new socket to the socket list
                for(int i=0; i < MAX_CLIENT; i++){
                    //if possition is empty
                    if(client_socket[i] == 0){
                        client_socket[i] = sock_connection;
                        cout << "added new client to the client list" << endl;
                    }
                }

            }
        }
        //else it will be other client contact
        for(int i=0; i < MAX_CLIENT; i++){
            int val_read;
            char buf[1024];
            string recv_msg;
            sock_connection = client_socket[i];

            if(FD_ISSET(sock_connection, &readfds)){
                if((val_read = read(sock_connection, buf, 1024)) == 0){
                    cout << "some body is disconnected" << endl;
                    close(sock_connection);
                    client_socket[i] = 0;
                }else{
                    send(sock_connection, buf, val_read,0);
                }
            }
        }
        
    }

    // end proccess

    //shut down the server
    close(sock_connection);
    shutdown(sock_listen,SHUT_RDWR);
    return 1;
}