#include "MimeType.h"

MimeType::MimeType()
{
    std::ifstream file("example/mime.types");
    std::string line;

    if (!file.is_open())
        return; // Exit if file not found

    while (std::getline(file, line))
    {
        // Skip empty lines or lines not starting with 'text', 'image', 'application', etc.
        if (line.empty() || line[0] == ' ' || line[0] == '\t' || line[0] == '#')
            continue;

        std::istringstream lineStream(line);
        std::string mimeType;
        lineStream >> mimeType; // First token is the MIME type

        std::string extension;
        while (lineStream >> extension)
        {
            addMapping(extension, mimeType);
        }
    }

    file.close();
}

MimeType::~MimeType() {}

void MimeType::addMapping(const std::string &extension, const std::string &mimeType)
{
    mappings.push_back(std::make_pair(extension, mimeType));
}

std::string MimeType::findMimeType(const std::string &extension) const
{
    for (std::list<std::pair<std::string, std::string> >::const_iterator it = mappings.begin(); it != mappings.end(); ++it)
    {
        if (it->first == extension)
        {
            return it->second;
        }
    }
    return "application/octet-stream"; // Default MIME type if not found
}

void MimeType::setMimeType(const std::string &extension, const std::string &newMimeType)
{
    for (std::list<std::pair<std::string, std::string> >::iterator it = mappings.begin(); it != mappings.end(); ++it)
    {
        if (it->first == extension)
        {
            it->second = newMimeType; // Update existing MIME type
            return;                   // Exit after updating
        }
    }
    // If not found, add a new mapping
    addMapping(extension, newMimeType);
}
