/**
 *  @file  inverted-index.h
 *  @brief iCE && tEA Search Engine module.
 *         Provides functionality for building and using an inverted index.
 */

#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <string>
#include <vector>
#include <map>
#include <mutex>

#include "thread_pool.h"

/**
 *  @class Entry
 *  @brief Implements a record indicating:
 *         1) how many times a specific word appears in a given document, or
 *         2) how many times all words from a request appear in a given document
 */
class Entry
{
public:
    /**
     *  @brief Comparison operator. Used in tests.
     */
    bool operator==(const Entry& other) const;

public:
    size_t docId;  ///< Identifier of the document
    size_t count;  ///< Number of occurrences of the word (or request words) in the document
};

/**
 *  @class InvertedIndex
 *  @brief Implements building of an inverted index and operations on it
 */
class InvertedIndex
{
public:
    /**
     *  @brief Constructor
     */
    InvertedIndex() = default;

    /**
     *  @brief  Builds the inverted index
     *  @param  fileNames - names of files in which search should be performed
     *  @return true - if the index was successfully built, false - otherwise
     */
    bool buildInvertedIndex(const std::vector<std::string> &fileNames);

    /**
     *  @brief  Tokenizes a word (removes prefix and suffix punctuations)
     *  @param  word - the word to tokenize
     *  @return true - if the word was successfully tokenized,
     *          false - if the word became empty after tokenization
     *          (contained only punctuation chars)
     */
    bool tokenizeWord(std::string& word);

    /**
     *  @brief Returns a vector of Entry records indicating how many times
     *         a specific word appears in the documents provided for search
     *  @param word - the specific word
     */
    std::vector<Entry> getWordEntries(const std::string &word);

    /**
     *  @brief Returns a text message about the last error
     */
    const std::string& getLastErrorStr() const;

private:
    /**
     *  @brief  Reads a file into the documents_ vector
     *  @param  docId - the file identifier
     *  @return true - if the file was successfully read, false - otherwise
     */
    bool readFile(const size_t docId);

    /**
     *  @brief Counts words in a file, builds Entry records for them,
     *         and stores the results in dictionary_
     *  @param docId - the file identifier
     */
    void countWords(const size_t docId);

private:
    ThreadPool threadPool_;

    std::vector<std::string> fileNames_;
    std::vector<std::string> documents_;

    std::map<std::string, std::vector<Entry>> dictionary_;

    std::mutex m_;

    std::string errorStr_;
};

#endif  // INVERTED_INDEX_H
