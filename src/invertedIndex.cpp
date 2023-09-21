/*
 *  InvertedIndex
 *  Логика конструктора и методов
*/

#include "invertedIndex.h"

// Оператор необходим для проведения тестовых сценариев    
bool Entry::operator ==(const Entry& other) const 
{
        return (doc_id == other.doc_id && count == other.count);
}


// Метод создания (обновления) индексированной базы документов
void InvertedIndex::updateDocumentBase(vector<string> textDocuments)
{
    setTextDocumentsContent(textDocuments);
    makeFreqDictionary();
}


// Метод получения содержимого текстовых файлов
void InvertedIndex::setTextDocumentsContent(vector<string>& textDocuments)
{
    ifstream file;
    string fileContent = "";
    string textLine = "";
        
    for (auto& textDocument : textDocuments)
    {
        file.open(textDocument);

        if (!file.is_open())
        {
            cout << "Error reading " << textDocument << endl;
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

        textDocumentsContent.push_back(fileContent);
        fileContent = "";
    }
}


// Метод создания инвертированного индекса
void InvertedIndex::makeFreqDictionary()
{
    int documentsCount = textDocumentsContent.size();
    vector<thread> threads;

    for (size_t doc_id = 0; doc_id < documentsCount; doc_id++)
        threads.push_back(thread(countWords, this, textDocumentsContent[doc_id], doc_id));
    
    for (auto& th : threads)
        th.join();
}


// Метод подсчета слов в текстовом документе
void InvertedIndex::countWords(string fileContent, size_t doc_id)
{
    map<string, size_t> words;
    string word;
    stringstream ss(fileContent);
    
    while (ss >> word)
    {
        words[word]++;
    }
    
    freqDictionary_access.lock();
    for (auto it = words.begin(); it != words.end(); it++)
    {
        Entry entry {doc_id, it->second};
        freqDictionary[it->first].push_back(entry);
    }
    freqDictionary_access.unlock();    
}

// Метод получения количества вхождений слова в базе документов
vector<Entry> InvertedIndex::getWordCount(string word)
{
    vector<Entry> wordCount;

    if (freqDictionary.count(word) > 0)
        wordCount = freqDictionary[word];
    
    return wordCount;
}
