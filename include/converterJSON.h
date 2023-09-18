#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <iostream>  // убрать
#include <fstream>
#include <string>
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

    nlohmann::json configJSON;
    int maxResponses;

    void getConfigJSON();
    void checkConfig();
    void checkTextDocumentsArray();

public:
    ConverterJSON(const string& defaultProjectName,
                  const string& projectVersion,
                  const string& configJSON_filename, 
                  const string& requestsJSON_filename, 
                  const string& answerJSON_filename);

    string getProjectName();
    
};

#endif  // CONVERTER_JSON_H
