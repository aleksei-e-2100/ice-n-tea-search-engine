/**
 *  @file  json-converter.cpp
 *  @brief iCE && tEA Search Engine module.
 *         Provides functionality for working with JSON files.
 */

#include "json_converter.h"

#include <exception>
#include <fstream>
#include <format>

#include "search_server.h"

JsonConverter::JsonConverter(
        const std::string& appVersion,
        const std::string& configFname,
        const std::string& requestsFname,
        const std::string& answersFname,
        const int responsesLimit)
{
    appVersion_ = appVersion;
    configFname_ = configFname;
    requestsFname_ = requestsFname;
    answersFname_ = answersFname;
    responsesMax_ = responsesLimit;
}

bool JsonConverter::init()
{
    if (!setConfigJson())
    {
        errorStr_ = configFname_ + " error";
        return false;
    }

    if (!setRequestsJson())
    {
        errorStr_ = requestsFname_ + " error";
        return false;
    }

    setFilenames();
    setRequests();

    return true;
}

std::vector<std::string> JsonConverter::getFilenames() const
{
    return fileNames_;
}

std::vector<std::string> JsonConverter::getRequests() const
{
    return requests_;
}

bool JsonConverter::writeAnswers(std::vector<std::vector<RelativeIndex>>& answers)
{
    std::string requestId;
    int requestNum = 1;

    for (auto& answer : answers)
    {
        std::string requestNumStr = std::format("{:03}", requestNum);
        requestId = "request" + requestNumStr;

        ++requestNum;

        if (answer.empty())
        {
            answersJson_["answers"][requestId]["result"] = false;
            continue;
        }

        answersJson_["answers"][requestId]["result"] = true;

        if (answer.size() == 1)
        {
            answersJson_["answers"][requestId]["doc_id"] = answer[0].docId;
            answersJson_["answers"][requestId]["rank"] = answer[0].rank;
            continue;
        }

        int responseNum = 1;

        for (auto& response : answer)
        {
            if (responseNum >= responsesMax_)
                break;

            nlohmann::json responseEntry =
                {{"doc_id", response.docId}, {"rank", response.rank}};

            answersJson_["answers"][requestId]["relevance"].emplace_back(responseEntry);

            ++responseNum;
        }
    }

    std::ofstream file(answersFname_);

    if (!file.is_open())
    {
        errorStr_ = answersFname_ + " error";
        return false;
    }

    file << std::setw(4) << answersJson_;

    file.close();

    return true;
}

const std::string& JsonConverter::getLastErrorStr() const
{
    return errorStr_;
}

bool JsonConverter::setConfigJson()
{
    std::ifstream file(configFname_);
    
    if (!file.is_open())
        return false;
    
    // Check if the file is in JSON format
    try
    {
        file >> configJson_;
    }
    catch(const std::exception& e)
    {
        file.close();
        return false;
    }
    
    file.close();

    // Check if the required "config" and "files" keys are present
    if (!configJson_.contains("config") ||
        !configJson_.contains("files"))
    {
        return false;
    }
    
    // Check whether there is extra data (extra keys)
    if (configJson_.size() != 2)
        return false;

    // Check the values of the "config" key
    if (configJson_["config"].size() != 1 && configJson_["config"].size() != 2)
        return false;

    if (!configJson_["config"].contains("version"))
        return false;

    if (!configJson_["config"]["version"].is_string())
        return false;

    if (configJson_["config"]["version"] != appVersion_)
        return false;

    if (configJson_["config"].size() == 2 &&
        !configJson_["config"].contains("max_responses"))
    {
        return false;
    }

    if (configJson_["config"].contains("max_responses") &&
        !configJson_["config"]["max_responses"].is_number())
    {
        return false;
    }

    if (configJson_["config"].contains("max_responses"))
    {
        int tmp = static_cast<int>(configJson_["config"]["max_responses"]);

        if (tmp > responsesMax_)
            return false;
        else
            responsesMax_ = tmp;
    }

    // Check the values of the "files" key
    if (configJson_["files"].size() == 0 ||
        !configJson_["files"].is_array())
    {
        return false;
    }
    
    for (auto &filename : configJson_["files"])
    {
        if (!filename.is_string())
            return false;
    }

    return true;
}

bool JsonConverter::setRequestsJson()
{
    std::ifstream file(requestsFname_);
    
    if (!file.is_open())
        return false;
    
    // Check if the file is in JSON format
    try
    {
        file >> requestsJson_;
    }
    catch(const std::exception& e)
    {
        file.close();
        return false;
    }
    
    file.close();

    // Check if the required "requests" key presents
    if (!requestsJson_.contains("requests"))
        return false;
    
    // Check whether there is extra data (extra keys)
    if (requestsJson_.size() > 1)
        return false;

    // Check the values of the "requests" key
    if (requestsJson_["requests"].size() == 0 ||
        !requestsJson_["requests"].is_array())
    {
        return false;
    }
    
    for (auto &request : requestsJson_["requests"])
    {
        if (!request.is_string())
            return false;
    }

    return true;
}

void JsonConverter::setFilenames()
{
    for (auto& filename : configJson_["files"])
    {
        fileNames_.emplace_back(std::string(filename));
    }
}

void JsonConverter::setRequests()
{
    for (auto& request : requestsJson_["requests"])
    {
        requests_.emplace_back(std::string(request));
    }
}
