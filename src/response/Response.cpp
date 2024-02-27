/*
 * Response.cpp
 *
 *  Created on: 19 févr. 2024
 *      Author: fbourgue
 */

#include "Response.h"
#include <sstream>

Response::Response() :
		statusCode(200)
{
}

void Response::setStatusCode(int code)
{
	statusCode = code;
}

void Response::setHeader(const std::string &key, const std::string &value)
{
	headers[key] = value;
}

void Response::setBody(const std::string &body)
{
	std::stringstream ss;
	ss << body.size();
	setHeader("Content-Length", ss.str());
}

std::string Response::toString() const
{
	std::ostringstream responseStream;
	responseStream << "HTTP/1.1 " << statusCode << " "
			<< /* Convert statusCode to reason phrase */"\r\n";

	for (std::map<std::string, std::string>::const_iterator it =
			headers.begin(); it != headers.end(); ++it)
	{
		responseStream << it->first << ": " << it->second << "\r\n";
	}

	responseStream << "\r\n" << body;
	return responseStream.str();
}

// #include "Response.h"

// Response::Response() {
// 	// TODO Auto-generated constructor stub

// }

// Response::~Response() {
// 	// TODO Auto-generated destructor stub
// }

// Response::Response(const Response &o) {
//	*this = o;
//
// }
//
// Response& Response::operator=(const Response &o) {
//	if (this != &o)
//		*this = o;
//	return *this;
//
// }
