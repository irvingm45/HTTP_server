#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

int main(){
	// Conseguimos el dominio
	char dominio[100]; printf("Dame algun dominio\n");
	fgets(dominio, sizeof(dominio), stdin);
	// Quitamos el salto de línea al final por fgets
	dominio[strcspn(dominio, "\n")] = 0;
	printf("Dominio %s ingresado\n", dominio);

	// Estructura "hints" que nos sirve como cuerpo de lo que pedimos y "res" para
	// guardar el puntero a la linked list
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC; // COMODIN de los tipos de IP
	hints.ai_socktype = SOCK_STREAM;	// Tipo de socket
	// hints.ai_family = AF_INET6; // Forzar algun tipo de IP
	
	// Ahora obtenemos la linked list en base a "hints" y guardandolo en res
	int status = getaddrinfo(dominio, NULL, &hints,  &res);
	if(status != 0) {
		printf("Fallo en la obtencion de direcciones\n");
		return 0;
	}

	// Ya tenemos la linked list en "res"
	struct addrinfo *aux = res; // Para recorrer la lista
	char ipstr[INET6_ADDRSTRLEN];

	while(aux != NULL) {
		void *addr;
		char *ipver;

		if(aux->ai_family == AF_INET) {
			addr = &( ( (struct sockaddr_in *)aux->ai_addr )->sin_addr );
			ipver = "IPv4";
		}
		else if(aux->ai_family == AF_INET6) {
			addr = &( ( (struct sockaddr_in6 *)aux->ai_addr )->sin6_addr );
			ipver = "IPv6";
		}

		// Pasamos la IP de binario a texto
		inet_ntop(aux->ai_family, addr, ipstr, sizeof ipstr);
		printf("%s: %s\n", ipstr, ipver);

		// Seguimos al siguiente nodo
		aux = aux->ai_next;
	}

	freeaddrinfo(res);
	return 0;
}
