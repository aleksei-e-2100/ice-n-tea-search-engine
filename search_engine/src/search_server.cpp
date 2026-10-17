/**
 *  @file  search-server.cpp
 *  @brief iCE && tEA Search Engine module.
 *         Provides functionality for processing search requests using an inverted index.
 */

#include "search_server.h"

#include <utility>
#include <spanstream>
#include <unordered_map>
#include <string>

#include "inverted_index.h"

bool RelativeIndex::operator==(const RelativeIndex& other) const 
{
    return (docId == other.docId && rank == other.rank);
}

// Метод обработки поисковых запросов
std::vector<std::vector<RelativeIndex>> SearchServer::search(
    InvertedIndex* invertedIndex, const std::vector<std::string>& requests)
{
    std::vector<std::vector<RelativeIndex>> answers;

    for (auto& request : requests)
    {
        // 1. Split a request into individual words, count the total number of occurrences
        std::unordered_map<std::string, std::vector<Entry>> wordsEntries;
        std::unordered_map<std::string, size_t> wordsCounts;

        // std::ispanstream works without copying
        std::ispanstream stream(request);

        std::string word;
        
        while (stream >> word)
        {
            if (!invertedIndex->tokenizeWord(word))
                continue;

            std::vector<Entry> wordEntries = std::move(invertedIndex->getWordEntries(word));

            // Skip a word that is not found in any document
            if (wordEntries.empty())
                continue;
            
            wordsEntries[word] = wordEntries;
            
            size_t wordCount = 0;

            for (const auto& entry : wordsEntries[word])
            {
                wordCount += entry.count;
            }
            
            wordsCounts[word] = wordCount;
        }

        // If none of the words in the request are found in documents
        // (incl. if the request is empty)
        if (wordsEntries.empty())
        {
            answers.push_back({});
            continue;
        }

        // 2. Sort each found word by increasing number of occurrences
        std::vector<std::string> wordsSorted;
        
        for (auto i = 0; i < wordsEntries.size(); ++i)
        {
            auto minCountIt = wordsCounts.begin();
            auto minCount = minCountIt->second;
            
            for (auto it = minCountIt;
                      it != wordsCounts.end();
                      ++it)
            {
                if (it->second < minCount)
                {
                    minCountIt = it;
                    minCount = it->second;
                }
            }
            
            wordsSorted.push_back(minCountIt->first);
            wordsCounts.erase(minCountIt);
        }

        /*
           3. Search for relative documents.
              The count field of the Entry class will contain the sum of occurrences of all found
              words in the document (previously, the Entry class was used for each individual word).
        */
        std::vector<Entry> relativeDocuments = wordsEntries[wordsSorted[0]];
        
        for (auto i = 1; i < wordsSorted.size(); ++i)
        {
            std::vector<Entry> compareDocuments = wordsEntries[wordsSorted[i]];

            for (auto rd = 0; rd < relativeDocuments.size(); ++rd)
            {
                for (auto cd = 0; cd < compareDocuments.size(); ++cd)
                {
                    if (compareDocuments[cd].docId == relativeDocuments[rd].docId)
                    {
                        relativeDocuments[rd].count += compareDocuments[cd].count;
                        compareDocuments.erase(compareDocuments.begin() + cd);
                        --cd;
                    }
                }
            }

            for (auto cd = 0; cd < compareDocuments.size(); ++cd)
                relativeDocuments.push_back(compareDocuments[cd]);
        }

        // 4. Sort found documents in descending order of relevance
        for (auto i = 0; i < relativeDocuments.size(); i++)
        {
            for (auto j = 0; j < relativeDocuments.size() - 1 - i; j++)
            {
                if (relativeDocuments[j].count < relativeDocuments[j + 1].count)
                    std::swap(relativeDocuments[j], relativeDocuments[j + 1]);
            }
        }
    
        // 5. Calculate of relative relevance, push the response
        size_t maxAbsRelevance = relativeDocuments[0].count;

        std::vector<RelativeIndex> requestAnswer;

        for (auto entry : relativeDocuments)
        {
            requestAnswer.push_back(
                {entry.docId,
                 static_cast<double>(entry.count) / static_cast<double>(maxAbsRelevance)});
        }

        answers.push_back(requestAnswer);
    }

    return answers;
}
