#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

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
    vector<string> textDocuments;
    map<string, vector<Entry>> freqDictionary;

    void countWordsInDocument(const string& fileContent, size_t doc_id);

public:
    InvertedIndex() = default;

    void updateDocumentBase(vector<string> inTextDocuments);
    vector<Entry> getWordCount(string word);
};

#endif  // INVERTED_INDEX_H