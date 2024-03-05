#pragma once
#include <string>
#include "../request/Request.h"
#include "../response/Response.h"

class ConnectorEvent
{
private:
	Request *req;
	Response *resp;
	std::string temp;
	int clientFd; // File descriptor for the client

public:
	//	ConnectorEvent& operator=(const ConnectorEvent &other);
	//	ConnectorEvent(Request req, Response resp);
	// ConnectorEvent(std::string temp);
	//	ConnectorEvent& operator=(const ConnectorEvent &other);
	ConnectorEvent(Request *req, Response *resp);
	ConnectorEvent(const ConnectorEvent &other);
	ConnectorEvent(std::string temp);
	~ConnectorEvent();
	std::string getTemp();
	int getClientFd() const;
};
