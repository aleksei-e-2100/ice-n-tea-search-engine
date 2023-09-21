/*
 *  ConverterJSON, часть 2
 *  Логика методов работы с configJSON
*/

#include "converterJSON.h"

// Метод получения данных из файла конфигурации, проверки
// его на соответствие формату json, проверки наличия в нем
// необходимых полей
void ConverterJSON::setConfigJSON(nlohmann::json& configJSON)
{
    ifstream file(configJSON_filename);
    
    if (!file.is_open())
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
        throw runtime_error("config file has no required data");
}


// Метод проверки конфигурации (поле "config" в файле конфигурации)
void ConverterJSON::checkConfig(const nlohmann::json& configJSON)
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


// Метод проверки корректности списка текстовых файлов
// (поле "files" в файле конфигурации)
void ConverterJSON::checkTextDocuments(const nlohmann::json& configJSON)
{
    // наличие сведений о файлах, содержание их в массиве
    if (configJSON["files"].size() == 0 
        || !configJSON["files"].is_array())
        throw runtime_error("text files data is incorrect");
    
    // имена файлов (пути к ним) в текстовом формате
    for (auto &filename : configJSON["files"])
    {
        if (!filename.is_string())
            throw runtime_error("text files data is incorrect");
    }
}
