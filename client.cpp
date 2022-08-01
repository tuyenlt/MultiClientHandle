#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <thread>

#define PORT 8000
#define ADDRESS "127.0.0.4"


using namespace std;

void sendMsg(string *client_msg , int socket){
    while(*client_msg != "exit"){
        fflush(stdin);
        cin.clear();
        getline(cin, *client_msg);
        send(socket, client_msg->c_str(), client_msg->length(), 0);
        cout << "passed" << endl;
    }
}

int main()
{
    int sockfd = -1;
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ADDRESS);

    // if (inet_pton(AF_INET, ADDRESS, &server_addr.sin_addr)
    //     <= 0) {
    //     printf(
    //         "\nInvalid address/ Address not supported \n");
    //     return -1;
    // }

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if((connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr))) < 0){
        perror("connect to server fail");
        exit(EXIT_FAILURE);
    }
    else{
        string client_msg = "";
        thread sendThread(sendMsg, &client_msg ,sockfd);
        char buf[1024];
        string recv_msg;
        int letter_recv;
        while(client_msg != "exit"){
            letter_recv = recv(sockfd, buf, 1024,0);
            recv_msg = buf;
            cout << recv_msg.substr(0,letter_recv) << endl;
        }
    }

    close(sockfd);

    return 1;
}