/*
 *  ConverterJSON, часть 3
 *  Логика метода setRequestsJSON()
*/

#include "converterJSON.h"

// Метод получения данных из файла запросов и их проверки
void ConverterJSON::setRequestsJSON()
{
    ifstream file(requestsJSON_filename);
    
    // проверка: открывается (существует ли) файл
    if (!file.is_open())
        throw runtime_error("requests file is missing");
    
    // проверка: соответствует ли файл формату json
    // (если да, то информация из него записывается в переменную)
    try
    {
        file >> requestsJSON;
    }
    catch(const exception& e)
    {
        file.close();
        throw runtime_error("requests file is not in json format");
    }
    
    file.close();

    // Проверка: есть ли необходимый ключ "requests"
    if (!requestsJSON.contains("requests"))
        throw runtime_error("requests file has no required data");
    
    // Проверка: нет ли лишних данных (лишних ключей)
    if (requestsJSON.size() > 1)
        throw runtime_error("requests file contains unnecessary information");

    // Проверка значений ключа "requests":
    // - является ли значение ключа "requests" массивом, не пустой ли он
    if (requestsJSON["requests"].size() == 0 
        || !requestsJSON["requests"].is_array())
        throw runtime_error("requests data is incorrect");
    
    // - являются ли запросы текстовыми значениями
    for (auto &request : requestsJSON["requests"])
    {
        if (!request.is_string())
            throw runtime_error("requests data is incorrect");
    }
}
