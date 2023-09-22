#include <gtest/gtest.h>
#include <string_view>
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
    * Если последующий поток завершится раньше предшествующего, он
    * раньше него внесет запись в словарь freqDictionary, и записи
    * о вхождении того или иного слова будут расположены не по
    * порядку номеров текстовых документов.
    * Например: 
    * слово входит в документ 0 два раза и в документ 1 три раза.
    * Если потоки завершаться по порядку, запись будет такая:
    * {0, 2}, {1, 3}
    * Если же второй поток завершиться раньше первого, запись будет такая:
    * {1, 3}, {0, 2}
    * На функционал приложения это никак не влияет. Записи корректны.
    * Но на результаты теста это влияет критически и может привести к 
    * ошибочному результату: ожидаемый результ {0, 2}, {1, 3}, а 
    * фактический результат - {1, 3}, {0, 2}. Хотя результат и верный,
    * тест пройден не будет.
    * Поэтому здесь важно упорядочить (отсортировать) результат, что и сделаем:
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
        "london is the capital of great britain",
        "big ben is the nickname for the great bell of the striking clock"
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
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuchino"
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
        "a b c d e f g h i g k l",
        "statement"
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
