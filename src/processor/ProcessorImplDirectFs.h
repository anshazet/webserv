#pragma once
#include "Processor.h"

#include "../Harl.h"
#include "../response/API/ResponseHeader.h"
#include "../response/factory/ResponseHeaderFactory.h"

#include "../util/FileUtilFactory.h"
#include "../util/StringUtil.h"
#include "../config/Config.h"
#include "../response/factory/ResponseFactory.h"
#include "../location/ProcessorAndLocationToProcessor.h"

class ProcessorImplDirectFs: public Processor
{
private:
	Config *config;
	FileUtil fileUtil;
	Harl harl;
	StringUtil stringUtil;
	ProcessorTypeEnum type;

public:
	ProcessorImplDirectFs(ProcessorTypeEnum type);
	~ProcessorImplDirectFs();
	Response* process(Request *request, Response *response,
			ProcessorAndLocationToProcessor *processorAndLocationToProcessor);
	void setConfig(Config *conf);
	virtual std::string toString();
	virtual void addProperty(std::string name, std::string value);
	virtual ProcessorTypeEnum getType();
};
