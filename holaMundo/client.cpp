#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
using namespace std;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char* hello = "Hola";

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convertir IP de texto a binario (inet_pton que vimos antes)
    inet_pton(AF_INET, "172.31.14.223", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock, hello, strlen(hello), 0);
    cout << "Mensaje enviado" << endl;

    close(sock);
    return 0;
}
