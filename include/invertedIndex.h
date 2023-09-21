#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <thread>
#include <mutex>

using namespace std;

struct Entry
{
    size_t doc_id, count;
    bool operator ==(const Entry& other) const;
};

class InvertedIndex
{
private:
    vector<string> textDocumentsContent;
    map<string, vector<Entry>> freqDictionary;

    void setTextDocumentsContent(vector<string>& textDocuments);
    void makeFreqDictionary();
    
    mutex freqDictionary_access;
    void countWords(string fileContent, size_t doc_id);

public:
    InvertedIndex() = default;

    void updateDocumentBase(vector<string> textDocuments);
    void testUpdateDocumentBase(vector<string> texts);

    vector<Entry> getWordCount(string word);
};

#endif  // INVERTED_INDEX_H