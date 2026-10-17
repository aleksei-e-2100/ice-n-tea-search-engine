/**
 *  @file  index-tests.cpp
 *  @brief iCE && tEA Search Engine unit tests
 */

#include "inverted_index.h"

#include <fstream>

#include <gtest/gtest.h>

// Prepares data file for testing
void prepareFile(const std::string& fileName, const std::string& document)
{
    std::ofstream file(fileName);

    if (file.is_open())
    {
        file << document;
        file.close();
    }
}

// Tests the functionality of the InvertedIndex class
void testInvertedIndexFunctionality(std::vector<std::string> fileNames,
                                    std::vector<std::string> words,
                                    std::vector<std::vector<Entry>> expected)
{
    std::vector<std::vector<Entry>> result;
    
    InvertedIndex invertedIndex;
    invertedIndex.buildInvertedIndex(fileNames);
    
    for(auto word : words)
    {
        std::vector<Entry> wordEntries = invertedIndex.getWordEntries(word);
        result.push_back(wordEntries);
    }

    /*
       Indexing is performed in multiple threads.
       If a subsequent thread finishes earlier than a preceding one,
       it will write its entry to freqDictionary first, and the records
       of word occurrences will not be ordered by the document numbers.

       For example:
       a word appears twice in document 0 and three times in document 1.
       If the threads finish in order, the record will be:
       {0, 2}, {1, 3}
       But if the second thread finishes earlier than the first, the record will be:
       {1, 3}, {0, 2}

       This does not affect the application’s functionality — the records are correct.
       However, it critically affects the test results and may lead to a failure:
       the expected result is {0, 2}, {1, 3}, while the actual result is {1, 3}, {0, 2}.
       Even though the result is valid, the test will not pass.

       Therefore, it is important to order (sort) the result here, which we do below:
    */

    for (auto& vec : result)
    {
        if (vec.size() < 2)
            continue;
        
        for (auto i = 0; i < vec.size(); ++i)
        {
            for (auto j = 0; j < vec.size() - 1 - i; ++j)
            {
                if (vec[j].docId > vec[j + 1].docId)
                    std::swap(vec[j], vec[j + 1]);
            }
        }
    }

    ASSERT_EQ(result, expected);
}

TEST(TestInvertedIndex, TestBasic_1) 
{
    std::vector<std::string> fileNames;

    for (auto i = 0; i < 2; ++i)
    {
        fileNames.push_back("text_1_" + std::to_string(i) + ".txt");
    }

    prepareFile(fileNames[0], "london is the capital of great britain");
    prepareFile(fileNames[1], "big ben is the nickname for the great bell of the striking clock");

    const std::vector<std::string> words =
    {
        "london", 
        "the"
    };
    
    const std::vector<std::vector<Entry>> expected =
    {
        {{0, 1}}, 
        {{0, 1}, {1, 3}}
    };

    testInvertedIndexFunctionality(fileNames, words, expected);
}

TEST(TestInvertedIndex, TestBasic_2) 
{
    std::vector<std::string> fileNames;

    for (auto i = 0; i < 4; ++i)
    {
        fileNames.push_back("text_2_" + std::to_string(i) + ".txt");
    }

    prepareFile(fileNames[0], "milk milk milk milk water water water");
    prepareFile(fileNames[1], "milk water water");
    prepareFile(fileNames[2], "milk milk milk milk milk water water water water water");
    prepareFile(fileNames[3], "americano cappuchino");

    const std::vector<std::string> words =
    {
        "milk", 
        "water",
        "cappuchino"
    };
    
    const std::vector<std::vector<Entry>> expected =
    {
        {{0, 4}, {1, 1}, {2, 5}},
        {{0, 3}, {1, 2}, {2, 5}},
        {{3, 1}}
    };

    testInvertedIndexFunctionality(fileNames, words, expected);
}

TEST(TestInvertedIndex, TestBasic_3) 
{
    std::vector<std::string> fileNames;

    for (auto i = 0; i < 2; ++i)
    {
        fileNames.push_back("text_3_" + std::to_string(i) + ".txt");
    }

    prepareFile(fileNames[0], "a b c d e f g h i g k l");
    prepareFile(fileNames[1], "statement");

    const std::vector<std::string> words =
    {
        "m", 
        "statement"
    };
    
    const std::vector<std::vector<Entry>> expected =
    {
        {},
        {{1, 1}}
    };

    testInvertedIndexFunctionality(fileNames, words, expected);
}
