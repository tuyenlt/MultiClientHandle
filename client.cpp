#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>

#define PORT 8080
#define ADDRESS "127.0.0.1"

using namespace std;


int main()
{
    int sockfd = -1;
    sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ADDRESS);
    server_addr.sin_port = htons(PORT);

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if((connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr))) < 0){
        perror("connect to server fail");
        exit(EXIT_FAILURE);
    }
    else{
        char buf[1024];
        string recv_msg;
        int letter_recv;
        letter_recv = recv(sockfd, buf, 1024,0);
        recv_msg = buf;
        cout << "msg recv from server:" << recv_msg << endl;
    }

    close(sockfd);

    return 1;
}