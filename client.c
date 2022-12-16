#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
	int sockfd,len;
	char buf[BUFSIZ];
	struct sockaddr_in serv;
	int port;
	if(argc != 3){
		printf("Usage: ./prog host port \n");
		exit(1);
	}
	if((sockfd = socket(PF_INET,SOCK_STREAM,0)) < 0){
		perror("Socket\n");
		exit(1);
	}
	serv.sin_family = PF_INET;
	port = strtol(argv[2],NULL,10);
	serv.sin_port = htons(port);
	inet_aton(argv[1],&serv.sin_addr);
	
	if(connect(sockfd,(struct sockaddr *)&serv,sizeof(serv)) < 0){
		perror("Connect\n");
		exit(1);
	}
	
	while(strncasecmp(buf,"exit\n",5) != 0){
		printf("==>");
		if((fgets(buf,BUFSIZ,stdin)) == NULL){
			perror("fgets\n");
		}
		len = send(sockfd,buf,strlen(buf),0);
		//printf("%d\n",len);
		len = recv(sockfd,buf,len,0);
		buf[len] = '\0';
		printf("<== %s\n",buf);
	}
	close(sockfd);
	return 0;
}
