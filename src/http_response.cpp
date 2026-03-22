#include "http_response.hpp"
#include <iostream>
#include <string>
#include <fstream>

void HttpResponse::buildResponse(const HttpRequest &req, std::string &directoryPath) {
    this->setVersion(req.getVersion());

    // We get the form of the path
    std::string form = pathForm(req.getPath());

    // We verify if the path is in echo form
    if(form == "/echo")
        buildEcho(req);
    // We verify if the path is in user-agent form
    else if(form == "/user-agent")
        buildUserAgent(req);
    else if(form == "/files")
        buildFile(req, directoryPath);
    else if (form == "/tmp"){
        this->setStatus("200 OK");
    }
    else if(form == "/"){
        std::cout << "Is origin\n";
        this->setStatus("200 OK");
    }
    else{
        setNotFound();
        std::cout << "Is not echo, user-agent, files or not origin\n";
    }

    // Debug
    printResponse();
}

std::string HttpResponse::responseToString() const {
    // Version and status code
    std::string ans = version + ' ' + status_code + "\r\n";
    // Headers
    for (const auto &[key, value] : headers) {
        ans += key + ": " + value + "\r\n";
    }
    ans += "\r\n"; // Blank line to separate headers from body

    // Body
    ans += body;
    return ans;
}

void HttpResponse::printResponse() const {
    std::cout << "-----Response-----\n";
    std::cout << "Version: " << version << "\n";
    std::cout << "Status Code: " << status_code << "\n";
    std::cout << "Headers:\n";
    for(const auto &header : headers) {
        std::cout << header.first << ": " << header.second << "\n";
    }
    std::cout << "---Body---\n" << body << "\n------------\n";
}

std::string HttpResponse::pathForm(const std::string &path) const {
    int n = path.size(), contador = 1;
    
    // We find the first slash
    while(path[contador] != '/' && contador < n) contador++;

    // We return the form of the path (origin, echo, or absolute)
    return path.substr(0, contador);
}

std::string HttpResponse::choping(const std::string &path) const {
    int n = path.size(), contador = 0;
    for(int i = 0; i < n; i++){
        // After the second slash, we substract the rest of the string
        if(contador == 2) return path.substr(i, n-i);
        if(path[i] == '/') contador++;
    }
    return "";
}

void HttpResponse::setNotFound(){
    this->setStatus("404 Not Found");
    this->setHeaders("Content-Length", "0");
}

void HttpResponse::buildEcho(const HttpRequest &rq) {
    std::cout << "Is echo\n";

    // We just get the last part of the path
    std::string pathChoped = choping(rq.getPath());

    this->setStatus("200 OK");
    this->setHeaders("Content-Type", "text/plain");
    this->setHeaders("Content-Length", std::to_string(pathChoped.size()));
    this->setBody(pathChoped);     
}

void HttpResponse::buildUserAgent(const HttpRequest &rq) {
    std::cout << "Is user-agent\n";

    std::string userAgent = rq.getHeader("user-agent");
    this->setStatus("200 OK");
    this->setHeaders("Content-Type", "text/plain");
    this->setHeaders("Content-Length", std::to_string(userAgent.size()));
    this->setBody(userAgent);
}

std::string HttpResponse::extractName(const std::string &filePath){
    int lastSlash = 0;
    int n = filePath.size();
    for(int i = 0; i < n; i++)
        if(filePath[i] == '/')
            lastSlash = i;
    
    return filePath.substr(lastSlash+1);
}

void HttpResponse::postFileRequested(const HttpRequest &rq, const std::string &filePath){
        // Set the status
        this->setStatus("201 Created");
        std::cout << "Is a POST\n";

        // We get the body that is the file content
        std::string bd = rq.getBody();
        this->setHeaders("Content-Length", std::to_string(bd.size()));
        this->setBody(bd);
        
        // We get the file name which is after the last slash
        std::string fileName = choping(rq.getPath());
        std::cout << "File Path: " << filePath << '\n';
        std::cout << "File Name: " << fileName << '\n';

        // We now create the file
        std::ofstream newFile(filePath);
        if(newFile.is_open()) {
            // We put the content
            newFile << rq.getBody();
            // Debug the content
            newFile.close();
            std::cout << "Content file: " << rq.getBody() << '\n';
        }
        else {
            std::cout << "Error\n";
        }
}

void HttpResponse::getFileRequested(const HttpRequest &rq, const std::string &filePath){
    // Set the status
    this->setStatus("200 OK");

    // File variable and verification
    std::ifstream file(filePath);
    if(!file.is_open()){
        setNotFound();
        std::cout << "File not found\n";
        return;
    }

    // Buffer of the file Content
    std::string fileContent;
    getline(file, fileContent, '\0');

    // We get the content-length
    this->setHeaders("Content-Length", std::to_string(fileContent.size()));

    // Body
    this->setBody(fileContent);
}

void HttpResponse::buildFile(const HttpRequest &rq, const std::string &directoryPath) {
    std::cout << "Is file\n";

    // We get the last part of the path which is the file path
    std::string filePath = directoryPath + choping(rq.getPath());

    this->setHeaders("Content-Type", "application/octet-stream");
    
    if(rq.getMethod() == "POST"){
        postFileRequested(rq, filePath);
    }
    else if(rq.getMethod() == "GET"){
        getFileRequested(rq, filePath);
    }
}