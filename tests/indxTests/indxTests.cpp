#include <gtest/gtest.h>
#include "invertedIndex.h"

using namespace std;

// Функция тестирования функциональности класса InvertedIndex
void testInvertedIndexFunctionality(const vector<string> textDocuments,
                                    const vector<string> requests,
                                    const vector<vector<Entry>> expected) 
{
    vector<vector<Entry>> result;
    
    InvertedIndex invertedIndex;
    invertedIndex.updateDocumentBase(textDocuments);
    
    for(auto request : requests) 
    {
        vector<Entry> wordCount = invertedIndex.getWordCount(request);
        result.push_back(wordCount);
    }

    /*
    * Индексация выполняется в несколько потоков.
    * В случае, если следующий поток завершиться раньше предшествующего,
    * в словаре freqDictionary записи о документах, в которые входит слово, 
    * будут не по порядку. 
    * Например:
    * слово входит в документ 0 два раза и в документ 1 три раза.
    * Если потоки завершаться по порядку, запись будет такая:
    * {0, 2}, {1, 3}
    * Если же второй поток завершиться раньше первого, запись будет такая:
    * {1, 3}, {0, 2}
    * Для целей данного приложения это не играет никакой роли. Но при
    * тестировании, это может привести к ошибочному результату теста - 
    * ожидание теста {0, 2}, {1, 3}, а результат - {1, 3}, {0, 2}.
    * Поэтому здесь важно упорядочить результат, что и сделаем:
    */

    for (auto& vec : result)
    {
        if (vec.size() < 2)
            continue;
        
        for (auto i = 0; i < vec.size(); i++)
        {
            for (auto j = 0; j < vec.size() - 1 - i; j++)
            {
                if (vec[j].doc_id > vec[j + 1].doc_id)
                    swap(vec[j], vec[j + 1]);
            }
        }
    }

    ASSERT_EQ(result, expected);
}


TEST(TestInvertedIndex, TestBasic_1) 
{
    const vector<string> textDocuments = 
    {
        "test_text_file_1.0.txt",
        "test_text_file_1.1.txt"
    };
    
    const vector<string> requests = 
    {
        "london", 
        "the"
    };
    
    const vector<vector<Entry>> expected = 
    {
        {{0, 1}}, 
        {{0, 1}, {1, 3}}
    };

    testInvertedIndexFunctionality(textDocuments, requests, expected);
}


TEST(TestInvertedIndex, TestBasic_2) 
{
    const vector<string> textDocuments = 
    {
        "test_text_file_2.0.txt",
        "test_text_file_2.1.txt",
        "test_text_file_2.2.txt",
        "test_text_file_2.3.txt"
    };
    
    const vector<string> requests = 
    {
        "milk", 
        "water",
        "cappuchino"
    };
    
    const vector<vector<Entry>> expected = 
    {
        {{0, 4}, {1, 1}, {2, 5}},
        {{0, 3}, {1, 2}, {2, 5}},
        {{3, 1}}
    };

    testInvertedIndexFunctionality(textDocuments, requests, expected);
}


TEST(TestInvertedIndex, TestBasic_3) 
{
    const vector<string> textDocuments = 
    {
        "test_text_file_3.0.txt",
        "test_text_file_3.1.txt"
    };
    
    const vector<string> requests = 
    {
        "m", 
        "statement"
    };
    
    const vector<vector<Entry>> expected = 
    {
        {},
        {{1, 1}}
    };

    testInvertedIndexFunctionality(textDocuments, requests, expected);
}
