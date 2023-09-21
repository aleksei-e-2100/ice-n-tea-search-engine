#include <iostream>
#include <string>

#include "converterJSON.h"
#include "invertedIndex.h"

using namespace std;

const string DEFAULT_PROJECT_NAME = "iCE && tEA Search Engine";
const string PROJECT_VERSION = "1.0";
const string CONFIG_JSON_FILENAME = "config.json";
const string REQUESTS_JSON_FILENAME = "requests.json";
const string ANSWER_JSON_FILENAME = "answer.json";

int main()
{
    ConverterJSON* converterJSON = nullptr;
    
    try
    {
        converterJSON = new ConverterJSON(DEFAULT_PROJECT_NAME,
                                          PROJECT_VERSION,
                                          CONFIG_JSON_FILENAME, 
                                          REQUESTS_JSON_FILENAME, 
                                          ANSWER_JSON_FILENAME);
    }
    catch(const exception& e)
    {
        cout << "Shutdown: " << e.what() << endl;
        return 0;
    }
    
    InvertedIndex* invertedIndex = new InvertedIndex();
    invertedIndex->updateDocumentBase(converterJSON->getTextDocuments());

    cout << "---" << endl;
    cout << converterJSON->getProjectName() << " " << PROJECT_VERSION << endl;
    cout << "---" << endl;
    
    delete converterJSON;
    delete invertedIndex;
}
