#pragma once
#include "Request.h"
#include <istream>
#include <iterator>
#include <iostream>

class HttpRequest : public Request
{
private:
    std::string method;
    std::string url;
    std::string httpVersion;
    std::map<std::string, std::string> headers;
    std::string body;

    void parseRequestLine(std::istringstream &stream);
    void parseHeaders(std::istringstream &stream);
    void parseBody(std::istringstream &stream);

public:
    HttpRequest(const std::string &requestText);
    virtual ~HttpRequest(){};

    virtual std::string getValue(std::string paramName);
    virtual void addParam(std::string paramName, std::string paramValue);
    virtual std::string getUri();
    virtual std::string getMethod();
    virtual void dump();
};