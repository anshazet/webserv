/*
 * HttpServer.cpp
 *
 *  Created on: 5 févr. 2024
 *      Author: fbourgue
 */

#include "HttpServer.h"
#include "connector/TcpConnector.h"

HttpServer::HttpServer() :
		harl(), connector()
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

void HttpServer::onDataReceiving(ConnectorEvent e)
{
//	std::cout << e.getTemp();
	std::string rawRequest = e.getTemp();
	Request *req = RequestFactory().build(&rawRequest);
//	req->dump();

//	Validator *validator = ValidatorFactory().build(req);
//	validator->validate(req);

	Processor *processor = ProcessorFactory().build(req);
	Response *resp = processor->process(req);

//Send Response
}

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
	} else if (nbytes < 0)
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
	TcpConnector *tcpConnector = dynamic_cast<TcpConnector*>(connector);
	if (tcpConnector)
	{
		return tcpConnector->getListenFd();
	} else
	{
		std::cerr << "Connector is not properly initialized or wrong type."
				<< std::endl;
		return -1;
	}
}
