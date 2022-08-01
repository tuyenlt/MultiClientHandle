#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>

#define PORT 8005
#define ADDRESS "127.0.0.4"

using namespace std;


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
        char buf[1024];
        string recv_msg;
        int letter_recv;
        while(1){
            letter_recv = recv(sockfd, buf, 1024,0);
            recv_msg = buf;
            cout << recv_msg.substr(0,letter_recv) << endl;
        }
    }

    close(sockfd);

    return 1;
}