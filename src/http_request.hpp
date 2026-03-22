#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <iostream>
#include <string>
#include <unordered_map>

class HttpRequest {
public:
    // Default constructor
    HttpRequest() = default;
    
    // Getters
    std::string getMethod() const { return method; }
    std::string getPath() const { return path; }
    std::string getVersion() const { return version; }
    std::string getHeader(std::string key) const;
    std::string getBody() const { return body; }
    
    // Functions
    // Creates a function that belongs to a class but not to an object
    void parseRequest(const std::string &rawData);
    void printRequest();

private:
    // Attributes
    std::string method;
    std::string path;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

#endif