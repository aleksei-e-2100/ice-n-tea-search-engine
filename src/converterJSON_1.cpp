/*
 *  ConverterJSON, часть 1
 *  Логика конструктора и публичных методов
*/

#include "converterJSON.h"

// Конструктор
ConverterJSON::ConverterJSON(const string& defaultProjectName,
                             const string& projectVersion,
                             const string& configJSON_filename, 
                             const string& requestsJSON_filename, 
                             const string& answerJSON_filename)
{
    this->configJSON_filename = configJSON_filename;
    this->requestsJSON_filename = requestsJSON_filename;
    this->answerJSON_filename = answerJSON_filename;
    this->projectVersion = projectVersion;

    nlohmann::json configJSON;

    setConfigJSON(configJSON);
    checkConfig(configJSON);
    checkTextDocuments(configJSON);
    
    if (configJSON["config"].contains("name"))
        projectName = configJSON["config"]["name"];
    else
        projectName = defaultProjectName;

    if (configJSON["config"].contains("max_responses"))
        maxResponses = configJSON["config"]["max_responses"];
    else
        maxResponses = 5;
    
    textDocuments = configJSON["files"];
}


// Метод получения названия проекта
string ConverterJSON::getProjectName()
{
    return projectName;
}   


// Метод получения списка текстовых файлов
vector<string> ConverterJSON::getTextDocuments()
{
    return textDocuments;
}   


// Метод получения предельного количества ответов
// на один запрос
int ConverterJSON::getResponsesLimit()
{
    return maxResponses;
}


// Метод записи результатов поисковых запросов
// в файл с ответами
void ConverterJSON::putAnswers() 
{
    // добавить параметры метода и логику
}
