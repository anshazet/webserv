#include "ConnectorEvent.h"

ConnectorEvent::~ConnectorEvent()
{
}

ConnectorEvent::ConnectorEvent(const ConnectorEvent &other) : req(other.req), resp(other.resp), temp(other.temp)
{
}
ConnectorEvent::ConnectorEvent(std::string temp) : temp(temp), req(NULL), resp(/* default state */)
{
}

ConnectorEvent::ConnectorEvent(Request *req, Response *resp)
{
	this->req = req;
	this->resp = resp;
}

std::string ConnectorEvent::getTemp()
{
	return temp;
}

int ConnectorEvent::getClientFd() const
{
	return clientFd;
}
