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

/*
void HttpServer::onDataReceiving(ConnectorEvent e)
{
	std::string rawRequest = e.getTemp();
	// Create a pointer to rawRequest to match the expected signature of RequestFactory::build
	Request *req = RequestFactory().build(&rawRequest); // Now passing the address of rawRequest

	if (req != NULL)
	{ // Use NULL instead of nullptr for C++98 compatibility
		Processor *processor = ProcessorFactory().build(req);
		if (processor != NULL)
		{
			Response *resp = processor->process(req);
			if (resp != NULL)
			{
				std::string responseText = resp->toString();
				// Assume you have a method to send the response back to the client
				// sendResponse(clientFd, responseText); // Example placeholder

				delete resp; // Clean up dynamically allocated memory
			}
			delete processor; // Clean up
		}
		delete req; // Clean up
	}
	else
	{
		// Handle error scenario, maybe log an error or send a default error response
	}
}
*/