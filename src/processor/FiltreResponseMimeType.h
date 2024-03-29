#pragma once
#include "../mimeType/MimeTypeHelper.h"
#include "../request/RequestHttp.h"
#include "../response/ResponseHttp.h"
#include "../response/factory/ResponseHeaderFactory.h"
#include "../response/factory/ResponseFactory.h"
#include "Processor.h"
#include <sstream>
#include <map>

class FiltreResponseMimeType: public Processor
{
private:
	MimeTypeHelper *mimeTypeHelper;
	std::map<std::string, std::string> properties;
	ProcessorTypeEnum type;

public:
	FiltreResponseMimeType(ProcessorTypeEnum type);
	//	FiltreResponseMimeType(MimeTypeHelper &mimeTypeHelper);
	std::string getResponseMimeType(const std::string &filePath) const;

	virtual Response* process(Request *request, Response *response,
			ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
	virtual void setConfig(Config *conf);
	virtual std::string toString();
	virtual void addProperty(std::string name, std::string value);
	virtual ProcessorTypeEnum getType();

	std::string getProperty(const std::string &name) const;
	void reloadConfigurations();
};
