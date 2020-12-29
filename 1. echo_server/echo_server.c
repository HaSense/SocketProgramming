#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//버퍼 사이즈를 1024바이트로 설정
#define BUF_SIZE 1024

//에러처리 함수
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int serv_sock, cli_sock;    //서버소켓, 클라이언트소켓 선언
	char message[BUF_SIZE];     //메세지는 1024바이트 문자배열
	int str_len, i;             //문자열 길이와 i변수 선언

	struct sockaddr_in serv_adr;    //서버 주소설정을 위한 구조체 선언
	struct sockaddr_in cli_adr;     //클라이언트 주소설정을 위한 구조체 선언
	socklen_t cli_adr_sz;          //부호없는 정수형 소켓의 길이표현

	if(argc != 2){                  //IP와 Port를 입력해야 되는데 하나만 입력하거나 3개이상 입력시 에러 발생
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

    //함수를 이용하여 서버소켓 만들기
    //PF_INET(프로토콜 체계) == AF_INET(주소 체계) 둘다 IPV4를 뜻합니다.
    //SOCK_STREAM --> TCP를 뜻합니다. 정수값은 1
    //protocol값을 지정 0이면 TCP IPPROTO_TCP(TCP 일때), IPPROTO_UDP(UDP 일때
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	
    if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1])); 

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	cli_adr_sz = sizeof(cli_adr);

	for(i = 0; i<5; i++)
	{
		cli_sock = accept(serv_sock, (struct sockaddr*)&cli_adr, &cli_adr_sz);
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
    printf("hello server\n");
	return 0;
}


void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
