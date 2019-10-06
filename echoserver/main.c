#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>



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
    //server_addr.sin_port = htons((uint16_t)argv[1]);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("Build \n");
    char buff_rcv[1024];
    char buff_snd[1024];


    int bind_R = bind(s_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(bind_R == -1){
        printf("Bind failed\n");
        return -1;
    }

    int listen_R = listen(s_socket,5);
    if(listen_R < 0){
        printf("Listen failed\n");
        return -1;
    }

    int client_addr_size = sizeof(client_addr);
    int c_socket = accept(s_socket,(struct sockaddr*)&client_addr,&client_addr_size);

    while(1)
    {

        if(c_socket < -1){
            printf("Client Accept Failed\n");
            return -1;
        }

        int read_R = (int)read(c_socket,buff_rcv,1024);
        if(read_R == -1){
            printf("Read Failed\n");
            return -1;
        }

        printf("%s \n",buff_rcv);


        //sprintf(buff_snd,"%s",buff_rcv);
        write(c_socket,buff_rcv,strlen(buff_rcv)+1);
    }
    close(c_socket);







    return 0;
}
