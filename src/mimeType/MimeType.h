#pragma once
#include <list>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>

class MimeType
{
private:
    std::list<std::pair<std::string, std::string> > mappings;

public:
    MimeType();
    ~MimeType();
    void addMapping(const std::string &extension, const std::string &mimeType);
    std::string findMimeType(const std::string &extension) const;
    void setMimeType(const std::string &extension, const std::string &newMimeType);
};