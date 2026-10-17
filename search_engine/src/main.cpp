/**
 *  @file  main.cpp
 *  @brief iCE && tEA Search Engine entry point
 */

#include <iostream>

#include "settings.h"
#include "json_converter.h"
#include "inverted_index.h"
#include "search_server.h"

int main()
{
    std::cout << std::endl;
    std::cout << appName << " " << appName << std::endl;
    std::cout << "---" << std::endl;

    JsonConverter jsonConverter(appVersion,
                                configFname,
                                requestsFname,
                                answersFname,
                                responsesLimit);

    if (!jsonConverter.init())
    {
        std::cout << "  can't run: " << jsonConverter.getLastErrorStr() << std::endl;
        std::cout << std::endl;

        return -1;
    }
    
    std::cout << "  indexing documents..." << std::endl;

    InvertedIndex invertedIndex;

    if (!invertedIndex.buildInvertedIndex(jsonConverter.getFilenames()))
    {
        std::cout << "  can't run: " << jsonConverter.getLastErrorStr() << std::endl;
        std::cout << std::endl;

        return -2;
    }

    std::cout << "  processing requests..." << std::endl;

    SearchServer searchServer;

    auto answers = searchServer.search(&invertedIndex, jsonConverter.getRequests());

    std::cout << "  writing answers..." << std::endl;

    if (!jsonConverter.writeAnswers(answers))
    {
        std::cout << "  can't run: " << jsonConverter.getLastErrorStr() << std::endl;
        std::cout << std::endl;

        return -3;
    }

    std::cout << "Finished" << std::endl;
    std::cout << std::endl;

    return 0;
}
