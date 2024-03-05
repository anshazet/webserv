/*
 * HttpServer.cpp
 *
 *  Created on: 5 févr. 2024
 *      Author: fbourgue
 */

#include "HttpServer.h"
#include "connector/TcpConnector.h"

HttpServer::HttpServer() : harl(), connector()
{
}

HttpServer::~HttpServer()
{
}

void shutFd(int fd)
{
	if (fd >= 0)
	{
		shutdown(fd, SHUT_RDWR);
		close(fd);
		fd = -1;
	}
}
void HttpServer::init(std::string ipStr, int port)
{
	netStruct ns;
	connector = ConnectorFactory().build(ipStr, port);
	connector->registerIt(this);

	connector->doListen();
}

void HttpServer::onIncomming(ConnectorEvent e)
{
}

// void HttpServer::onDataReceiving(ConnectorEvent e)
// {
// 	//	std::cout << e.getTemp();
// 	std::string rawRequest = e.getTemp();
// 	Request *req = RequestFactory().build(&rawRequest);
// 	//	req->dump();

// 	//	Validator *validator = ValidatorFactory().build(req);
// 	//	validator->validate(req);

// 	Processor *processor = ProcessorFactory().build(req);
// 	Response *resp = processor->process(req);

// 	// Send Response
// }

std::string HttpServer::readRequest(int clientFd)
{
	char buffer[BUF_SIZE];
	std::string requestText;
	int nbytes;

	while ((nbytes = recv(clientFd, buffer, sizeof(buffer), 0)) > 0)
	{
		requestText.append(buffer, nbytes);
	}

	// Check for socket closed or error
	if (nbytes == 0)
	{
		// Connection closed
		std::cout << "Client disconnected." << std::endl;
	}
	else if (nbytes < 0)
	{
		// Error occurred
		std::cerr << "recv() error: " << strerror(errno) << std::endl;
	}

	return requestText;
}

void HttpServer::sendResponse(int clientFd, const std::string &response)
{
	send(clientFd, response.c_str(), response.size(), 0);
}

void HttpServer::closeClient(int clientFd)
{
	shutFd(clientFd);
}

int HttpServer::getListenFd()
{
	TcpConnector *tcpConnector = dynamic_cast<TcpConnector *>(connector);
	if (tcpConnector)
	{
		return tcpConnector->getListenFd();
	}
	else
	{
		std::cerr << "Connector is not properly initialized or wrong type."
				  << std::endl;
		return -1;
	}
}

void HttpServer::onDataReceiving(ConnectorEvent e)
{
	std::string rawRequest = e.getTemp();
	HttpRequest httpRequest(rawRequest);

	if (isCGIRequest(httpRequest.getUri()))
	{
		CGIHandler cgiHandler;
		std::map<std::string, std::string> envVars = prepareCGIEnvironment(httpRequest);
		std::string scriptPath = getScriptPath(httpRequest.getUri());

		std::string cgiOutput = cgiHandler.executeCGIScript(scriptPath);
		std::string httpResponse = generateHttpResponse(cgiOutput);

		sendResponse(e.getClientFd(), httpResponse);
	}
	else
	{
		// Handle non-CGI requests as usual
	}
}

bool HttpServer::isCGIRequest(const std::string &uri)
{
	// Check if URI starts with /cgi-bin/
	return uri.find("/cgi-bin/") == 0;
}

std::map<std::string, std::string> HttpServer::prepareCGIEnvironment(const HttpRequest &request)
{
	size_t queryPos = request.getUri().find('?');
	// Clear existing environment variables
	env.clear();

	// Populate environment variables
	env["REQUEST_METHOD"] = request.getMethod();
	if (queryPos != std::string::npos)
	{
		env["QUERY_STRING"] = request.getUri().substr(queryPos + 1);
	}
	else
	{
		env["QUERY_STRING"] = "";
	}
	env["CONTENT_TYPE"] = request.getValue("Content-Type");
	env["CONTENT_LENGTH"] = request.getValue("Content-Length");
	return env;
}

std::string HttpServer::getScriptPath(const std::string &uri)
{
	// Example: Assuming CGI scripts are located in /var/www/cgi-bin/
	std::string basePath = "/var/www";
	return basePath + uri;
}

std::string HttpServer::generateHttpResponse(const std::string &cgiOutput)
{
	std::string response;

	// Parse CGI output for headers and body
	size_t pos = cgiOutput.find("\r\n\r\n");
	std::string headers = cgiOutput.substr(0, pos);
	std::string body = cgiOutput.substr(pos + 4);

	// Construct HTTP response
	response = "HTTP/1.1 200 OK\r\n" + headers + "\r\n\r\n" + body;
	return response;
}

int HttpServer::getClientFd(int clientId)
{
	std::map<int, int>::const_iterator it = _clients.find(clientId);
	if (it != _clients.end())
	{
		return it->second; // Return the file descriptor for the client
	}
	else
	{
		return -1; // Indicate that the client was not found
	}
}
