/*
 *  InvertedIndex
 *  Логика конструктора и методов
*/

#include "invertedIndex.h"


// Оператор необходим для тестовых сценариев
bool Entry::operator ==(const Entry& other) const 
{
        return (doc_id == other.doc_id && count == other.count);
}


// Метод создания базы текстовых документов с инвертированным индексом
void InvertedIndex::updateDocumentBase(vector<string> inTextDocuments)
{
    freqDictionary.clear();
    textDocuments = inTextDocuments;
    
    vector<thread> threads;

    for (auto doc_id = 0; doc_id < textDocuments.size(); doc_id++)
        threads.emplace_back(&InvertedIndex::countWordsInDocument, 
                             this, 
                             textDocuments[doc_id], 
                             doc_id);
    
    for (auto& th : threads)
        th.join();
}


// Метод подсчета слов в одном текстовом документе
mutex freqDictionary_access;
void InvertedIndex::countWordsInDocument(const string& textDocument, size_t doc_id)
{
    map<string, size_t> words;
    string word;
    stringstream ss(textDocument);
    
    while (ss >> word)
    {
        words[word]++;
    }
    
    freqDictionary_access.lock();
    for (auto it = words.begin(); it != words.end(); it++)
    {
        freqDictionary[it->first].push_back({doc_id, it->second});
    }
    freqDictionary_access.unlock();    
}


// Метод получения количества вхождений конкретного слова во всей базе документов
vector<Entry> InvertedIndex::getWordCount(string word)
{
    vector<Entry> wordCount;

    if (freqDictionary.count(word) > 0)
        wordCount = freqDictionary[word];
    
    return wordCount;
}
