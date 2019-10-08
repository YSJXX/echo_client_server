#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


#define BUFF_SIZE 1024

struct Node {
    struct Node *next;
    int s_socket;
}node;

static struct Node *head;


struct thread_data{
    int s_socket;
};

void *thread ()
{
    char buff_rcv[BUFF_SIZE];

    while(1)
    {
        int read_R = (int)read(node.s_socket,buff_rcv,BUFF_SIZE);                     //Read
        if(read_R == -1){
            printf("Read Failed\n");
            //return -1;
        }
        printf("|||||| TEST\n");
        printf("%s \n",buff_rcv);


        //sprintf(buff_snd,"%s",buff_rcv);
        write(node.s_socket,buff_rcv,strlen(buff_rcv)+1);
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


    struct thread_data *save_date;


    //head->next = NULL;

    //head->s_socket = NULL;

    while(1)
    {
        int client_addr_size = sizeof(client_addr);
        //node = malloc(sizeof(struct Node));
        node.s_socket = accept(s_socket,(struct sockaddr*)&client_addr,&client_addr_size);

        if(head == NULL)
            head = &node;
        else
            head->next = &node;
        head = &node;

        if(node.s_socket < -1){
            printf("Client Accept Failed\n");
            return -1;
        }

        pthread_create(&jthread,NULL,thread,0);

    }
    //int status;
    //pthread_join(thread_listen,(void **)&status);



    //close(list->s_socket);
    free(head);
    //free(list);
    return 0;
}
