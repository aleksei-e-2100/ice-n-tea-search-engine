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
};

#endif  // INVERTED_INDEX_H