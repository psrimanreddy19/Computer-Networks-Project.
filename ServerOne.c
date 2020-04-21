#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
int main()
{
	int port=40000;
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	char buffer[100];
	int newsocketfd;
	struct sockaddr_in server_addr,client_addr;
	if(sockfd<0)
	{
		printf("The socket didnt get created\n");
		return 0;
	}
	memset(&server_addr,'\0',sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	server_addr.sin_port=htons(port);
	if(bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
	{
		printf("Not binded\n");
		return 0;
	}
	listen(sockfd,5);
	int length=sizeof(client_addr);
	int n;

	fd_set master;
	int clientSocket[10];
	int i;
	for(i=0;i<10;i++)
	{
		clientSocket[i]=0;
	}
	int fd;
	int player=0;
	int answer=0;
	int savedFd;
	int max;
	int EnteredOnce=0;
	int activity;
	int readSocket,writeSocket;
	while(1)
	{
		FD_ZERO(&master);
		FD_SET(sockfd,&master);
		max=sockfd;
		for(i=0;i<10;i++)
		{
			fd=clientSocket[i];
			if(fd>0)
			{
				FD_SET(fd,&master);
			}
			if(fd>max)
			{
				max=fd;
			}
		}
		if(player>=2 && answer==0)
                {
                        char question[100];
                        bzero(question,sizeof(question));
                        printf("Write the question\n");
                        scanf("%s",question);
                        for(i=0;i<10;i++)
                        {
                                if(clientSocket[i]!=0)
                                {
                                        writeSocket=write(clientSocket[i],question,sizeof(question));
                                }
                        }
                        answer=1;
                        if(strcmp("BYE",question)==0)
                                return 0;
                }

		activity=select(max+1,&master,NULL,NULL,NULL);
		
		if(FD_ISSET(sockfd,&master))
		{
			int len=sizeof(server_addr);
			int newSocket=accept(sockfd,(struct sockaddr*)&server_addr,&len);
			for(i=0;i<10;i++)
			{
				if(clientSocket[i]==0)
				{
					clientSocket[i]=newSocket;
					break;
				}
			}
			player++;
			printf("Connection is established from %s and %d port\n",inet_ntoa(server_addr.sin_addr),ntohs(server_addr.sin_port));
		}

		int pass=1;
		for(i=0;i<10;i++)
		{
			if(FD_ISSET(clientSocket[i],&master))
			{
				EnteredOnce=1;
				answer=0;
				bzero(buffer,sizeof(buffer));
				readSocket=read(clientSocket[i],buffer,sizeof(buffer));
				printf("%d : %s\n",clientSocket[i],buffer);
				if(strcmp("BYE",buffer)==0)
					return 0;
				if(strcmp("1",buffer)==0)
				{
					char answer[100]="Write you answer";
					writeSocket=write(clientSocket[i],answer,sizeof(answer));
					bzero(answer,sizeof(answer));
					bzero(buffer,sizeof(buffer));
					printf("Write answer to the question\n");
					scanf("%s",answer);
					n=read(clientSocket[i],buffer,sizeof(buffer));
					char msg[100]="You have scored";
					char wrongmsg[100]="You did not score";
					if(strcmp(buffer,answer)==0)
					{
						n=write(clientSocket[i],msg,sizeof(msg));
						bzero(msg,sizeof(msg));
					}
					else
					{
						n=write(clientSocket[i],wrongmsg,sizeof(wrongmsg));
						bzero(wrongmsg,sizeof(wrongmsg));
					}
				}
				break;

			}
		}
	}
}
