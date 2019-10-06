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

    int write_R;
    int read_R;
    char send_text[BUFF_SIZE];
    while(1)
    {

        //fgets(send_text,100,stdin);
        scanf("%s",send_text);
        write_R = (int)write(c_socket,send_text,strlen(send_text));
        //send(c_socket,send_text,strlen(send_text),0);
        //printf("TEST\n");

        if(write_R == -1) printf("Write Failed\n");

        char buff[1024];
        read_R = (int)read(c_socket,buff,1024);
        if(read_R == -1) printf("Read Failed\n");

        printf("%s\n",buff);
    }

    close(c_socket);
    return 0;
}
