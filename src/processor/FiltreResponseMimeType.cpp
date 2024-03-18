#include "FiltreResponseMimeType.h"
#include <algorithm>

FiltreResponseMimeType::FiltreResponseMimeType(MimeTypeHelper &mimeTypeHelper)
    : mimeTypeHelper(mimeTypeHelper) {}

std::string FiltreResponseMimeType::getResponseMimeType(const std::string &filePath) const
{
    // Extract the file extension from the filePath
    std::string::size_type dotPos = filePath.rfind('.');
    if (dotPos == std::string::npos)
    {
        return "application/octet-stream"; // Default MIME type
    }

    std::string extension = filePath.substr(dotPos + 1);

    // Convert to lowercase to standardize
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    // Use the MimeTypeHelper to find the MIME type
    return mimeTypeHelper.findMimeType(extension);
}

// Modify the Response object to include the correct MIME type header
Response *FiltreResponseMimeType::process(Request *request, Response *response,
                                          ProcessorAndLocationToProcessor *processorAndLocationToProcessor)
{
    // Cast the Request and Response to their derived types if you're sure about the object types.
    RequestHttp *httpReq = static_cast<RequestHttp *>(request);
    ResponseHttp *httpResp = static_cast<ResponseHttp *>(response);

    if (!httpReq || !httpResp)
        return response; // Safety check

    // Use getPath() from RequestHttp to get the path (assuming it gives the file path)
    std::string path = httpReq->getPath();

    // Extracting MIME type using the file path
    std::string mimeType = getResponseMimeType(path);

    // Assuming ResponseHttp's header can be used to set MIME type.
    // If direct manipulation isn't possible, you might need to adjust how headers are set.
    ResponseHeader *respHeader = httpResp->getHeader();
    if (respHeader)
    {
        // Construct the Content-Type header string
        std::string contentTypeHeader = "Content-Type: " + mimeType;
        // Add this header to the response. This assumes you have a method to add headers
        // directly or manipulate the header's fields in ResponseHeader.
        // This is a placeholder for how you might add the header.
        respHeader->addField(contentTypeHeader);
    }

    return response;
}

void FiltreResponseMimeType::setConfig(Config *conf)
{
    // Example implementation, adjust according to how Config is designed
    // For example, if Config allows retrieving properties by name:
    std::string mimeTypesFilePath = conf->getProperty("mimeTypesFilePath");
    if (!mimeTypesFilePath.empty())
    {
        // Assuming MimeTypeHelper can be reinitialized or updated with a new file
        mimeTypeHelper.loadMappingsFromFile(mimeTypesFilePath);
    }
}

std::string FiltreResponseMimeType::toString()
{
    return "FiltreResponseMimeType Processor";
}

void FiltreResponseMimeType::addProperty(std::string name, std::string value)
{
    // Assuming Processor has a std::map or similar to store properties:
    // properties[name] = value;
    // But since there's no direct indication of property storage in Processor,
    // this is a placeholder. You need to adjust this based on your actual implementation.
}
