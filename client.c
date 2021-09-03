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
#include <arpa/inet.h>
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

#define SIZE 500

struct time_process{
    int time;
    int process;
    int kernel_time;
};

void insert_by_priority(struct time_process);
void delete_by_priority(struct time_process);
void create();
void check(struct time_process);
void display_pqueue();
//
struct time_process endData ={-1, -1, -1};

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


void func(int sockfd)
{
	char buff[MAX];
	int n;
	read(sockfd, buff, sizeof(buff));
	printf("From Server : %s", buff);
	
}

int main()
{
    printf("%c",'\n');
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	int n;
    printf("Enter n: ");
    scanf("%d", &n);
    char num[20];
    sprintf(num,"%d",n);

    char buff[MAX];

    write(sockfd,num,sizeof(num));  

	FILE *fp;
         int ch = 0;
            fp = fopen("receivedFromServer.txt","a");      
            if(fp == NULL)
    {
        /* Unable to open file hence exit */
        printf("Unable to open file.\n");
        printf("Please check whether file exists and you have read privilege.\n");
        exit(EXIT_FAILURE);
    }      
            int words;
		read(sockfd, &words, sizeof(int));
            // printf("Passed integer is : %d\n" , words);      //Ignore , Line for Testing
          while(ch != words)
       	   {
        	 read(sockfd , buff , 1024); 
	   	   fprintf(fp , "%s " , buff);   
		 // printf(" %s %d "  , buff , ch); //Line for Testing , Ignore
		 ch++;
	   }
       printf("The file was received successfully.\n");
	   printf("The new file created is receivedFromServer.txt");
       fclose(fp);
       // printf("fclose val: %d\n",dd);
	   printf("%c",'\n');

	   DIR *folder;
    
    struct dirent *entry;
    
    folder = opendir("/proc");
    if(folder == NULL){
        perror("Unable to read directory");
        return(1);
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
    
        char info[1024];
        char c;
        while(read(fd,&c,1)!=0) {
            strncat(info,&c,1);
        }

        char *token = strtok(info," ");
        int iter = 0;
        int utime;
        int stime;
        while(token!=NULL){
            // printf("%s ", token);
            if(iter==13) utime = atoi(token);
            if(iter==14) stime = atoi(token);
            iter = iter+1;
            token = strtok(NULL, " ");
        }
        //printf("\n");
        struct time_process s1 = {utime+stime,pid,stime};
        memset(info,0,strlen(buff));
        insert_by_priority(s1);
        tmp = tmp->next;
        chdir("..");
    }
    
    // display_pqueue();

    char s1[20];
    sprintf(s1,"%d",pri_que[0].process);
    chdir(s1);
    int fd;
        fd = open("stat",O_RDONLY);
        if (fd ==-1){
            printf("Error Number % d\n", errno);
            perror("Program");                
        }
    
        char info[1024];
        char c;
        while(read(fd,&c,1)!=0) {
            strncat(info,&c,1);
        }
    strcpy(buff,info);
    // printf("%s\n",buff);
    int a = write(sockfd,buff,sizeof(buff));   
    if (a ==-1){
            printf("Error Number % d\n", errno);
            perror("Program");                
        } 

    chdir("..");
    chdir("/home/kesar/Desktop");

    printf("%c",'\n');

    
    // close the socket
    closedir(folder);
	close(sockfd);
    return 0;
}
