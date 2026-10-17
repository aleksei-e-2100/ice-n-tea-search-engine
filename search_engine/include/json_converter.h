/**
 *  @file  json-converter.h
 *  @brief iCE && tEA Search Engine module.
 *         Provides functionality for working with JSON files.
 */

#ifndef JSON_CONVERTER_H
#define JSON_CONVERTER_H

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class RelativeIndex;

/**
 *  @class JsonConverter
 *  @brief Implements operations with JSON files
 */
class JsonConverter
{
public:
    /**
     *  @brief Constructor
     *  @param appVersion - application version
     *  @param configFname - configuration JSON file name
     *  @param requestsFname - JSON file name containing search requests
     *  @param answersFname - JSON file name for writing responses
     *  @param responsesLimit - maximum number of responses per request
     */
    JsonConverter(const std::string& appVersion,
                  const std::string& configFname,
                  const std::string& requestsFname,
                  const std::string& answersFname,
                  const int responsesLimit);

    /**
     *  @brief Initializes the class instance and validates configuration data
     *  @return true - if initialization succeeded, false - otherwise
     */
    bool init();
    
    /**
     *  @brief Returns the names of files in which search should be performed
     */
    std::vector<std::string> getFilenames() const;

    /**
     *  @brief Returns the search requests
     */
    std::vector<std::string> getRequests() const;

    /**
     *  @brief Writes answers to the JSON output file
     *  @param answers - search results to be written to the file
     */
    bool writeAnswers(std::vector<std::vector<RelativeIndex>>& answers);

    /**
     *  @brief Returns a text message about the last error
     */
    const std::string& getLastErrorStr() const;

private:
    /**
     *  @brief  Validates the configuration JSON file and stores configuration data
     *  @return true - if configuration is valid, false - otherwise
     */
    bool setConfigJson();

    /**
     *  @brief  Validates the requests JSON file and stores request data
     *  @return true - if requests are valid, false - otherwise
     */
    bool setRequestsJson();

    /**
     *  @brief Extracts the list of filenames from the configuration JSON file and
     *         stores them in the class instance
     */
    void setFilenames();

    /**
     *  @brief Extracts the requests from the requests JSON file and
     *         stores them in the class instance
     */
    void setRequests();

private:
    std::string appVersion_;
    std::string configFname_;
    std::string requestsFname_;
    std::string answersFname_;

    nlohmann::json configJson_;
    nlohmann::json requestsJson_;
    nlohmann::ordered_json answersJson_;

    std::vector<std::string> fileNames_;
    std::vector<std::string> requests_;

    int responsesMax_;

    std::string errorStr_;
};

#endif  // JSON_CONVERTER_H
