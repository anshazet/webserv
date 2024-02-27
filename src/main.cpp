#include "HttpServer.h"
#include "request/Request.h"
#include "response/Response.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

int main(int ac, char **av)
{
	HttpServer server;
	server.init("127.0.0.1", 8080);

	// Assuming server.init() has setup the listening socket and it's accessible here
	int listenFd = server.getListenFd(); // You need to implement getListenFd() in HttpServer

	// Set the listening socket to non-blocking
	fcntl(listenFd, F_SETFL, O_NONBLOCK);

	struct pollfd fds[200]; // Adjust size based on expected number of concurrent connections
	int nfds = 1, currentSize = 1;

	memset(fds, 0, sizeof(fds));
	fds[0].fd = listenFd;
	fds[0].events = POLLIN;

	while (true)
	{
		int pollCount = poll(fds, currentSize, -1); // Timeout is set to -1 (wait indefinitely)

		if (pollCount == -1)
		{
			perror("poll");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < currentSize; i++)
		{
			if (fds[i].revents == 0)
				continue;

			if (fds[i].revents != POLLIN)
			{
				std::cerr << "Error! revents = " << fds[i].revents << std::endl;
				continue;
			}

			if (fds[i].fd == listenFd)
			{
				// Accept new connection
				int clientFd = accept(listenFd, NULL, NULL);
				if (clientFd < 0)
				{
					perror("accept");
					continue;
				}

				fcntl(clientFd, F_SETFL, O_NONBLOCK); // Set to non-blocking
				fds[currentSize].fd = clientFd;
				fds[currentSize].events = POLLIN;
				currentSize++;

				std::cout << "New connection accepted: " << clientFd
						<< std::endl;
			} else
			{
				// Handle client request
				std::string requestText = server.readRequest(fds[i].fd);
				if (!requestText.empty())
				{
//					Request request(requestText);
					Response response;
					response.setStatusCode(200);
					response.setHeader("Content-Type", "text/html");
					response.setBody(
							"<html><body><h1>Hello, World!</h1></body></html>");
					server.sendResponse(fds[i].fd, response.toString());
				}
				close(fds[i].fd); // Close connection after handling
				fds[i].fd = -1;	  // Mark as closed
			}
		}
	}

	return 0;
}

/*
 int main(int ac, char **av)
 {
 HttpServer server;
 server.init("127.0.0.1", 8080);

 //	if(ac == 2){
 //		Harl harl = Harl();
 //		harl.complain(av[1]);
 //	}else{
 //		std::cerr << "Nombre d'arguments incorrect." << std::endl;
 //	}

 std::string requestText = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
 Request request(requestText);

 std::cout << "Parsed Request Method: " << request.method << std::endl;
 std::cout << "Parsed Request URL: " << request.url << std::endl;

 return 0;
 }
 */
