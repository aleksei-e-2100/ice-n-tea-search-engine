/*
 *  ConverterJSON
 *  Логика конструктора и методов
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


// Метод получения названия приложения
string ConverterJSON::getAppName() const
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
vector<string> ConverterJSON::getRequests() const
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

    if (!file.is_open())
        throw runtime_error("can't create (open) answers file");

    file << setw(4) << answersJSON;
    file.close();
}


// Метод проверки файла с ответами
void ConverterJSON::checkAnswersFile()
{
    ifstream file(answersJSON_filename); 

    if (!file.is_open())
        throw runtime_error("can't check answers in answers file");

    nlohmann::ordered_json checkAnswersJSON;

    try
    {
        file >> checkAnswersJSON;
    }
    catch(const exception& e)
    {
        file.close();
        throw runtime_error("answers were put incorrectly");
    }

    file.close();

    if(checkAnswersJSON != answersJSON)
        throw runtime_error("answers were put incorrectly");
}
