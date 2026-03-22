#include "http_request.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

void HttpRequest::parseRequest(const std::string& rawData) {
    std::stringstream ss(rawData);

    // Extract the method, path, and HTTP version
    ss >> method >> path >> version;
    // Skip any extra whitespace or newlines
    while(ss.peek() == ' ' || ss.peek() == '\r' || ss.peek() == '\n') ss.get();

    // Extract headers
    std::string line;
    
    // While we don't get an empty line (which indicates the end of headers), we keep extracting headers
    while(getline(ss, line) && line != "\r") {
        // Remove the trailing \r
        if(!line.empty() && line.back() == '\r') line.pop_back(); 

        // We find the ':'
        size_t colonPos = line.find(':');
        std::string key, value;

                    // npos = no position
        if(colonPos != std::string::npos) {
            // We get the name header in lowercase
            key = line.substr(0, colonPos);
            for(auto &c : key) c = std::tolower(c); // Convert header keys to lowercase for case-insensitivity

            // We skip the ':'
            colonPos++;
            // We skip any spaces after the colons
            while(colonPos < line.size() && line[colonPos] == ' ') colonPos++;
            value = line.substr(colonPos);

            // Save the header in the map
            headers[key] = value;
        }
    }

    // Body
    std::string body;
    if(std::getline(ss, body, '\0')) {
        this->body = body;
    }
}

std::string HttpRequest::getHeader(std::string key) const {
    for(auto &c : key) c = tolower(c);
    
    auto it = headers.find(key);
    if (it != headers.end()) {
        return it->second;
    }

    return "";
}

void HttpRequest::printRequest(){
    std::cout << "-----Request-----\n";
    std::cout << "Method: " << method << "\n";
    std::cout << "Path: " << path << "\n";
    std::cout << "Version: " << version << "\n";
    std::cout << "Headers:\n";
    for(const auto &header : headers)
        std::cout << header.first << ": " << header.second << "\n";
    std::cout << "---Body---\n" << body << "\n------------\n";
}