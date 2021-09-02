#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#define MAX 1024
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
// void func(int sockfd)
// {
// 	char buff[MAX];
// 	int n;
// 	// infinite loop for chat
// 	int words = 0;
// 	FILE *fp;
// 	fp = fopen("glad.txt","r");

// 	bzero(buff, MAX);
// 	printf("%s\n", buff);
// 	char ch;
// 	while(ch!='*'){
// 		fscanf(fp, "%s", buff);
// 		write(sockfd, buff,255);
// 		ch = fgetc(fp);
// 	}
// 	printf("%s\n", buff);
// 	fclose(fp);


		//read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		// printf("From client: %s\t To client : ", buff);
		// bzero(buff, MAX);
		// n = 0;
		// // copy server message in the buffer
		// while ((buff[n++] = getchar()) != '\n')
		// 	;

		// // and send that buffer to client
		// write(sockfd, buff, sizeof(buff));

		// // if msg contains "Exit" then server exit and chat ended.
		// if (strncmp("exit", buff, 4) == 0) {
		// 	printf("Server Exit...\n");
		// 	break;
		// }
	
//}

// Driver function
int main()
{
	int sockfd, connfd, len;
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
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");



	// Function for chatting between client and server
	// func(connfd);

	char buff[MAX];

	FILE *fp;
	fp = fopen("/home/kesar/Desktop/glad","r");
	if(fp == NULL)
    {
        /* Unable to open file hence exit */
        printf("Unable to open file.\n");
        printf("Please check whether file exists and you have read privilege.\n");
        exit(EXIT_FAILURE);
    }
	bzero(buff, MAX);
	char str[] = "heelo";
	strcat(buff,str);
	printf("%s\n", buff);
	char ch;
	while(ch!='*'){
		fscanf(fp, "%s", buff);
		write(sockfd, buff,255);
		ch = fgetc(fp);
		//printf("%s\n", buff);
	}
	fclose(fp);
	
	

	// After chatting close the socket
	close(sockfd);
}
