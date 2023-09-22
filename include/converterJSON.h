#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>

#include "nlohmann/json.hpp"

using namespace std;

class ConverterJSON
{
private:
    string appVersion;

    string configJSON_filename;
    string requestsJSON_filename;
    string answerJSON_filename;

    nlohmann::json configJSON;
    void setConfigJSON();

public:
    ConverterJSON(const string& appVersion,
                  const string& configJSON_filename, 
                  const string& requestsJSON_filename, 
                  const string& answerJSON_filename);
    
    string getAppName();
    vector<string> getTextDocuments();
    int getResponsesLimit();
    void putAnswers();
};

#endif  // CONVERTER_JSON_H
