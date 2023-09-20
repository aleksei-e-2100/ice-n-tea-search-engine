/*
 *  InvertedIndex
 *  Логика конструктора и методов
*/

#include "invertedIndex.h"

// Метод для создания (обновления) индексированной базы документов,
// по которым будет совершаться поиск
void InvertedIndex::updateDocumentBase(vector<string> textDocuments)
{
    setTextDocumentsContent(textDocuments);
    makeFreqDictionary();
}


// Метод для получения содержимого текстовых файлов
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


// Метод для создания инвертированного индекса
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
// (для каждого текстового документа запускается в отдельном потоке)
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
