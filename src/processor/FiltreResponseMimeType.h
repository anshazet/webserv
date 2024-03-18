#pragma once
#include "../mimeType/MimeTypeHelper.h"
#include "../request/RequestHttp.h"
#include "../response/ResponseHttp.h"
#include "Processor.h"

class FiltreResponseMimeType : public Processor
{
private:
	MimeTypeHelper &mimeTypeHelper;

public:
	FiltreResponseMimeType(MimeTypeHelper &mimeTypeHelper);
	std::string getResponseMimeType(const std::string &filePath) const;

	virtual Response *process(Request *request, Response *response,
							  ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
	virtual void setConfig(Config *conf);
	virtual std::string toString();
	virtual void addProperty(std::string name, std::string value); // passer parameter
};
// regarder Response* ProcessorImplDirectFs::process(Request *request, Response *response,
//		ProcessorAndLocationToProcessor *processorAndLocationToProcessor)