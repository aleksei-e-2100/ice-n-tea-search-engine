/*
 *  ConverterJSON, часть 2
 *  Логика метода setConfigJSON()
*/

#include "converterJSON.h"

// Метод получения данных из файла конфигурации и их проверки
void ConverterJSON::setConfigJSON()
{
    ifstream file(configJSON_filename);
    
    // проверка: открывается (существует ли) файл
    if (!file.is_open())
        throw runtime_error("config file is missing");
    
    // проверка: соответствует ли файл формату json
    // (если да, то информация из него записывается в переменную)
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

    // Проверка: есть ли необходимые ключи "config" и "files"
    if (!configJSON.contains("config") 
        || !configJSON.contains("files"))
        throw runtime_error("config file has no required data");
    
    // Проверка: нет ли лишних данных (лишних ключей)
    if (configJSON.size() > 2)
        throw runtime_error("config file contains unnecessary information");

    // Проверка значений ключа "config":
    // - есть ли ключ "version"
    if (!configJSON["config"].contains("version"))
        throw runtime_error("config file has no version data");

    // - правильный ли тип значения ключа "version"
    if (!configJSON["config"]["version"].is_string())
        throw runtime_error("config file version data is incorrect");

    // - соответствует ли версия файла конфигурации версии приложения
    if (configJSON["config"]["version"] != appVersion)
        throw runtime_error("config file has incorrect version");

    // - правильный ли тип значения ключа "max_responses"
    //   (если такой ключ есть)
    if (configJSON["config"].contains("max_responses"))
    {
        if (!configJSON["config"]["max_responses"].is_number())
            throw runtime_error("max responses data is incorrect");
        
        int num = static_cast<int>(configJSON["config"]["max_responses"]);
        if (configJSON["config"]["max_responses"] > num
            || configJSON["config"]["max_responses"] < 1)
            throw runtime_error("max responses data is incorrect");
    }

    // - нет ли лишних данных (лишних ключей)
    if (configJSON["config"].size() > 3)
        throw runtime_error("config data contains unnecessary information");

    // - нет ли лишних данных (ключей) и соответствуют ли значения
    //   тех ключей, что есть, необходимым типам
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

    // Проверка значений ключа "files":
    // - является ли значение ключа "files" массивом, не пустой ли он
    if (configJSON["files"].size() == 0 
        || !configJSON["files"].is_array())
        throw runtime_error("text files data is incorrect");
    
    // - являются ли имена файлов текстовыми значениями
    for (auto &filename : configJSON["files"])
    {
        if (!filename.is_string())
            throw runtime_error("text files data is incorrect");
    }
}
