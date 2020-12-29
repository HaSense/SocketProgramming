#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char* argv[])
{
	int serv_sock, cli_sock;
	char message[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_adr;
	struct sockaddr_in cli_adr;
	socklen_t cInt_adr_sz;

	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1])); 

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	cInt_adr_sz = sizeof(cli_adr);

	for(i = 0; i<5; i++)
	{
		cInt_sock = accept(serv_sock, (struct sockaddr*)&cli_adr, &cli_adr_sz);
		if(cli_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		printf("Client IP : %s, PORTNO : %d\n", inet_ntoa(cli_adr.sin_addr), cli_adr.sin_port);
		while((str_len = read(cli_sock, message, BUF_SIZE)) != 0)
			write(cli_sock, message, str_len);

		close(cli_sock);
	}

	close(serv_sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
