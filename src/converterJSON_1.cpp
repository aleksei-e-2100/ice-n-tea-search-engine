/*
 *  ConverterJSON, часть 1
 *  Логика конструктора и публичных методов
*/

#include "converterJSON.h"


// Конструктор
ConverterJSON::ConverterJSON(const string& appVersion,
                             const string& configJSON_filename, 
                             const string& requestsJSON_filename, 
                             const string& answersJSON_filename)
{
    this->appVersion = appVersion;
    this->configJSON_filename = configJSON_filename;
    this->requestsJSON_filename = requestsJSON_filename;
    this->answersJSON_filename = answersJSON_filename;

    setConfigJSON();
    setRequestsJSON();
    
    if (configJSON["config"].contains("max_responses"))
        responsesLimit = configJSON["config"]["max_responses"];
    else
        responsesLimit = 5;
}


// Метод получения названия приложения
string ConverterJSON::getAppName()
{
    if (configJSON["config"].contains("name"))
        return configJSON["config"]["name"];
    else
        return "";
}   


// Метод получения содержимого текстовых файлов
vector<string> ConverterJSON::getTextDocuments()
{
    vector<string> textDocuments;
    
    ifstream file;
    string fileContent = "";
    string textLine = "";
        
    for (auto& filename : configJSON["files"])
    {
        file.open(filename);

        if (!file.is_open())
        {
            cout << "Error reading " << filename << endl;
        }
        else
        {
            while (!file.eof())
            {
                getline(file, textLine);
                fileContent = fileContent + textLine + " ";
                textLine = "";
            }
            
            file.close();

            fileContent.erase(fileContent.length(), 1);
        }

        textDocuments.push_back(fileContent);
        fileContent = "";
    }
    
    return textDocuments;
}   


// Метод получения содержимого запросов
vector<string> ConverterJSON::getRequests()
{
    return requestsJSON["requests"];
}


// Метод записи результатов поисковых запросов
// в файл с ответами
void ConverterJSON::putAnswers(vector<vector<RelativeIndex>>& answers) 
{
    string requestID;
    int requestNum = 1;
    
    for (auto& currentAnswer : answers)
    {
        if (requestNum < 10)
            requestID = "request00" + to_string(requestNum);
        else if (requestNum < 100)
            requestID = "request0" + to_string(requestNum);
        else
            requestID = "request" + to_string(requestNum);
        
        requestNum ++;
        if (currentAnswer.empty())
        {
            answersJSON["answers"][requestID]["result"] = false;
            continue;
        }

        answersJSON["answers"][requestID]["result"] = true;

        if (currentAnswer.size() == 1)
        {
            answersJSON["answers"][requestID]["doc_id"] = currentAnswer[0].doc_id;
            answersJSON["answers"][requestID]["rank"] = currentAnswer[0].rank;
            continue;
        }

        int responseNum = 1;
        for (auto& response : currentAnswer)
        {
            if (responseNum >= responsesLimit) 
                break;
            
            nlohmann::json responseEntry = 
                    {{"doc_id", response.doc_id}, {"rank", response.rank}};

            
            answersJSON["answers"][requestID]["relevance"].emplace_back(responseEntry);

            responseNum++;
        }
    }

    ofstream file(answersJSON_filename); 
    file << setw(4) << answersJSON;
    file.close();
}
