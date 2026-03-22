#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "http_request.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

class HttpResponse{
public:
    // Default constructor
    HttpResponse() = default;

    // Setters
    HttpResponse &setVersion(const std::string &value){
        version = value;
        return *this;
    }
    HttpResponse &setStatus(const std::string &value){
        status_code = value;
        return *this;
    }
    HttpResponse &setHeaders(const std::string &key, const std::string &value){
        headers[key] = value;
        return *this;
    }
    HttpResponse &setBody(const std::string &value){
        body = value;
        return *this;
    }

    // Functions
    // Function that creates the response
    void buildResponse(const HttpRequest &req, std::string &directoryPath);
    std::string responseToString() const;
    void printResponse() const;

private:
    std::string version;
    std::string status_code;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    // Functions for building the response
    std::string pathForm(const std::string &path) const;
    std::string choping(const std::string &path) const;
    void setNotFound();
    void buildEcho(const HttpRequest &rq);
    void buildUserAgent(const HttpRequest &rq);
    std::string extractName(const std::string &filePath);
    void postFileRequested(const HttpRequest &rq, const std::string &filePath);
    void getFileRequested(const HttpRequest &rq, const std::string &filePath);
    void buildFile(const HttpRequest &rq, const std::string &directoryPath);
};

#endif