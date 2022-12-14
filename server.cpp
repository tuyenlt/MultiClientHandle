#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <fstream>
#include <thread>


#define PORT 8000
#define MAX_CLIENT 10
#define ADDRESS "127.0.0.4"

using namespace std;

int main(){
    int sock_listen;
    int sock_connection;
    int opt = 1;
    sockaddr_in server_addr;
    int addlen = sizeof(server_addr);
    
    sock_listen = socket(AF_INET,SOCK_STREAM,0);

   if( setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);


    if(bind(sock_listen, (sockaddr*)&server_addr, addlen) < 0){
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
    int client_socket[MAX_CLIENT] = {0};
    int activity;
    int sd;
    string data; // 

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
            exit(EXIT_FAILURE);
        }

        // new client connect handle
        if(FD_ISSET(sock_listen, &readfds)){
            if((sock_connection = accept(sock_listen,(sockaddr*)&server_addr, 
                                        (socklen_t*)&addlen)) < 0)
            {
                perror("acept error:");
                exit(EXIT_FAILURE);
            }else{
                cout << "acepted new client" << endl;
                // send(sock_connection,"acpeted",7,0);
                fstream load_history("log.txt");
                int leter;
                while(!load_history.eof()){
                    getline(load_history,data);
                    leter = send(sock_connection, data.c_str(), data.length(),0);
                }
                load_history.close();
                // add new socket to the socket list
                for(int i=0; i < MAX_CLIENT; i++){
                    //if possition is empty
                    if(client_socket[i] == 0){
                        client_socket[i] = sock_connection;
                        // cout << "added new client to the client list" << endl; 
                    }
                }

            }
        }
        //if other client contact
        for(int i=0; i < MAX_CLIENT; i++){
            int val_read;
            char buf[1024];
            string recv_msg;
            sock_connection = client_socket[i];

            if(FD_ISSET(sock_connection, &readfds)){
                if((val_read = read(sock_connection, buf, 1024)) < 0){
                    // client disconnected
                    cout << "client disconnected" << endl;
                    close(sock_connection);
                    client_socket[i] = 0;
                }else{
                    cout << "client send msg" << endl;
                    fstream log_file("log.txt",ios::app|ios::out);
                    recv_msg = buf;
                    log_file << recv_msg.substr(0,val_read);
                    log_file.close();
                    send(client_socket[i], buf, val_read,0);
                    cout << "send msg to client " << client_socket[i] << endl;
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