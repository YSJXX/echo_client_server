#include <iostream>
#include <stdio.h>
#include <sys/socket.h>     //sockect
#include <arpa/inet.h>      //sockaddr_in
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>

using namespace std;

#define BUFF_SIZE 1024

struct threadD{
    int socket;
};

void* threadS(void* arg)
{
    struct threadD * data =(struct threadD *)arg;
    char buff[BUFF_SIZE];
    while(true)
    {
        if(recv(data->socket,buff,BUFF_SIZE,0)== -1) printf("Recv Failed\n");
        printf("%s\n",buff);
    }
}

int main(int argc,char * argv[])
{
    int c_socket = socket(AF_INET,SOCK_STREAM,0);

    if(c_socket == -1)
    {
        printf("Socket Open Failed\n");
        return -1;
    }

    struct sockaddr_in server_addr;
    pthread_t thread;
    struct threadD * data=(struct threadD*)malloc(sizeof(struct threadD));
    //memset(&server_addr,0,sizeof(server_addr));
    bzero((char*)&server_addr,sizeof(server_addr));                     //?
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
    char chat_buff[BUFF_SIZE];
    char send_buff[BUFF_SIZE];
    char comp[10]="exit";
    char msg[10]="Connect";
    send_R = (int)send(c_socket,msg,sizeof(msg),0);
    if(send_R == -1) printf("Send Failed\n");
    while(true)
    {

        fgets(chat_buff,100,stdin);
        //scanf("%s",chat_buff);
        if(!strncmp(chat_buff,comp,4))
        {
            sprintf(send_buff,"----------------------was leave ");
            send_R = (int)send(c_socket,send_buff,sizeof(send_buff),0);
            memset(send_buff,0,BUFF_SIZE);
            close(c_socket);
            break;
        }
        sprintf(send_buff,"Jun1: %s ",chat_buff);

        send_R = (int)send(c_socket,send_buff,sizeof(send_buff),0);
        if(send_R == -1) printf("Send Failed\n");
        memset(chat_buff,0,BUFF_SIZE);


        data->socket=c_socket;
        pthread_create(&thread,nullptr,threadS,data);
        memset(send_buff,0,BUFF_SIZE);

    }

    return 0;
}
