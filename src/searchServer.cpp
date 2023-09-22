/*
 *  SearchServer
 *  Логика конструктора и методов
*/

#include "searchServer.h"

// Оператор необходим для тестовых сценариев ---- уточнить
bool RelativeIndex::operator ==(const RelativeIndex& other) const 
{
    return (doc_id == other.doc_id && rank == other.rank);
}


// Конструктор
SearchServer::SearchServer(InvertedIndex& invertedIndex)
{
    this->invertedIndex = invertedIndex;
}


// Метод обработки поисковых запросов
vector<vector<RelativeIndex>> SearchServer::search(const vector<string>& requests)
{
    vector<vector<RelativeIndex>> answers;
    
    for (auto& currentRequest : requests)
    {
        unordered_map<string, vector<Entry>> wordsUnsorted;
        unordered_map<string, vector<Entry>> wordsSorted;

        string word;
        stringstream ss(currentRequest);
    
        while (ss >> word)
        {
            wordsUnsorted[word] = invertedIndex.getWordCount(word);
            
            int totalCount = 0;
            for (auto& entry : wordsUnsorted[word])
            {
                totalCount += entry.count;
            }

        }
            
        
        for (auto it = wordsUnsorted.begin(); it != wordsUnsorted.end(); it++)
        {

        
        }

    }
    
    return answers;
}
