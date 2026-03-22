#include "http_request.hpp"
#include "http_response.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unordered_map>
#include <algorithm>

void handleClient(int clientSocket, std::string filePath){
  // Request handling in origin form (compact version)
  char buffer[1024];
  int bytesReceived = read(clientSocket, buffer, sizeof(buffer));
  if(bytesReceived <= 0){
    std::cout << "Error in read\n";
    return;
  }

  // Convert the buffer to a string for easier manipulation
  std::string rawData(buffer, bytesReceived);
  std:: cout << "Raw request data:\n" << rawData << "\n" << "------------------\n";
  
  // We parse each part of the request
  HttpRequest request; request.parseRequest(rawData);
  request.printRequest();
  
  // We build the response
  HttpResponse response; response.buildResponse(request, filePath);
  std::string ans = response.responseToString();
  
  if(send(clientSocket, ans.c_str(), ans.size(), 0) == -1){
    std::cout << "Error in send\n";
    return;
  }
  
  close(clientSocket);
}

struct sockaddr_in initializeServerAddress() {
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);
  return server_addr;
}

int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  // Handle the arguments of main
  std::string filePath;
  for (int i = 1; i < argc; i++){
    if( (std::string(argv[i]) == "--directory" || std::string(argv[i]) == "-d") && i + 1 < argc){
      filePath = argv[i + 1];
      std::cout << "File path set to: " << filePath << "\n";
      break;
    }
    else if ( (std::string(argv[i]) == "--directory" || std::string(argv[i]) == "-d") && i + 1 >= argc){
      std::cout << "Missing file path\n";
      return 0;
    }
  }

  // You can use print statements as follows for debugging, they'll be visible when running tests.
  std::cout << "Logs--------------!\n";
  
  // Creation of de Server socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) { // Error handler
    std::cerr << "Failed to create server socket\n";
    return 0;
  }
  
  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return 0;
  }
  
  struct sockaddr_in server_addr = initializeServerAddress();
  // We bind the port for receive
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) { // Bind error handler
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }
  
  // We the maximum requests
  int connection_backlog = SOMAXCONN;
  if (listen(server_fd, connection_backlog) != 0) { // Error handler in listen()
    std::cerr << "listen failed\n";
    return 1;
  }

  // Now we create a loop for receiving infinitely request
  while(true){

    // Socket address of the client
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    std::cout << "Waiting for a client to connect...\n";
    
    //  Client socket
    int clientSocket = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
    if(clientSocket == -1){
      std::cout << "Error stablishing a connection with client\n";
      continue;
    }
    std::cout << "Client connected\n";
  
    // Handle the client
    std::thread t(handleClient, clientSocket, filePath);
    t.detach();
  }

  close(server_fd);
  return 0;
}
