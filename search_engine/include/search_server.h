/**
 *  @file  search-server.h
 *  @brief iCE && tEA Search Engine module.
 *         Provides functionality for processing search requests using an inverted index.
 */

#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include <vector>
#include <string>

class InvertedIndex;

/**
 *  @class RelativeIndex
 *  @brief Represents a document's relevance score for a specific request.
 *         Contains the document identifier and its calculated rank.
 */
class RelativeIndex
{
public:
    /**
     *  @brief Comparison operator. Used in tests.
     */
    bool operator==(const RelativeIndex& other) const;

public:
    size_t docId;  ///< Identifier of the document
    double rank;   ///< Relevance score (rank) of the document
};

/**
 *  @class SearchServer
 *  @brief Implements search for request's words across the provided files,
 *         producing ranked and sorted results.
 */
class SearchServer
{
public:
    /**
     *  @brief Сonstructor
     */
    SearchServer() = default;

    /**
     *  @brief  Performs search for request's words using the provided inverted index.
     *          Produces ranked and sorted results for each query.
     *  @param  invertedIndex - pointer to an InvertedIndex instance (uses its built
     *          index and word tokenization methods)
     *  @param  requests - search requests
     *  @return A vector of vectors of RelativeIndex records representing results for
     *          each request
     */
    std::vector<std::vector<RelativeIndex>> search(
        InvertedIndex* invertedIndex, const std::vector<std::string>& requests);
};

#endif  // SEARCH_SERVER_H
