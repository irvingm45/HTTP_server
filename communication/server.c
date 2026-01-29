#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#define MYPORT "3490"
#define BACKLOG 10

int main(){
	struct addrinfo hints, *res;
	int sockfd;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		// IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;		// fill in my IP for me

	getaddrinfo(NULL, MYPORT, &hints, &res);

	// Obtenemos el file descriptor
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sockfd == -1){
		printf("Error con la obtención del socket\n");
		return 0;
	}

	if(bind(sockfd, res->ai_addr, res->ai_addrlen) == -1){
		printf("Error con la asignación al puerto\n");
		return 0;
	}
	if(listen(sockfd, BACKLOG) == -1) {
		printf("Error en la escucha del puerto\n");
		return 0;
	}

	// Manage incoming connections
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	int new_fd;

	addr_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
}
