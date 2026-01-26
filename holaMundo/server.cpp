#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

int main(){
	int server_fd, 		new_socket;
    struct sockaddr_in 	address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Socket creation
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configurar el puerto y la IP (INADDR_ANY escucha en todas tus IPs locales)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(8080); // Puerto 8080

    // 3. Pegar el socket al puerto (Bind)
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // 4. Escuchar conexiones
    listen(server_fd, 3);
    cout << "Esperando en el puerto 8080..." << endl;

    // 5. Aceptar la conexión
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    // 6. Leer el mensaje
    read(new_socket, buffer, 1024);
    cout << "Mensaje del remitente: " << buffer << endl;

    close(new_socket);
    close(server_fd);
    return 0;
}
