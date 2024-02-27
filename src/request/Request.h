#pragma once
#include <string>
#include <map>

#include <sstream>

class Request
{
public:
	virtual ~Request(){};

	virtual std::string getValue(std::string paramName) = 0;
	virtual void addParam(std::string paramName, std::string paramValue) = 0;
	virtual std::string getUri() = 0;
	virtual std::string getMethod() = 0;
	virtual void dump() = 0;
};

// class Request
// {
// private:
// 	void parseRequestLine(std::istringstream &stream);
// 	void parseHeaders(std::istringstream &stream);
// 	void parseBody(std::istringstream &stream);

// public:
// 	Request();
// 	virtual ~Request();

// 	Request(const std::string &requestText);
// 	std::string method;
// 	std::string url;
// 	std::string httpVersion;
// 	std::map<std::string, std::string> headers;
// 	std::string body;

// 	virtual std::string getValue(std::string paramName) = 0;
// 	virtual void addParam(std::string paramName, std::string paramValue) = 0;
// 	virtual std::string getUri() = 0;
// 	virtual std::string getMethod() = 0;
// 	virtual void dump() = 0;
// };

// class Request
// {
// private:
// public:
// 	Request();
// 	~Request();
// 	//	Request(Request const &o);
// 	//	Request& operator=(Request const &o);

// 	std::string getValue(std::string paramName);
// 	void addParam(std::string paramName, std::string paramValue);
// };
