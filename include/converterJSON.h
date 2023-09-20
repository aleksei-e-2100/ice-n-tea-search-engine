#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <fstream>
#include <string>
#include <vector>
#include <exception>

#include "nlohmann/json.hpp"

using namespace std;

class ConverterJSON
{
private:
    string projectName;
    string projectVersion;
    
    string configJSON_filename;
    string requestsJSON_filename;
    string answerJSON_filename;

    vector<string> textDocuments;
    int maxResponses;

    void setConfigJSON(nlohmann::json& configJSON);
    void checkConfig(const nlohmann::json& configJSON);
    void checkTextDocuments(const nlohmann::json& configJSON);

public:
    ConverterJSON(const string& defaultProjectName,
                  const string& projectVersion,
                  const string& configJSON_filename, 
                  const string& requestsJSON_filename, 
                  const string& answerJSON_filename);

    string getProjectName();
    vector<string> getTextDocuments();
    int getResponsesLimit();
    void putAnswers(); // добавить параметры
};

#endif  // CONVERTER_JSON_H
