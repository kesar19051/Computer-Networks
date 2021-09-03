#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
	char buff[MAX];
	int n;
	read(sockfd, buff, sizeof(buff));
	printf("From Server : %s", buff);
	
}

int main()
{
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

	char buff[MAX];
	FILE *fp;
         int ch = 0;
            fp = fopen("glad_receive.txt","a");      
            if(fp == NULL)
    {
        /* Unable to open file hence exit */
        printf("Unable to open file.\n");
        printf("Please check whether file exists and you have read privilege.\n");
        exit(EXIT_FAILURE);
    }      
            int words;
		read(sockfd, &words, sizeof(int));
            printf("Passed integer is : %d\n" , words);      //Ignore , Line for Testing
          while(ch != words)
       	   {
        	 read(sockfd , buff , 1024); 
	   	 fprintf(fp , " %s" , buff);   
		 // printf(" %s %d "  , buff , ch); //Line for Testing , Ignore
		 ch++;
	   }
     printf("The file was received successfully\n");
	   printf("The new file created is glad_received.txt");
	   printf("%c",'\n');
	
	// close the socket
	close(sockfd);
}
