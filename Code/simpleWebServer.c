//Be nam _e_ khoda

/* Note:
	1- Read the code to understand what it does and try to run it 
	2- Some return-values have not been checked, so you need to check them for your final implementation
	3- This code has been tested on Ubuntu 18.04 and Mozilla Firefox 99
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/sendfile.h>
#include <fcntl.h>


#define PORTNUM		8080
#define BUFSIZE		2048

//uncomment below line or use -DDEBUG as your compiler flag to print received packet's content
//#define DEBUG


char webpage[] =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title>OSLab</title><link rel=\"icon\" href=\"irFlag.png\">\r\n"
"<style>body {background-color: #099fb0}</style></head>\r\n"
"<body><center><h1>Operating System Lab Final Project</h1><br>\r\n"
"<img src=\"fetr.jpg\"></center></body></html>\r\n";


int main(int argc, char *argv[])
{
	int fdimg;
	int fd_server;
	int fd_client;
	socklen_t sin_len;
	char buf[BUFSIZE];
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	sin_len = sizeof(client_addr);

	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0)
	{
		perror("socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORTNUM);

	if(bind(fd_server, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind");
		close(fd_server);
		exit(1);
	}

	if(listen(fd_server,10) == -1)
	{
		perror("listen");
		close(fd_server);
		exit(1);
	}

	while(1)
	{		
		fd_client = accept(fd_server, (struct sockaddr *)&client_addr, &sin_len);
		
		if(fd_client == -1)
		{
			perror("Connection Failed! Can't Connect to Client ... \n");
			continue;
		}
		printf("Client Connection Accepted ... \n");
	
		memset(buf, 0, BUFSIZE);
		read(fd_client, buf, BUFSIZE);

		#ifdef DEBUG
		printf("%s", buf);
		#endif

		if(!strncmp(buf, "GET /irFlag.png", 15))
		{
			fdimg = open("../Resources/irFlag.png", O_RDONLY);
			sendfile(fd_client, fdimg, NULL, 200000);
			close(fdimg);
		}
		else if(!strncmp(buf, "GET /fetr.jpg", 13))
		{
			fdimg = open("../Resources/fetr.jpg", O_RDONLY);
			sendfile(fd_client, fdimg, NULL, 60000);
			close(fdimg);	
		}
		else
		{
			write(fd_client, webpage, sizeof(webpage)-1);
		}
	
		close(fd_client);
		printf("Client Connection Closed ... \n");
	}

	return 0;	
}