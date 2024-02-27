#include "CGIHandler.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sstream>
#include <sys/wait.h>
#include <sstream>

CGIHandler::CGIHandler()
{
}

CGIHandler::~CGIHandler()
{
}

std::string CGIHandler::executeCGI(const std::string &scriptPath, const std::map<std::string, std::string> &envVariables)
{
    // Set environment variables
    for (std::map<std::string, std::string>::const_iterator it = envVariables.begin(); it != envVariables.end(); ++it)
    {
        setenv(it->first.c_str(), it->second.c_str(), 1);
    }

    // Execute CGI script and capture the output
    std::string command = "exec " + scriptPath;
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
        return "ERROR";

    char buffer[128];
    std::string result = "";
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    int status = pclose(pipe);
    if (WIFEXITED(status))
    {
        int exitStatus = WEXITSTATUS(status);
        if (exitStatus != 0)
        {
            // Handle non-zero exit status as an error
            std::ostringstream oss;
            oss << exitStatus;
            logError("CGI script exited with error status: " + oss.str());
            return "ERROR";
        }
    }
    else
    {
        // Handle cases where the script didn't exit normally
        logError("CGI script did not exit normally.");
        return "ERROR";
    }

    // Reset environment variables
    for (std::map<std::string, std::string>::const_iterator it = envVariables.begin(); it != envVariables.end(); ++it)
    {
        unsetenv(it->first.c_str());
    }

    // Parse and store the CGI output
    parseOutput(result);

    return responseBody;
}

void CGIHandler::parseOutput(const std::string &output)
{
    std::istringstream stream(output);
    std::string line;
    bool headerSection = true; // Start by parsing headers

    responseHeaders.clear(); // Clear previous headers if any
    responseBody.clear();    // Clear previous body if any

    while (std::getline(stream, line))
    {
        // Check and remove carriage return at the end if present
        if (!line.empty() && line[line.length() - 1] == '\r')
        {
            line.erase(line.length() - 1);
        }

        if (headerSection)
        {
            // Check for the empty line indicating end of headers
            if (line.empty())
            {
                headerSection = false; // Headers end, body starts
                continue;
            }

            // Parse header line
            std::size_t pos = line.find(":");
            if (pos != std::string::npos)
            {
                std::string headerName = line.substr(0, pos);
                std::string headerValue = line.substr(pos + 2); // Skip ": "
                responseHeaders[headerName] = headerValue;
            }
        }
        else
        {
            // The first line of the body will be an empty line, skip it
            if (line.empty())
            {
                continue;
            }
            // The rest of the stream is the body
            responseBody.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
            break; // Exit after body is read
        }
    }
}

void CGIHandler::logError(const std::string &message)
{
    std::ofstream logFile("error_log.txt", std::ios::app); // Append to the log file
    if (logFile)
    {
        logFile << message << std::endl;
    }
}

/*
void CGIHandler::logError(const std::string &message, int exitStatus)
{
    std::stringstream ss;
    ss << exitStatus; // Convert integer to string
    std::string exitStatusStr = ss.str();

    std::ofstream logFile("error_log.txt", std::ios::app); // Append to the log file
    if (logFile)
    {
        logFile << message << ": " << exitStatusStr << std::endl;
    }
}
*/

void CGIHandler::logSuccess(const std::string &message)
{
    std::ofstream logFile("access_log.txt", std::ios::app); // Append to the log file
    if (logFile)
    {
        logFile << message << std::endl;
    }
}
