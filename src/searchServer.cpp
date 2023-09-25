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
        // разбитие поискового запроса на отдельные слова,
        // подсчет общего количества вхождений
        unordered_map<string, vector<Entry>> words;
        unordered_map<string, size_t> wordsTotalCount;

        stringstream ss(currentRequest);
        string word;
        
        while (ss >> word)
        {
            vector<Entry> wordEntries = invertedIndex.getWordCount(word);

            // пропускаем слово, которое не встречается в текстовых документах
            if (wordEntries.empty())
                continue;
            
            words[word] = wordEntries;
            
            size_t totalCount = 0;
            for (auto& entry : words[word])
            {
                totalCount += entry.count;
            }
            
            wordsTotalCount[word] = totalCount;
        }

        // если ни одно из слов в запросе не входит в текстовые документы,
        // в т.ч. если запрос пустой
        if (words.empty())
        {
            answers.push_back({});
            continue;
        }

        // сортировка каждого найденного слова 
        // по возрастанию количества вхождений
        vector<string> wordsSorted;
        
        for (auto i = 0; i < words.size(); i++)
        {
            auto minTotalCount_it = wordsTotalCount.begin();
            auto minTotalCount = minTotalCount_it->second;
            
            for (auto it = minTotalCount_it; 
                      it != wordsTotalCount.end(); 
                      it++)
            {
                if (it->second < minTotalCount)
                {
                    minTotalCount_it = it;
                    minTotalCount = it->second;
                }
            }
            
            wordsSorted.push_back(minTotalCount_it->first);
            wordsTotalCount.erase(minTotalCount_it->first);
        }

        // поиск релевантных документов
        // в данном случае в поле count структуры Entry будет записана
        // сумма вхождений всех найденных слов в документ
        // (до этого структура Entry использовалась для каждого отдельного слова)
        vector<Entry> relativeDocuments = words[wordsSorted[0]];
        
        for (auto i = 1; i < wordsSorted.size(); i++)
        {
            vector<Entry> checkingDocuments = words[wordsSorted[i]];
            auto relativeDocumentsCount = relativeDocuments.size();

            for (auto rel_d = 0; rel_d < relativeDocumentsCount; rel_d++)
            {
                for (auto check_d = 0; check_d < checkingDocuments.size(); check_d++)
                {
                    if (checkingDocuments[check_d].doc_id == relativeDocuments[rel_d].doc_id)
                    {
                        relativeDocuments[rel_d].count += checkingDocuments[check_d].count;
                        checkingDocuments.erase(checkingDocuments.begin() + check_d);
                    }
                }
            }

            for (auto check_d = 0; check_d < checkingDocuments.size(); check_d++)
                relativeDocuments.push_back(checkingDocuments[check_d]);
        }

        // сортировка найденных документов по убыванию релевантности
        for (auto i = 0; i < relativeDocuments.size(); i++)
        {
            for (auto j = 0; j < relativeDocuments.size() - 1 - i; j++)
            {
                if (relativeDocuments[j].count < relativeDocuments[j + 1].count)
                    swap(relativeDocuments[j], relativeDocuments[j + 1]);
            }
        }
    
        // расчет относительной релевантности, запись ответа на запрос
        size_t maxAbsRelevance = relativeDocuments[0].count;
        vector<RelativeIndex> currentRequestAnswer;

        for (auto entry : relativeDocuments)
        {
            currentRequestAnswer.push_back(
                    {entry.doc_id, 
                    static_cast<float>(entry.count) / static_cast<float>(maxAbsRelevance)});
        }

        answers.push_back(currentRequestAnswer);
    }

    return answers;
}
