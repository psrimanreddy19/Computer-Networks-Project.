#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<arpa/inet.h>
int main()
{
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	int port=40000;
	char buffer[100];
	if(sockfd<0)
	{
		printf("Socket not created\n");
		return 0;
	}
	struct sockaddr_in server_addr;
	memset(&server_addr,'\0',sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	server_addr.sin_port=htons(port);
	printf("%s %d\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));
	int p=connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(p<0)
	{
		printf("Didnt connect\n");
		return 0;
	}
	int n;
	int inside=0;
	while(1)
	{
		bzero(buffer,sizeof(buffer));
		n=read(sockfd,buffer,sizeof(buffer));
		printf("Server %s\n",buffer);
		if(strcmp("BYE",buffer)==0)
			return 0;
		while(1)
		{
			bzero(buffer,sizeof(buffer));
			scanf("%s",buffer);
			if(strcmp("P",buffer)==0)
				break;
			n=write(sockfd,buffer,sizeof(buffer));
			bzero(buffer,sizeof(buffer));
			n=read(sockfd,buffer,sizeof(buffer));
			printf("Server::: %s\n",buffer);
			bzero(buffer,sizeof(buffer));
			scanf("%s",buffer);
			n=write(sockfd,buffer,sizeof(buffer));
			bzero(buffer,sizeof(buffer));
			n=read(sockfd,buffer,sizeof(buffer));
			printf("Server::::::: %s\n",buffer);
			bzero(buffer,sizeof(buffer));
		/*	n=read(sockfd,buffer,sizeof(buffer));
			printf("From Server:: %s\n",buffer);
			bzero(buffer,sizeof(buffer));*/
		/*	scanf("%s",buffer);
			n=write(sockfd,buffer,sizeof(buffer));*/
			break;
		}

	}
	close(sockfd);
}

