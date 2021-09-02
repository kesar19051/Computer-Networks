#include <stdio.h>
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


#define MAX 500

struct time_process{
    int time;
    int process;
};


void insert_by_priority(struct time_process);
void delete_by_priority(struct time_process);
void create();
void check(struct time_process);
void display_pqueue();
//
struct time_process endData ={-1, -1};

struct time_process pri_que[MAX];

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


int main()
{

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
    printf("%d\n", files);
	//printf("%c",'\n');
	//printf("%c",'\n');
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
        while(token!=NULL){
            if(iter==13) utime = atoi(token);
            if(iter==14) stime = atoi(token);
            iter = iter+1;
            token = strtok(NULL, " ");
        }
        memset(buff,0,strlen(buff));
        //printf("\n");
        struct time_process s1 = {utime+stime,pid};
        insert_by_priority(s1);
        tmp = tmp->next;
        chdir("..");
    }

    display_pqueue();
    closedir(folder);

    return(0);
}
