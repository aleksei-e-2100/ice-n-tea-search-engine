#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include <string> //
#include <vector>
#include <unordered_map>
#include <sstream>

#include "invertedIndex.h"

using namespace std;

struct RelativeIndex
{
    size_t doc_id;
    float rank;

    bool operator ==(const RelativeIndex& other) const;
};



class SearchServer
{
private:
    InvertedIndex invertedIndex;

public:
    SearchServer(InvertedIndex& invertedIndex);

    vector<vector<RelativeIndex>> search(const vector<string>& requests);
};

#endif  // SEARCH_SERVER_H