/*
 *  ConverterJSON, часть 1
 *  Логика конструктора и публичных методов
*/

#include "converterJSON.h"


// Конструктор
ConverterJSON::ConverterJSON(const string& appVersion,
                             const string& configJSON_filename, 
                             const string& requestsJSON_filename, 
                             const string& answerJSON_filename)
{
    this->appVersion = appVersion;
    this->configJSON_filename = configJSON_filename;
    this->requestsJSON_filename = requestsJSON_filename;
    this->answerJSON_filename = answerJSON_filename;

    setConfigJSON();
    // setRequestsJSON();
    // setAnswerJSON();
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


// Метод получения предельного количества ответов
// на один запрос
int ConverterJSON::getResponsesLimit()
{
    if (configJSON["config"].contains("max_responses"))
        return configJSON["config"]["max_responses"];
    else
        return -1;
}


// Метод записи результатов поисковых запросов
// в файл с ответами
void ConverterJSON::putAnswers() 
{
    // TODO
}
