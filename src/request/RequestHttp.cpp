/*
 * RequestHttp.cpp
 *
 *  Created on: 22 févr. 2024
 *      Author: fbourgue
 */

#include "RequestHttp.h"

RequestHttp::RequestHttp()
{
	// TODO Auto-generated constructor stub
}

RequestHttp::~RequestHttp()
{
	// TODO Auto-generated destructor stub
}

// RequestHttp::RequestHttp(const RequestHttp &other)
//{
//	// TODO Auto-generated constructor stub
//
// }
//
// RequestHttp& RequestHttp::operator=(const RequestHttp &other)
//{
//	// TODO Auto-generated method stub
//
// }
RequestHttp::RequestHttp(std::string *rawContent)
{

	std::stringstream lines;
	lines.str(rawContent->c_str());
	char key[2048], val[2048], line[2048];
	while (lines)
	{
		lines.getline(line, 2048, '\n');
		std::stringstream lineSs;
		lineSs.str(line);
		std::cout << key << " -> " << val << std::endl;
		std::string lineStr = lineSs.str();
		if (!lineStr.compare(0, 3, "GET"))
		{
			method = "GET";
			lineSs.getline(key, 2048, ' ');
			lineSs >> key;
			uri = key;
		}
		else
		{
			lineSs.getline(key, 2048, ':');
			lineSs >> val;
			kv[std::string(key)] = std::string(val);
		}
	}
}

void RequestHttp::dump() const
{
	std::map<std::string, std::string>::const_iterator ite = kv.begin();
	while (ite != kv.end())
	{
		std::cout << ite->first << " -> " << ite->second << std::endl;
		ite++;
	}
}

std::string RequestHttp::getValue(std::string paramName) const
{
	std::map<std::string, std::string>::const_iterator it = kv.find(paramName);
	if (it != kv.end())
	{
		return it->second;
	}
	else
	{
		return ""; // Return an empty string if the parameter is not found
	}
}

void RequestHttp::addParam(std::string paramName, std::string paramValue)
{
	kv[paramName] = paramValue;
}

std::string RequestHttp::getUri() const
{
	return uri;
}

std::string RequestHttp::getMethod() const
{
	return method;
}
