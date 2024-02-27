#pragma once

#include <string>
#include <map>

class Response
{
private:
	int statusCode;
	std::map<std::string, std::string> headers;
	std::string body;

public:
	Response();
	void setStatusCode(int code);
	void setHeader(const std::string &key, const std::string &value);
	void setBody(const std::string &body);
	std::string toString() const;
};
