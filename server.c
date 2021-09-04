#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

#define SIZE 500

struct time_process{
    int time;
    int process;
    int kernel_time;
    char p_name[100];
};


void insert_by_priority(struct time_process);
void delete_by_priority(struct time_process);
void create();
void check(struct time_process);
void display_pqueue();
//
struct time_process endData ={-1, -1, -1, "no_name"};

struct time_process pri_que[SIZE];

int front, rear;

void create();
void insert_end();

struct node {
	int data;
	struct node* next;
};
struct node *start = NULL;
struct node *tmp;


void create(){
    front = rear = -1;
}


void insert_by_priority(struct time_process data){
    if (rear >= MAX - 1){
        printf("\nQueue overflow no more elements can be inserted");
        return;
    }

    if ((front == -1) && (rear == -1)){
        front++;
        rear++;
        pri_que[rear] = data;
        return;
    }    

    else check(data);

    rear++;
}

void check(struct time_process data){

    int i,j;

    for (i = 0; i <= rear; i++){

        if (data.time >= pri_que[i].time){
            for (j = rear + 1; j > i; j--){
                pri_que[j] = pri_que[j - 1];
            }
            pri_que[i] = data;
            return;
        }
    }

    pri_que[i] = data;

}

void delete_by_priority(struct time_process data){
    int i;

    if ((front==-1) && (rear==-1)){
        printf("\nQueue is empty no elements to delete");
        return;

    }
    for (i = 0; i <= rear; i++){
        if (data.time == pri_que[i].time){
            for (; i < rear; i++){
                pri_que[i] = pri_que[i + 1];

        }
        pri_que[i] = endData;
        rear--;
        if (rear == -1) 
            front = -1;

        return;

        }
    }
}

void display_pqueue(){

    if ((front == -1) && (rear == -1)){
        printf("\nQueue is empty");
        return;
    }

    for (; front <= rear; front++){
        printf("%d %d\n", pri_que[front].time, pri_que[front].process);
    }

    front = 0;
}


void insert_begin(int data)
{
    struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    if(temp==NULL){
        printf("nOut of Memory Space:n");
        return;
    }
        
    temp->data = data;
    temp->next =NULL;
    if(start==NULL){
        start=temp;
    }
    else{
        temp->next=start;
        start=temp;
    }
}

int is_pid_folder(const struct dirent *entry) {
    const char *p;

    for (p = entry->d_name; *p; p++) {
        if (!isdigit(*p))
            return 0;
    }

    return 1;
}

int files;
int fileNum = 0;

void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    start = NULL;
    int connfd = *(int*)socket_desc;
    free(socket_desc);

    char num[20];
    read(connfd, num, sizeof(num));
    int n = atoi(num);

    DIR *folder;
    
    struct dirent *entry;
    
    folder = opendir("/proc");
    if(folder == NULL){
        perror("Unable to read directory");
        return 0;
    }

    files = 0;

    while((entry=readdir(folder))){
        if (!is_pid_folder(entry))
            continue;
        int pid = atoi(entry->d_name);
        insert_begin(pid);
        files++;
    }
    chdir("/proc");
    create();
    tmp = start;
    while(tmp!=NULL){
        int pid = tmp->data;
        char str[20];
        sprintf(str,"%d",pid);
        chdir(str);
        //printf("%s ",str);
        int fd;
        fd = open("stat",O_RDONLY);
        if (fd ==-1){
            printf("Error Number % d\n", errno);
            perror("Program");                
        }
    
        char buff[1024];
        char c;
        while(read(fd,&c,1)!=0) {
            strncat(buff,&c,1);
        }

        char *token = strtok(buff," ");
        int iter = 0;
        int utime;
        int stime;
        char naam[100];
        while(token!=NULL){
            // printf("%s ", token);
            if(iter==1){
                strcpy(naam,token);
            }
            if(iter==13) utime = atoi(token);
            if(iter==14) stime = atoi(token);
            iter = iter+1;
            token = strtok(NULL, " ");
        }
        //printf("\n");
        struct time_process s1 = {utime+stime,pid,stime,*naam};
        memset(buff,0,strlen(buff));
        insert_by_priority(s1);
        tmp = tmp->next;
        chdir("..");
    }

    // display_pqueue();

    closedir(folder);

    chdir("/home/kesar/Desktop");

    // FILE *fp;
    // fp = fopen("sentToClient.txt", "a");
    FILE *fp;
    char toAppend[100];
    sprintf(toAppend,"%d",fileNum);
    char fileName[] = "sentToClient";
    strcat(fileName, toAppend);
    strcat(fileName,".txt");
    fp = fopen(fileName, "a");
    if(fp == NULL)
    {
        /* Unable to open file hence exit */
        printf("Unable to open file.\n");
        printf("Please check whether file exists and you have read privilege.\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i<n; i++){
        int a = pri_que[i].process;
        int b = pri_que[i].time-pri_que[i].kernel_time;
        int c = pri_que[i].kernel_time;

        char stra[10];
        char strb[10];
        char strc[10];
        sprintf(stra, "%d", a);
        strcat(stra, " ");
        sprintf(strb, "%d", b);
        strcat(strb, " ");
        sprintf(strc, "%d", c);
        strcat(strc, " ");

        strcat(stra, strb);
        strcat(stra, strc);

        char finalString[] = "PID; User CPU Time; Kernel CPU Time: ";
        strcat(finalString, stra);
        // printf("%s\n", finalString);
        fprintf(fp, "%s ", finalString);
    }

    fclose(fp);

    char buff[MAX];

    FILE *f;
    
    int words = 0;
    char c;
    char dhinchak[] = "/home/kesar/Desktop/";
    strcat(dhinchak,fileName);
    f=fopen(dhinchak,"r");
    // f=fopen("/home/kesar/Desktop/sentToClient.txt","r");
    while((c=getc(f))!=EOF)         //Counting No of words in the file
    {   
        fscanf(f , "%s" , buff);
        if(isspace(c)||c=='\t')
        words++;    
    }
    //printf("Words = %d \n"  , words); //Ignore
    write(connfd,toAppend,sizeof(toAppend));
    write(connfd, &words, sizeof(int));
    rewind(f);
    while(fscanf(f, "%s", buff)!=EOF){  
        write(connfd,buff,sizeof(buff));  
   } 

    printf("The file was sent successfully.\n");
    printf("%c",'\n');
    int a = read(connfd, buff, sizeof(buff));
    if (a ==-1){
            printf("Error Number % d\n", errno);
            perror("Program");                
        }
    // printf("%d\n",a);
        fclose(f);
    printf("Received from client %d\n", fileNum);
    printf("%c",'\n');
    printf("%s\n", buff);
    //Free the socket pointer
    

    close(connfd);
    return 0;
}

int main(){
    
    printf("%c",'\n');

    int sockfd, connfd, len, *new_sock;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 3)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    while( (connfd = accept(sockfd, (struct sockaddr *)&cli, (socklen_t*)&len)) )
    {
        puts("Connection accepted");
        
        //Reply to the client
        // message = "Hello Client , I have received your connection. And now I will assign a handler for you\n";
        // write(new_socket , message , strlen(message));
        
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = connfd;
        
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        fileNum = fileNum+1;
        puts("Handler assigned");
    }

    // Accept the data packet from client and verification
    // connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    // int n;
    // printf("Enter n: ");
    // scanf("%d", &n);
    
    close(sockfd);
    // close(connfd);
    return(0);
}
