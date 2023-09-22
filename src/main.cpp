#include <iostream>
#include <string>

#include "converterJSON.h"
#include "invertedIndex.h"
#include "searchServer.h"

using namespace std;

const string DEFAULT_APP_NAME = "iCE && tEA Search Engine";
const string APP_VERSION = "1.0";
const string CONFIG_JSON_FILENAME = "config.json";
const string REQUESTS_JSON_FILENAME = "requests.json";
const string ANSWER_JSON_FILENAME = "answer.json";

int main()
{
    ConverterJSON* converterJSON = nullptr;
    
    try
    {
        converterJSON = new ConverterJSON(APP_VERSION,
                                          CONFIG_JSON_FILENAME, 
                                          REQUESTS_JSON_FILENAME, 
                                          ANSWER_JSON_FILENAME);
    }
    catch(const exception& e)
    {
        cout << "App cannot start: " << e.what() << endl;
        return 0;
    }
    
    string appName = converterJSON->getAppName();
    if (appName == "") appName = DEFAULT_APP_NAME;

    cout << "---" << endl;
    cout << appName << " " << APP_VERSION << endl;
    cout << "---" << endl;

    InvertedIndex* invertedIndex = new InvertedIndex();
    cout << "Indexing documents base..." << endl;
    invertedIndex->updateDocumentBase(converterJSON->getTextDocuments());
    cout << "Ok" << endl;

    SearchServer* searchServer = new SearchServer(*invertedIndex);
    cout << "Processing requests..." << endl;
    vector<vector<RelativeIndex>> answers = 
            searchServer->search(converterJSON->getRequests());
    cout << "Ok" << endl;

    cout << endl;
    delete converterJSON;
    delete invertedIndex;
    delete searchServer;
}
