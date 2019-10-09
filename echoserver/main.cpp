#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <list>

#define BUFF_SIZE 1024

using namespace std;

static std::list<int> li;

struct thread{
    int socket;
};


void *thread (void * arg)
{
    struct thread * data=(struct thread *)arg;
    int socket = data->socket;
    char buff_rcv[BUFF_SIZE];
    char msg[10]="Connect";
    while(true)
    {
        if(recv(socket,buff_rcv,BUFF_SIZE,0) == -1){
            printf("Read Failed\n");
            //return -1;
        }
        if(!(strncmp(buff_rcv,msg,7))) memset(buff_rcv,0,BUFF_SIZE);
        printf("%s \n",buff_rcv);

        for(list<int>::iterator it=li.begin();it !=li.end();++it)
        {
            if(send(*it,buff_rcv,strlen(buff_rcv)+1,0) == -1)    printf("send Failed\n");
        }
    }
}


int main(int argc, char* argv[])
{
    if(argc >3) return -1;

    int s_socket = socket(AF_INET,SOCK_STREAM,0);
    if(s_socket == -1){
        printf("Open Failed\n");
        return -1;
    }

    struct sockaddr_in server_addr,client_addr;

    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((u_int16_t)atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("###Sever \n");
    int opt = 1;
    setsockopt(s_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    int bind_R = bind(s_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(bind_R == -1){
        printf("Bind failed\n");
        return -1;
    }

    pthread_t jthread;


    int listen_R = listen(s_socket,5);
    if(listen_R < 0){
        printf("Listen failed\n");
        return -1;
    }

    struct thread *data=(struct thread*)malloc(sizeof(struct thread*));

    while(1)
    {
        int client_addr_size = sizeof(client_addr);
        li.push_back(accept(s_socket,(struct sockaddr*)&client_addr,(socklen_t*)&client_addr_size));
        if(li.back() < -1){
            printf("Client Accept Failed\n");
            return -1;
        }
        data->socket=li.back();
        pthread_create(&jthread,nullptr,thread,data);
    }

    return 0;
}
