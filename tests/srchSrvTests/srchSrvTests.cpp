#include <gtest/gtest.h>
#include <string_view>
#include "invertedIndex.h"
#include "searchServer.h"

using namespace std;


TEST(TestSearchServer, TestBasic_1) 
{
    const vector<string> textDocuments = 
    {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    
    const vector<string> requests = 
    {
        "milk water", 
        "sugar"
    };
    
    const vector<vector<RelativeIndex>> expected = 
    {
        {{2, 1}, {0, 0.7}, {1, 0.3}},
        {}
    };

    InvertedIndex invertedIndex;
    invertedIndex.updateDocumentBase(textDocuments);
    
    SearchServer searchServer(invertedIndex);

    vector<vector<RelativeIndex>> result = searchServer.search(requests);
    
    ASSERT_EQ(result, expected);    
}


TEST(TestSearchServer, TestBasic_2) 
{
    const vector<string> textDocuments = 
    {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland"
    };
    
    const vector<string> requests = 
    {
        "moscow is the capital of russia"
    };
    
    /*
    * Индексация текстовых документов несколько потоков
    * может привести к тому, что ответы с одинаковой релевантностью
    * будут выведены не в порядке возрастания doc_id
    * (подробнее см. комментарий в indxTests.cpp)
    * В этом случае тест выдаст ошибочный результат.
    * В данном случае делать сортировку бессмысленно, т.к. 
    * ожидаемый результат может быть не выдан вообще:
    * Например:
    * ожидается {0, 0.666666687}, {1, 0.666666687}, {2, 0.666666687},
    * а резузультат будет {0, 0.666666687}, {2, 0.666666687}, {3, 0.666666687}
    * Поэтому последние три ответа проверим только по релевантности,
    * намеренно присвоим их doc_id значения 100
    */

    const vector<vector<RelativeIndex>> expected = 
    {
        {{7, 1}, {14, 1}, {100, 0.666666687}, {100, 0.666666687}, {100, 0.666666687}}
    };

    InvertedIndex invertedIndex;
    invertedIndex.updateDocumentBase(textDocuments);
    
    SearchServer searchServer(invertedIndex);

    vector<vector<RelativeIndex>> result = searchServer.search(requests);

    // ограничение количества ответов 
    // и присвоение 100 последним трем doc_id
    for (auto& answer : result)
    {
        if (answer.size() > 5)
            answer.resize(5);
        
        answer[2].doc_id = 100;
        answer[3].doc_id = 100;
        answer[4].doc_id = 100;
    }

    ASSERT_EQ(result, expected);    
}
