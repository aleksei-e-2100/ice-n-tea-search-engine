/**
 *  @file  inverted-index.cpp
 *  @brief iCE && tEA Search Engine module.
 *         Provides functionality for building and using an inverted index.
 */

#include "inverted_index.h"

#include <fstream>
#include <spanstream>
#include <functional>
#include <cctype>

bool Entry::operator==(const Entry& other) const 
{
    return (docId == other.docId && count == other.count);
}

bool InvertedIndex::buildInvertedIndex(const std::vector<std::string> &fileNames)
{
    dictionary_.clear();
    documents_.clear();

    fileNames_ = fileNames;

    // Set the vector size so that it no longer moves around in memory and can be read without a mutex
    documents_.resize(fileNames_.size(), "");

    /*
       Multithreaded reading of files from disk is inefficient (not always efficient). Therefore,
       the main thread always reads, file by file, and what it reads sends to the threadpool. The
       number of threads in the threadpool corresponds to the number of cores. One thread is not
       enough, because if a large file is read, the threadpool will process it and may not have
       time if the next file read is very small. In general, idle threads in the threadpool sleep
       and do not load the processor.
    */
    
    threadPool_.run();

    for (size_t docId = 0; docId < fileNames_.size(); ++docId)
    {
        if (!readFile(docId))
        {
            errorStr_ = "can't read file " + fileNames_[docId];
            return false;
        }

        threadPool_.enqueue(std::bind(&InvertedIndex::countWords, this, docId));
    }

    // It is important to wait for the thread pool threads to finish running, otherwise, when the
    // application finishes, they may encounter already destroyed methods of the main class.
    threadPool_.wait();

    return true;
}

bool InvertedIndex::tokenizeWord(std::string& word)
{
    size_t start = 0;
    size_t end = word.size();

    while (start < end && std::ispunct(static_cast<unsigned char>(word[start])))
    {
        ++start;
    }

    while (end > start && ispunct(static_cast<unsigned char>(word[end - 1])))
    {
        --end;
    }

    word = word.substr(start, end - start);

    if (word.empty())
        return false;

    return true;
}

std::vector<Entry> InvertedIndex::getWordEntries(const std::string& word)
{
    std::vector<Entry> entries;

    if (dictionary_.count(word) > 0)
        entries = dictionary_[word];

    return entries;
}

const std::string &InvertedIndex::getLastErrorStr() const
{
    return errorStr_;
}

bool InvertedIndex::readFile(const size_t docId)
{
    std::string textLine = "";

    std::ifstream file(fileNames_[docId]);

    if (!file.is_open())
        return false;

    while (!file.eof())
    {
        getline(file, textLine);
        documents_[docId] = documents_[docId] + textLine + " ";
        textLine = "";
    }

    file.close();

    return true;
}

void InvertedIndex::countWords(const size_t docId)
{
    std::map<std::string, size_t> words;
    std::string word;

    // std::ispanstream works without copying
    std::ispanstream stream(documents_[docId]);

    while (stream >> word)
    {
        if (tokenizeWord(word))
            words[word]++;
    }

    // Free up memory
    documents_[docId] = "";

    std::lock_guard<std::mutex> lock(m_);

    for (auto it = words.begin(); it != words.end(); ++it)
    {
        dictionary_[it->first].push_back({docId, it->second});
    }
}
