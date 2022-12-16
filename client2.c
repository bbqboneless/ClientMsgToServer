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
	char *filename = argv[3];
	struct sockaddr_in serv;
	int port;
	if(argc != 4){
		printf("Not enough parameters \n");
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
		printf("==> sending: ");
		if((fgets(buf,BUFSIZ,stdin)) == NULL){
			perror("fgets\n");
		}
		len = send(sockfd,buf,strlen(buf),0);
		FILE *fp = fopen(filename,"a");
	
		if(fp == NULL){
			printf("Error opening file %s\n",filename);
			return -1;
		}
		//printf("%d\n",len);
		len = recv(sockfd,buf,len,0);
		buf[len] = '\0';
		for(int i = 0;i<len;i++){
			fputc(buf[i],fp);
		}
		fputc('\n',fp);
		printf("received [%s]\n",buf);
		fclose(fp);
	}
	close(sockfd);
	return 0;
}
