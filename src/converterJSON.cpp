#include "converterJSON.h"

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

    getConfigJSON();
    checkConfig();
    checkTextDocumentsArray();
    
    if (configJSON["config"].contains("name"))
        projectName = configJSON["config"]["name"];
    else
        projectName = defaultProjectName;

    if (configJSON["config"].contains("max_responses"))
        maxResponses = configJSON["config"]["max_responses"];
    else
        maxResponses = 5;
}


// Метод для получения данных из файла конфигурации, проверки
// его на соответствие формату json, проверки наличия в нем
// необходимых полей
void ConverterJSON::getConfigJSON()
{
    ifstream file (configJSON_filename, ios::in);
    
    if(!file.is_open())
        throw runtime_error("config file is missing");
    
    try
    {
        file >> configJSON;
    }
    catch(const exception& e)
    {
        file.close();
        throw runtime_error("config file is not in json format");
    }
    
    file.close();

    if (!configJSON.contains("config") 
        || !configJSON.contains("files"))
        throw runtime_error("config file has required data");
}


// Метод для проверки конфигурации 
// (поле "config" в файле конфигурации)
void ConverterJSON::checkConfig()
{
    // наличие поля "version"
    if (!configJSON["config"].contains("version"))
        throw runtime_error("config file has no version data");

    // правильный формат данных в поле "version"
    if (!configJSON["config"]["version"].is_string())
        throw runtime_error("config file version data is incorrect");

    // соответствие версии файла конфигурации версии проекта
    if (configJSON["config"]["version"] != projectVersion)
        throw runtime_error("config file has incorrect version");

    // значение в поле "max_responses"
    if (configJSON["config"].contains("max_responses"))
    {
        if (!configJSON["config"]["max_responses"].is_number())
            throw runtime_error("max responses data is incorrect");
        
        int num = static_cast<int>(configJSON["config"]["max_responses"]);
        if (configJSON["config"]["max_responses"] > num)
            throw runtime_error("max responses data is incorrect");
    }

    // отсутствие лишних полей
    if (configJSON["config"].size() > 3)
        throw runtime_error("config data contains unnecessary information");

    // отсутствие лишних полей либо полей с некорректными значениями
    if (configJSON["config"].size() > 1)
    {
        for (auto it = configJSON["config"].begin(); 
                  it != configJSON["config"].end(); 
                  it++)
        {
        if (it.key() != "version" 
            && it.key() != "max_responses" 
            && (it.key() != "name" || !it.value().is_string()))
            throw runtime_error("config data is incorrect");
        }
    }
}


// Метод для проверки корректности списка текстовых файлов
// (поле "files" в файле конфигурации)
void ConverterJSON::checkTextDocumentsArray()
{
    // наличие сведений о файлах, содержание их в массиве
    if (configJSON["files"].size() == 0 
        || !configJSON["files"].is_array())
        throw runtime_error("text files data is incorrect");

    // имена файлов (пути к ним) в текстовом формате


}


// Метод для получения названия проекта
string ConverterJSON::getProjectName()
{
    return projectName;
}   

