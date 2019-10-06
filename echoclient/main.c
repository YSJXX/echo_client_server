#include <stdio.h>
#include <sys/socket.h>     //sockect
#include <arpa/inet.h>      //sockaddr_in
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 1024

int main(int argc,char * argv[])
{
    int c_socket = socket(AF_INET,SOCK_STREAM,0);

    if(c_socket == -1)
    {
        printf("Socket Open Failed\n");
        return -1;
    }

    struct sockaddr_in server_addr;

    //memset(&server_addr,0,sizeof(server_addr));
    bzero((char*)&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((u_int16_t)atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    int connect_R = connect(c_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(connect_R == -1)
    {
        printf("Connect Failed\n");
        return -1;
    }

    int send_R;
    int recv_R;
    char chat_buff[BUFF_SIZE];
    char send_buff[BUFF_SIZE];
    //char rcv_buff[BUFF_SIZE];
    while(1)
    {

        fgets(chat_buff,100,stdin);

        sprintf(send_buff,"Jun: %s \n",chat_buff);
        send_R = send(c_socket,send_buff,sizeof(send_buff),0);
        if(send_R == -1) printf("Send Failed\n");
        memset(chat_buff,0,BUFF_SIZE);
        memset(send_buff,0,BUFF_SIZE);


        recv(c_socket,chat_buff,BUFF_SIZE,0);
        if(recv_R == -1) printf("Recv Failed\n");

        printf("%s\n",chat_buff);
        memset(chat_buff,0,BUFF_SIZE);

    }

    close(c_socket);
    return 0;
}
