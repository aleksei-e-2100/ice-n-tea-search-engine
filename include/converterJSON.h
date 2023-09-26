#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>

#include "nlohmann/json.hpp"

#include "searchServer.h"

using namespace std;


class ConverterJSON
{
private:
    string appVersion;
    int responsesLimit;

    string configJSON_filename;
    string requestsJSON_filename;
    string answersJSON_filename;

    nlohmann::json configJSON;
    void setConfigJSON();

    nlohmann::json requestsJSON;
    void setRequestsJSON();

    nlohmann::ordered_json answersJSON;

public:
    ConverterJSON(const string& appVersion,
                  const string& configJSON_filename, 
                  const string& requestsJSON_filename, 
                  const string& answersJSON_filename);
    
    string getAppName();
    vector<string> getTextDocuments();
    vector<string> getRequests();
    
    void putAnswers(vector<vector<RelativeIndex>>& answers);
};

#endif  // CONVERTER_JSON_H
