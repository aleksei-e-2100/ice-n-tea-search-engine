/**
 *  @file  search-tests.cpp
 *  @brief iCE && tEA Search Engine unit tests
 */

#include "search_server.h"

#include <fstream>

#include <gtest/gtest.h>

#include "inverted_index.h"

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

TEST(TestSearchServer, TestBasic_1) 
{
    std::vector<std::string> fileNames;

    for (auto i = 0; i < 4; ++i)
    {
        fileNames.push_back("text_1_" + std::to_string(i) + ".txt");
    }

    prepareFile(fileNames[0], "milk milk milk milk water water water");
    prepareFile(fileNames[1], "milk water water");
    prepareFile(fileNames[2], "milk milk milk milk milk water water water water water");
    prepareFile(fileNames[3], "americano cappuccino");

    const std::vector<std::string> requests =
    {
        "milk water", 
        "sugar"
    };
    
    const std::vector<std::vector<RelativeIndex>> expected =
    {
        {{2, 1}, {0, 0.7}, {1, 0.3}},
        {}
    };

    InvertedIndex invertedIndex;
    invertedIndex.buildInvertedIndex(fileNames);
    
    SearchServer searchServer;
    auto result = searchServer.search(&invertedIndex, requests);

    ASSERT_EQ(result, expected);
}

TEST(TestSearchServer, TestBasic_2)
{
    std::vector<std::string> fileNames;

    for (auto i = 0; i < 4; ++i)
    {
        fileNames.push_back("text_1_" + std::to_string(i) + ".txt");
    }

    prepareFile(fileNames[0], "milk. milk: milk, milk water water water .."); // Not tokenized words
    prepareFile(fileNames[1], "milk water water");
    prepareFile(fileNames[2], "milk milk milk milk milk water water water water water");
    prepareFile(fileNames[3], "americano cappuccino");

    const std::vector<std::string> requests =
    {
        "milk.. water ::", // Not tokenized words
        "sugar"
    };

    const std::vector<std::vector<RelativeIndex>> expected =
    {
        {{2, 1}, {0, 0.7}, {1, 0.3}},
        {}
    };

    InvertedIndex invertedIndex;
    invertedIndex.buildInvertedIndex(fileNames);

    SearchServer searchServer;
    auto result = searchServer.search(&invertedIndex, requests);

    ASSERT_EQ(result, expected);
}


TEST(TestSearchServer, TestBasic_3)
{
    std::vector<std::string> fileNames;

    for (auto i = 0; i < 22; ++i)
    {
        fileNames.push_back("text_2_" + std::to_string(i) + ".txt");
    }

    prepareFile(fileNames[0], "london is the capital of great britain");
    prepareFile(fileNames[1], "paris is the capital of france");
    prepareFile(fileNames[2], "berlin is the capital of germany");
    prepareFile(fileNames[3], "rome is the capital of italy");
    prepareFile(fileNames[4], "madrid is the capital of spain");
    prepareFile(fileNames[5], "lisboa is the capital of portugal");
    prepareFile(fileNames[6], "bern is the capital of switzerland");
    prepareFile(fileNames[7], "minsk is the capital of belarus");
    prepareFile(fileNames[8], "astana is the capital of kazakhstan");
    prepareFile(fileNames[9], "beijing is the capital of china");
    prepareFile(fileNames[10], "tokyo is the capital of japan");
    prepareFile(fileNames[11], "bangkok is the capital of thailand");
    prepareFile(fileNames[12], "amsterdam is the capital of netherlands");
    prepareFile(fileNames[13], "helsinki is the capital of finland");
    prepareFile(fileNames[14], "oslo is the capital of norway");
    prepareFile(fileNames[15], "stockholm is the capital of sweden");
    prepareFile(fileNames[16], "riga is the capital of latvia");
    prepareFile(fileNames[17], "tallinn is the capital of estonia");
    prepareFile(fileNames[18], "warsaw is the capital of poland");
    prepareFile(fileNames[19], "we visit stockholm it is the capital of sweden");
    prepareFile(fileNames[20], "copenhagen is the capital of denmark");
    prepareFile(fileNames[21], "vienna is the capital of austria");

    const std::vector<std::string> requests =
    {
        "stockholm is the capital of sweden"
    };
    
    /*
       Indexing text documents in multiple threads
       may lead to answers with identical relevance
       being output not in ascending order of docId
       (see the detailed comment in indxTests.cpp).
       In this case, the test will produce an incorrect result.

       Sorting is meaningless here, because the expected result
       may not appear at all.

       For example:
       expected: {0, 0.666667}, {1, 0.666667}, {2, 0.666667}
       actual:   {0, 0.666667}, {2, 0.666667}, {3, 0.666667}

       Therefore, we check only the relevance of the last three answers,
       assigning docId the value 100.
    */

    const std::vector<std::vector<RelativeIndex>> expected =
    {
        {{15, 1},                    // "stockholm is the capital of sweden"
         {19, 1},                    // "we visit stockholm. it is the capital of sweden"
         {100, 0.6666666666666666},  // any other (see below)
         {100, 0.6666666666666666},  // any other (see below)
         {100, 0.6666666666666666}}  // any other (see below)
    };

    InvertedIndex invertedIndex;
    invertedIndex.buildInvertedIndex(fileNames);
    
    SearchServer searchServer;
    auto result = searchServer.search(&invertedIndex, requests);

    // Limit the number of responses and assign 100 to the last three docIds
    for (auto& answer : result)
    {
        if (answer.size() > 5)
            answer.resize(5);
        
        answer[2].docId = 100;
        answer[3].docId = 100;
        answer[4].docId = 100;
    }
    
    ASSERT_EQ(result, expected);    
}
