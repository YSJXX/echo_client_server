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

struct Node {
    int s_socket;
    struct Node *next;
};



struct thread_data{
    int s_socket;
    struct Node *head;
};

void *thread (void* arg)
{

    struct thread_data * data =(struct thread_data *)arg;

    struct Node * head = (struct Node *)data->head;
    int s_socket = data->s_socket;

    char buff_rcv[BUFF_SIZE];

    while(1)
    {
        //printf("node->s_socket : %d\n",s_socket);
        printf("2: node address : %p\n",(void *)head->next);
        //head = head->next;
        int read_R = (int)read(s_socket,buff_rcv,BUFF_SIZE);                     //Read
        if(read_R == -1){
            printf("Read Failed\n");
            //return -1;
        }
        printf("%s \n",buff_rcv);



        //sprintf(buff_snd,"%s",buff_rcv);
        write(s_socket,buff_rcv,strlen(buff_rcv)+1);
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
    std::list<int> list;
    struct Node *head = malloc(sizeof (struct Node));

    head->next =NULL;
    head->s_socket = NULL;

    while(1)
    {
        int client_addr_size = sizeof(client_addr);
        struct thread_data *data =malloc(sizeof(struct thread_data));
        struct Node *node = malloc(sizeof (struct Node));
        node->s_socket = accept(s_socket,(struct sockaddr*)&client_addr,&client_addr_size);
        data->s_socket=node->s_socket;
        if(node->s_socket < -1){
            printf("Client Accept Failed\n");
            return -1;
        }

        if(head == NULL)
        {
            head = node;
            head->next=NULL;
        }
        else
        {
            node->next = head;
            head = node;
        }

        data->head =head;
        printf("1: node address : %p\n",(void *)node);
        printf("---------head address : %p\n",(void *)head);
        pthread_create(&jthread,NULL,thread,(void *)data);
        sleep(1);
    }
    //int status;
    //pthread_join(thread_listen,(void **)&status);



    //close(list->s_socket);
    free(head);
    //free(list);
    return 0;
}
